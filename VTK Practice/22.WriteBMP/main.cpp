/*
Description:
This example demonstrate how to write data to an *.BMP file
and read the *.BMP file.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkImageData.h"
#include "vtkBMPWriter.h"
#include "vtkSmartPointer.h"
#include "vtkImageMapper3D.h"
#include "vtkImageCanvasSource2D.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkImageActor.h"
#include "vtkBMPReader.h"
#include "vtkInteractorStyleImage.h"

using namespace std;

int main()
{
	string strDstFile = "../Bin/ExampleFiles/BMPWriterTest.bmp";

	vtkSmartPointer<vtkImageCanvasSource2D> imageSource = vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource->SetScalarTypeToUnsignedChar();
	imageSource->SetExtent( 0, 9, 0, 9, 0, 0 );
	imageSource->SetNumberOfScalarComponents( 3 );
	imageSource->SetDrawColor( 0, 0, 0, 0 );
	imageSource->FillBox( 0, 9, 0, 9 );
	imageSource->SetDrawColor( 255, 0, 0 );
	imageSource->FillBox( 5, 7, 5, 7 );
	imageSource->Update();

	vtkSmartPointer<vtkBMPWriter> bmpWriter = vtkSmartPointer<vtkBMPWriter>::New();
	bmpWriter->SetFileName( strDstFile.c_str() );
	bmpWriter->SetInputConnection( imageSource->GetOutputPort() );
	bmpWriter->Write();
	bmpWriter->Update();

	//read and display for verification
	vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
	reader->SetFileName( strDstFile.c_str() );
	reader->Update();

	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->GetMapper()->SetInputConnection( reader->GetOutputPort() );

	//setup renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->ResetCamera();

	//setup render window
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> styleImage = vtkSmartPointer<vtkInteractorStyleImage>::New();
	renderWindowInteractor->SetInteractorStyle( styleImage );

	//Render and start interaction
	renderWindowInteractor->SetRenderWindow( renderWindow );
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();




}