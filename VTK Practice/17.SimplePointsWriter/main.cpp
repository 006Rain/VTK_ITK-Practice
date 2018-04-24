/*
Description:
This example demonstrates how to write actors to an 
simple points file.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkSimplePointsWriter.h"

int main()
{
	vtkSmartPointer<vtkSphereSource> spereSource = vtkSmartPointer<vtkSphereSource>::New();
	spereSource->Update();

	vtkSmartPointer<vtkSimplePointsWriter> writer = vtkSmartPointer<vtkSimplePointsWriter>::New();
	writer->SetFileName( "./TestSimplePointsWriter.xyz" );
	writer->SetInputConnection( spereSource->GetOutputPort() );
	writer->Write();

	return EXIT_SUCCESS;
}