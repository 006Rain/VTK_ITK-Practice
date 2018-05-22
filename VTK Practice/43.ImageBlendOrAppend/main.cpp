/*
Description:
Test vtkImageAppend or vtkImageBlend.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT( vtkRenderingOpenGL2 );

#include "vtkSmartPointer.h"
#include "vtkJPEGReader.h"
#include "vtkImageAppend.h"
#include "vtkImageBlend.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"

#define Bool_Append 1

int main()
{
	//Files
	std::string strFile1 = "../Bin/ExampleFiles/apple.jpg";
	std::string strFile2 = "../Bin/ExampleFiles/desk.jpg";

	//Reader
	vtkSmartPointer<vtkJPEGReader> reader1 = vtkSmartPointer<vtkJPEGReader>::New();
	reader1->SetFileName( strFile1.c_str() );
	reader1->Update();

	vtkSmartPointer<vtkJPEGReader> reader2 = vtkSmartPointer<vtkJPEGReader>::New();
	reader2->SetFileName( strFile2.c_str() );
	reader2->Update();

	//Append or Blend
#if Bool_Append
	vtkSmartPointer<vtkImageAppend> imgAppend = vtkSmartPointer<vtkImageAppend>::New();
	imgAppend->AddInputConnection( reader1->GetOutputPort() );
	imgAppend->AddInputConnection( reader2->GetOutputPort() );
#else
	vtkSmartPointer<vtkImageBlend> imgBlend = vtkSmartPointer<vtkImageBlend>::New();
	imgBlend->AddInputConnection( reader1->GetOutputPort() );
	imgBlend->AddInputConnection( reader2->GetOutputPort() );
	imgBlend->SetOpacity( 0, 0.4 );
	imgBlend->SetOpacity( 1, 0.6 );
#endif

	//Visualize
	vtkSmartPointer<vtkImageViewer2> imgViewer2 = vtkSmartPointer<vtkImageViewer2>::New();
#if Bool_Append	
	imgViewer2->SetInputConnection( imgAppend->GetOutputPort() );
#else
	imgViewer2->SetInputConnection( imgBlend->GetOutputPort() );
#endif

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imgViewer2->SetupInteractor( interactor );
	imgViewer2->SetSize( 600, 600 );
	imgViewer2->Render();
	imgViewer2->GetRenderer()->ResetCamera();
	imgViewer2->Render();

	interactor->Start();
	return EXIT_SUCCESS;
}