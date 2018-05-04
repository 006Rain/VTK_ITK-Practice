/*
Description:
This examples show how to position an AxesActor in 3D.
Notice that position and orientation of the AxesActor
is done with a user transform.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT( vtkRenderingFreeType );

#include "vtkActor.h"
#include "vtkAxesActor.h"
#include "vtkCamera.h"
#include "vtkCaptionActor2D.h"
#include "vtkNamedColors.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextProperty.h"
#include "vtkTransform.h"

int main()
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter( 0, 0, 0 );
	sphereSource->SetRadius( 0.5 );

	//Create a mapper
	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection( sphereSource->GetOutputPort() );

	//Create an actor
	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->SetMapper( sphereMapper );

	//a renderer and render window
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWin = vtkSmartPointer<vtkRenderWindow>::New();
	renderWin->SetWindowName( "Axes" );
	renderWin->AddRenderer( renderer );
	renderWin->SetSize( 800, 600 );
	renderWin->SetPosition( 500, 100 );

	//an interactor
	vtkSmartPointer<vtkRenderWindowInteractor> rendWinInter = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rendWinInter->SetRenderWindow( renderWin );

	//add the actors to the scene
	renderer->AddActor( sphereActor );
	renderer->SetBackground( colors->GetColor3d( "SlateGray" ).GetData() );

	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->Translate( 0.1, 0.5, 0.0 );
	transform->Scale( 1.1, 1.3, 1.2 );

	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

	//The axes are positioned with a user transform
	axes->SetUserTransform( transform );

	//Properties of the axes labels can be set as follows
	//this set the x axis label to red
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor( colors->GetColor3d( "Yellow" ).GetData() );

	//the actual text of the axis label can be changed:
	axes->SetXAxisLabelText( "X haha haha" );
	axes->SetYAxisLabelText( "Y Heha Heha" );

	renderer->AddActor( axes );

	renderer->GetActiveCamera()->Azimuth( 50 );
	renderer->GetActiveCamera()->Elevation( -30 );

	renderer->ResetCamera();
	renderWin->Render();

	//begin mouse interaction
	rendWinInter->Start();

	return EXIT_SUCCESS;
}