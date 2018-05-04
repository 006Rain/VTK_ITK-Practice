/*
Description:
The polyLine is a composite one-dimensional cell
consisting of one or more connected lines. The
polyLine is defined by an ordered list of n+1
points, where n is the number of the lines in the
polyLine. Each pair of points(i, i+1) defines a line.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDoubleArray.h"
#include "vtkNamedColors.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyLine.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkVersion.h"

int main()
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	//Create five points
	double origin[ 3 ] = { 0.0, 0.0, 0.0 };
	double p0[ 3 ] = { 1.0, 0.0, 0.0 };
	double p1[ 3 ] = { 0.0, 1.0, 0.0 };
	double p2[ 3 ] = { 0.0, 1.0, 2.0 };
	double p3[ 3 ] = { 1.0, 2.0, 3.0 };

	//Create a vtkPoints object and store the points in it
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint( origin );
	points->InsertNextPoint( p0 );
	points->InsertNextPoint( p1 );
	points->InsertNextPoint( p2 );
	points->InsertNextPoint( p3 );

	vtkSmartPointer<vtkPolyLine> polyLine = vtkSmartPointer<vtkPolyLine>::New();
	polyLine->GetPointIds()->SetNumberOfIds( 5 );
	for( int n = 0; n < 5; n++ )
	{
		polyLine->GetPointIds()->SetId( n, n );
	}

	//Create a cell array to store the lines in and add the lines to it
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	cells->InsertNextCell( polyLine );

	//Create a polydata to store everything in
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

	//Add the points to dataset
	polyData->SetPoints( points );

	//Add the lines to the dataset
	polyData->SetLines( cells );

	//pipe line
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData( polyData );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->SetColor( 255, 0, 230 );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );

	vtkSmartPointer<vtkRenderWindow> rendWindow = vtkSmartPointer<vtkRenderWindow>::New();
	rendWindow->AddRenderer( renderer );

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow( rendWindow );
	
	rendWindow->Render();
	renderWindowInteractor->Start();

	return 0;
}