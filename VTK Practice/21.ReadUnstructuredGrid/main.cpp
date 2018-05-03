/*
Description:
This example demonstrates how to read an unstructured grid(VTU)file.
The front facing are colored white, while the back facing are colored
red.
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
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->EdgeVisibilityOn();
	actor->GetProperty()->SetLineWidth( 2.0 );

	vtkSmartPointer<vtkProperty> backFace = vtkSmartPointer<vtkProperty>::New();
	backFace->SetColor( colors->GetColor3d( "tomato" ).GetData() );
	actor->SetBackfaceProperty( backFace );

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