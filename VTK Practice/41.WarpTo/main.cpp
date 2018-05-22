/*
Description:
Bend an Object.
*/

/*
vtkTubeFilter:
filter that generates tubes around lines.

vtkTubeFilter is a filter that generates a tube around each input line. The 
tubes are made up of triangle strips and rotate around the tube with the 
rotation of the line normals. (If no normals are present, they are computed 
automatically.) The radius of the tube can be set to vary with scalar or 
vector value. If the radius varies with scalar value the radius is linearly 
adjusted. If the radius varies with vector value, a mass flux preserving 
variation is used. The number of sides for the tube also can be specified. 
You can also specify which of the sides are visible. This is useful for 
generating interesting striping effects. Other options include the ability t 
cap the tube and generate texture coordinates. Texture coordinates can be 
used with an associated texture map to create interesting effects such as 
marking the tube with stripes corresponding to length or time.

This filter is typically used to create thick or dramatic lines. Another 
common use is to combine this filter with vtkStreamTracer to generate 
streamtubes.

Warning:
The number of tube sides must be greater than 3. If you wish to use fewer 
sides (i.e., a ribbon), use vtkRibbonFilter.The input line must not have 
duplicate points, or normals at points that are parallel to the incoming/
outgoing line segments. (Duplicate points can be removed with 
vtkCleanPolyData.) If a line does not meet this criteria, then that line is 
not tubed.
*/

/*
vtkWarpTo:
deform geometry by warping towards a point.

vtkWarpTo is a filter that modifies point coordinates by moving the points 
towards a user specified position.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkLineSource.h"
#include "vtkTubeFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkWarpTo.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"

int main()
{
	//Create a line
	vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
	lineSource->SetPoint1( 0.0, 0.0, 0.0 );
	lineSource->SetPoint2( 0.0, 1.0, 0.0 );
	lineSource->SetResolution( 20 );
	lineSource->Update();

	//line actor
	vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	lineMapper->SetInputConnection( lineSource->GetOutputPort() );
	lineMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
	lineActor->SetMapper( lineMapper );
	lineActor->GetProperty()->SetColor( 100, 255, 0 );

	//Create a tube (cylinder) around the line
	vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
	tubeFilter->SetInputConnection( lineSource->GetOutputPort() );
	tubeFilter->SetRadius( 0.001 );//default is 0.5
	tubeFilter->SetNumberOfSides( 50 );
	tubeFilter->Update();

	vtkSmartPointer<vtkWarpTo> warpTo = vtkSmartPointer<vtkWarpTo>::New();
	warpTo->SetInputConnection( tubeFilter->GetOutputPort() );
	warpTo->SetPosition( 10, 1, 0 );
	warpTo->SetScaleFactor( 10 );
	warpTo->AbsoluteOn();

	//WarpTo actor
	vtkSmartPointer<vtkDataSetMapper> warpMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	warpMapper->SetInputConnection( warpTo->GetOutputPort() );
	warpMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> warpActor = vtkSmartPointer<vtkActor>::New();
	warpActor->SetMapper( warpMapper );
	warpActor->GetProperty()->SetColor( 200, 0, 100 );
	warpActor->SetPosition( 0.5, 0, 0 );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetPosition( 500, 200 );
	renderWindow->SetSize( 600, 400 );
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );
	
	renderer->AddActor( lineActor );
	renderer->AddActor( warpActor );
	
	interactor->Initialize(); 
	interactor->Start();

	return EXIT_SUCCESS;
}