/*
Description:
An STL file describes a triangulated three-dimensional surface by
the unit normal and vertices(ordered by the right-hand rule) of
the triangles. This example saves a 3D geometric data stored in a 
vtkPolyData to an STL file using vtkSTKWriter.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkPolyData.h"
#include "vtkSTLWriter.h"
#include "vtkSTLReader.h"
#include "vtkSphereSource.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"

using namespace std;

int main()
{
	string strDstFile = "./TestSTLWriter.stl";

	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->Update();

	vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
	stlWriter->SetFileName( strDstFile.c_str() );
	stlWriter->SetInputConnection( sphereSource->GetOutputPort() );
	stlWriter->Write();
	stlWriter->Update();

	//read and display for verification
	vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName( strDstFile.c_str() );
	reader->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( reader->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( 0.5, 0.9, 0.3 );

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize( 600, 400 );
	renderWindow->SetPosition( 500, 150 );
	
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}