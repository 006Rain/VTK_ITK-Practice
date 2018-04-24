#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSphereSource.h"
#include "vtkPolyData.h"
#include "vtkPLYWriter.h"
#include "vtkPLYReader.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"

using namespace std;

int main()
{
	string strDstFile = "./TestPLYWriter.ply";

	vtkSmartPointer<vtkSphereSource> spereSource = vtkSmartPointer<vtkSphereSource>::New();
	spereSource->SetThetaResolution( 4 );// Set sphere surface number

	vtkSmartPointer<vtkPLYWriter> plyWriter = vtkSmartPointer<vtkPLYWriter>::New();
	plyWriter->SetFileName( strDstFile.c_str() );
	plyWriter->SetInputConnection( spereSource->GetOutputPort() );
	plyWriter->Write();
	plyWriter->Update();

	//read and display for verification
	vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
	reader->SetFileName( strDstFile.c_str() );
	reader->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( reader->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->SetColor( 0.6, 0.5, 0.1 );
	actor->RotateY( 30 );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( 0.3, 0.4, 0.8 );

	vtkSmartPointer<vtkRenderWindow> renerWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renerWindow->AddRenderer( renderer );
	renerWindow->SetSize( 400, 300 );
	renerWindow->SetPosition( 500, 150 );
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow( renerWindow );

	renerWindow->Render();
	renderWindowInteractor->Start();
	return EXIT_SUCCESS;
}