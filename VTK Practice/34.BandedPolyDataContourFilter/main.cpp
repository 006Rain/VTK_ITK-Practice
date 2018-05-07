/*
Description:
Using vtkBandedPolyDataContourFilter
to split values by filling the different 
colors.
*/

/*
vtkBandedPolyDataContourFilter :
is a filter that takes as input vtkPolyData and produces as output filled contours 
(also represented as vtkPolyData). Filled contours are bands of cells that all have 
the same cell scalar value, and can therefore be colored the same. The method is also 
referred to as filled contour generation.

To use this filter you must specify one or more contour values. You can either use the
method SetValue() to specify each contour value, or use GenerateValues() to generate a
series of evenly spaced contours. Each contour value divides (or clips) the data into 
two pieces, values below the contour value, and values above it. The scalar values of 
each band correspond to the specified contour value. Note that if the first and last 
contour values are not the minimum/maximum contour range, then two extra contour 
values are added corresponding to the minimum and maximum range values. These extra 
contour bands can be prevented from being output by turning clipping on.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkBandedPolyDataContourFilter.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkNamedColors.h"

int main()
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertPoint( 0, 0, 0, 0 );
	points->InsertPoint( 1, 0, 1, 0 );
	points->InsertPoint( 2, 0, 2, 0 );
	points->InsertPoint( 3, 1, 0, 0 );
	points->InsertPoint( 4, 1, 1, 0 );
	points->InsertPoint( 5, 1, 2, 0 );
	points->InsertPoint( 6, 2, 0, 0 );
	points->InsertPoint( 7, 2, 2, 0 );
	points->InsertPoint( 8, 3, 0, 0 );
	points->InsertPoint( 9, 3, 1, 0 );
	points->InsertPoint( 10, 3, 2, 0 );
	points->InsertPoint( 11, 4, 0, 0 );
	points->InsertPoint( 12, 6, 0, 0 );
	points->InsertPoint( 13, 5, 2, 0 );
	points->InsertPoint( 14, 7, 0, 0 );
	points->InsertPoint( 15, 9, 0, 0 );
	points->InsertPoint( 16, 7, 2, 0 );
	points->InsertPoint( 17, 9, 2, 0 );
	points->InsertPoint( 18, 10, 0, 0 );
	points->InsertPoint( 19, 12, 0, 0 );
	points->InsertPoint( 20, 10, 1, 0 );
	points->InsertPoint( 21, 12, 1, 0 );
	points->InsertPoint( 22, 10, 2, 0 );
	points->InsertPoint( 23, 12, 2, 0 );
	points->InsertPoint( 24, 10, 3, 0 );
	points->InsertPoint( 25, 12, 3, 0 );

	vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
	polys->InsertNextCell( 4 );
	polys->InsertCellPoint( 14 );
	polys->InsertCellPoint( 15 );
	polys->InsertCellPoint( 17 );
	polys->InsertCellPoint( 16 );
	polys->InsertNextCell( 3 );
	polys->InsertCellPoint( 11 );
	polys->InsertCellPoint( 12 );
	polys->InsertCellPoint( 13 );

	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
	scalars->SetNumberOfTuples( 26 );
	scalars->SetTuple1( 0, 0 );
	scalars->SetTuple1( 1, 50 );
	scalars->SetTuple1( 2, 100 );
	scalars->SetTuple1( 3, 0 );
	scalars->SetTuple1( 4, 60 );
	scalars->SetTuple1( 5, 100 );
	scalars->SetTuple1( 6, 10 );
	scalars->SetTuple1( 7, 90 );
	scalars->SetTuple1( 8, 10 );
	scalars->SetTuple1( 9, 50 );
	scalars->SetTuple1( 10, 90 );
	scalars->SetTuple1( 11, 10 );
	scalars->SetTuple1( 12, 40 );
	scalars->SetTuple1( 13, 100 );
	scalars->SetTuple1( 14, 0 );
	scalars->SetTuple1( 15, 60 );
	scalars->SetTuple1( 16, 40 );
	scalars->SetTuple1( 17, 100 );
	scalars->SetTuple1( 18, 0 );
	scalars->SetTuple1( 19, 25 );
	scalars->SetTuple1( 20, 40 );
	scalars->SetTuple1( 21, 50 );
	scalars->SetTuple1( 22, 50 );
	scalars->SetTuple1( 23, 75 );
	scalars->SetTuple1( 24, 75 );
	scalars->SetTuple1( 25, 100 );

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints( points );
	polyData->SetPolys( polys );
	polyData->GetPointData()->SetScalars( scalars );

	vtkSmartPointer<vtkBandedPolyDataContourFilter> bandedFilter = vtkSmartPointer<vtkBandedPolyDataContourFilter>::New();
	bandedFilter->SetInputData( polyData );
	bandedFilter->GenerateValues( 5, 0, 50 );
#if 0
	bandedFilter->ClippingOn();
#endif

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( bandedFilter->GetOutputPort() );
	mapper->SetScalarModeToUseCellData();
	mapper->SetScalarRange( 0, 4 );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	//Create the pipe line
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderer->AddActor( actor );

	renderWindow->SetSize( 600, 400 );
	renderWindow->SetPosition( 500, 100 );
	renderWindow->Render();
	renderer->SetBackground( colors->GetColor3d( "Melon" ).GetData() );
	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}