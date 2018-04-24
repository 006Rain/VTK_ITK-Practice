/*
Description:
This example reads a DICOM file and display it
on the screen
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkImageViewer2.h"
#include "vtkDICOMImageReader.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

using namespace std;

int main()
{
	string strDicomDir = "../Bin/ExampleFiles/prostate.dcm";

	//Read all the DICOM files in the specified directory
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName( strDicomDir.c_str() );
	reader->Update();

	//Visualize
	vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imageViewer->SetInputConnection( reader->GetOutputPort() );
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor( renderWindowInteractor );
	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();

	renderWindowInteractor->Start();
	return EXIT_SUCCESS;
}