/*
Description:
This example demonstrates how to read a *.obj file.
The result is displayed.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include <iostream>

#include "vtkOBJReader.h"
#include "vtkOBJExporter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"

using namespace std;

int main()
{
	string strFile = "../Bin/ExampleFiles/cube.obj";

	vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
	reader->SetFileName( strFile.c_str() );
	reader->Update();

	//Visualization
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( reader->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( 0.3, 0.2, 0.8 );

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );

	//Test Export Obj File begin
	vtkSmartPointer<vtkOBJExporter> objExporter = vtkSmartPointer<vtkOBJExporter>::New();
	objExporter->SetInput( renderWindow );
	objExporter->SetFilePrefix( "../Bin/ExampleFiles/OBJ_File_Export_Test.obj" );
	objExporter->Write();
	objExporter->Update();
	//Test Export Obj File end

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow( renderWindow );

	renderWindowInteractor->Start();
	return EXIT_SUCCESS;
}