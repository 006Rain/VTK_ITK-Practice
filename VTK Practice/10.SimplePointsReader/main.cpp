/*
Description:
A file of XYZ coordinates is a very easy format that can be used for
simple storage from many applications. This example demonstrates how
to read this type of file into a polydata object. An example file of
these points would look like this:
0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0 0.0
As a demo, save the above three lines to a file called simple.xyz.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkSimplePointsReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

using namespace std;

int main()
{
	string strFile = "../Bin/ExampleFiles/simple.xyz";

	//Read File
	vtkSmartPointer<vtkSimplePointsReader> reader = vtkSmartPointer<vtkSimplePointsReader>::New();
	reader->SetFileName( strFile.c_str() );
	reader->Update();

	//Visualzie
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( reader->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->SetPointSize( 10.5 );//Set the diameter of a point. The size is expressed in screen units.

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( 0.3, 0.4, 0.5 );//background color

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );

	vtkSmartPointer<vtkRenderWindowInteractor> rendWindowInter = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rendWindowInter->SetRenderWindow( renderWindow );

	rendWindowInter->Start();
	return EXIT_SUCCESS;
}