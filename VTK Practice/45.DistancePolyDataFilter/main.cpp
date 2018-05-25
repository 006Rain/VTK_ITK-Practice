/*
Description:
Compute the distance function from one 
vtkPolyData to another.
-----------------------------------------------------
Add: vtkMapper::SetScalarRange( double,double )
Specify range in terms of scalar minimum and maximum.
These value are used to map scalars into lookup table.
Has no effect when UseLookupTableScalarRange is true.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT( vtkRenderingFreeType );

#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkDistancePolyDataFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPolyDataReader.h"
#include "vtkCleanPolyData.h"
#include "vtkProperty.h"
#include "vtkPointData.h"
#include "vtkScalarBarActor.h"
#include "vtkSphereSource.h"
#include "vtkNamedColors.h"

int main( int argc, char* argv[] )
{
	vtkSmartPointer<vtkPolyData> input1, input2;

	if( 3 == argc )
	{
		vtkSmartPointer<vtkPolyDataReader> reader1 = vtkSmartPointer<vtkPolyDataReader>::New();
		reader1->SetFileName( argv[ 1 ] );
		reader1->Update();
		input1 = reader1->GetOutput();

		vtkSmartPointer<vtkPolyDataReader> reader2 = vtkSmartPointer<vtkPolyDataReader>::New();
		reader2->SetFileName( argv[ 2 ] );
		reader2->Update();
		input2 = reader2->GetOutput();
	}
	else
	{
		vtkSmartPointer<vtkSphereSource> sphereSource1 = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource1->SetCenter( 1.5, 0, 0 );
		sphereSource1->SetPhiResolution( 21 );
		sphereSource1->SetThetaResolution( 30 );
		sphereSource1->Update();
		input1 = sphereSource1->GetOutput();

		vtkSmartPointer<vtkSphereSource> sphereSource2 = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource2->SetPhiResolution( 30 );
		sphereSource2->SetThetaResolution( 21 );
		sphereSource2->Update();
		input2 = sphereSource2->GetOutput();
	}

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	vtkSmartPointer<vtkCleanPolyData> clean1 = vtkSmartPointer<vtkCleanPolyData>::New();
	clean1->SetInputData( input1 );

	vtkSmartPointer<vtkCleanPolyData> clean2 = vtkSmartPointer<vtkCleanPolyData>::New();
	clean2->SetInputData( input2 );

	vtkSmartPointer<vtkDistancePolyDataFilter> distanceFilter = vtkSmartPointer<vtkDistancePolyDataFilter>::New();

	distanceFilter->SetInputConnection( 0, clean1->GetOutputPort() );
	distanceFilter->SetInputConnection( 1, clean2->GetOutputPort() );
	distanceFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( distanceFilter->GetOutputPort() );
	//mapper->SetScalarRange( distanceFilter->GetOutput()->GetPointData()->GetScalars()->GetRange()[ 0 ], distanceFilter->GetOutput()->GetPointData()->GetScalars()->GetRange()[ 1 ] );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper2->SetInputConnection( distanceFilter->GetOutputPort( 1 ) );
	//mapper2->SetScalarRange( distanceFilter->GetSecondDistanceOutput()->GetPointData()->GetScalars()->GetRange()[ 0 ], distanceFilter->GetSecondDistanceOutput()->GetPointData()->GetScalars()->GetRange()[ 1 ] );

	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	actor2->SetMapper( mapper2 );

	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	scalarBar->SetLookupTable( mapper->GetLookupTable() );
	scalarBar->SetTitle( "Distance" );
	scalarBar->SetNumberOfLabels( 5 );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground( colors->GetColor3d( "Silver" ).GetData() );
	renderer->SetBackground2( colors->GetColor3d( "Gold" ).GetData() );
	renderer->GradientBackgroundOn();

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetPosition( 600, 200 );
	renderWindow->SetSize( 800, 600 );

	vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renWinInteractor->SetRenderWindow( renderWindow );

	renderer->AddActor( actor );
	renderer->AddActor( actor2 );
	renderer->AddActor2D( scalarBar );

	renderWindow->Render();
	renWinInteractor->Start();
	return EXIT_SUCCESS;
}