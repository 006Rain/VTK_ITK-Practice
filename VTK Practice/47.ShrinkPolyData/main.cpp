/*
Description:
Move all items in a PolyData towards their centroid.
-------------
Add:vtkShrinkPolyData
shrink cells composing PolyData.

vtkShrinkPolyData shrinks cells composing a polygonal dataset
(e.g., vertexes, lines, polygons, and triangle strips) towards their centroid. 
The centroid of a cell is computed as the average position of the cell points. 
Shrinking results in disconnecting the cells from one another. 
The output dataset type of this filter is polygonal data.

During execution the filter passes its input cell data to its output. 
Point data attributes are copied to the points created during the shrinking process.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkShrinkPolyData.h"
#include "vtkSphereSource.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRTXMLPolyDataReader.h"
#include "vtkNamedColors.h"

int main()
{
	vtkSmartPointer<vtkPolyData> inputPolyData; 
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetPhiResolution( 15 );
	sphereSource->SetThetaResolution( 25 );
	sphereSource->Update();
	inputPolyData = sphereSource->GetOutput();

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	//Shrink cells to compose PolyData
	vtkSmartPointer<vtkShrinkPolyData> shrink = vtkSmartPointer<vtkShrinkPolyData>::New();
	shrink->SetShrinkFactor( 0.45 );
	shrink->SetInputData( inputPolyData );

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( shrink->GetOutputPort() );
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkProperty> backProperty = vtkSmartPointer<vtkProperty>::New();
	backProperty->SetColor( colors->GetColor3d( "Peacock" ).GetData() );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->SetColor( colors->GetColor3d( "Salmon" ).GetData() );
	actor->SetBackfaceProperty( backProperty );

	//Visualize
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize( 600, 600 );
	renderWindow->SetPosition( 600, 200 );

	vtkSmartPointer<vtkRenderWindowInteractor> rendWinInteraction = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rendWinInteraction->SetRenderWindow( renderWindow );

	renderer->AddActor( actor );
	renderer->SetBackground( colors->GetColor3d( "Burlywood" ).GetData() );
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth( 30 );
	renderer->GetActiveCamera()->Elevation( 30 );
	renderer->GetActiveCamera()->Dolly( 1.5 );
	renderer->ResetCameraClippingRange();

	renderWindow->Render();
	rendWinInteraction->Start();
	return EXIT_SUCCESS;
}