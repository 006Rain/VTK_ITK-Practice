/*
Description:
This example demonstrates how to read an unstructured grid(VTU)file.
The front facing are colored white, while the back facing are colored
red.
*/

/*
* 知识点：
*	Actor的属性中关于颜色的设置只有当Actor的Mapper没有标量数据(ScalarData)时才起作用。
*	缺省情况下，Mapper输入的标量数据会对Actor进行着色，而Actor的颜色设置会被忽略。
*	如果要忽略这些标量数据，可以使用方法ScalarVisibilityOff()。
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkNamedColors.h"

using namespace std;

int main()
{
	string strSrcFile = "../Bin/ExampleFiles/tetra.vtu";

	//read all data from the file
	vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
	reader->SetFileName( strSrcFile.c_str() );
	reader->Update();

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	//Create a mapper and actor
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputConnection( reader->GetOutputPort() );
	mapper->ScalarVisibilityOff();//忽略标量数据，启用actor自己设置的颜色

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->EdgeVisibilityOn();
	actor->GetProperty()->SetLineWidth( 2.0 );

	vtkSmartPointer<vtkProperty> backFace = vtkSmartPointer<vtkProperty>::New();
#if 0 //actor自己设定颜色，前提是调用mapper->ScalarVisibilityOff();
	backFace->SetColor( 0, 255, 0 );
	actor->SetProperty( backFace );
#else
	backFace->SetColor( colors->GetColor3d( "tomato" ).GetData() );
	actor->SetBackfaceProperty( backFace );
#endif

	//Create a renderer, render window and interactor
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( colors->GetColor3d( "Wheat" ).GetData() );
	
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize( 640, 480 );
	
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow( renderWindow );

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}