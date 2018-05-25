/*
Description:
Perform boolean operations on two vtkPolyData objects.

The vtkBoolOperationPolyDataFilter works best with "clean" data,
so this example first runs vtkTriangleFilter and then vtkCleanPolyData.

This example can be run in three ways:
1. BooleanOperationPolyDataFilter - Compute the intersection of two spheres.

2.BooleanOperationPolyDataFilter - intersection | difference | union - Computes
	the intersection(difference or union) of two spheres.

3.BooleanOperationPolyDataFilter input1.vtk intersection|difference|union 
	input2.vtk - Computes the intersection(union or difference) of 
	two vtkPolyDatas.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkBooleanOperationPolyDataFilter.h"

#include "vtkCleanPolyData.h"
#include "vtkTriangleFilter.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkNamedColors.h"

#include "vtkBYUReader.h"
#include "vtkOBJReader.h"
#include "vtkPLYReader.h"
#include "vtkPolyDataReader.h"
#include "vtkSTLReader.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkSphereSource.h"
#include "vtksys/SystemTools.hxx"

#include "vtkCamera.h"

//#define BOOL_OPERATION "union"
#define BOOL_OPERATION "intersection"
//#define BOOL_OPERATION "difference"

namespace
{
	vtkSmartPointer<vtkPolyData> ReadPolyData( const char* fileName );
	void PositionCamera( vtkSmartPointer<vtkRenderer>& renderer, double* viewUp, double* position );
}

int main( int argc, char*argv[] )
{
	vtkSmartPointer<vtkPolyData> input1;
	vtkSmartPointer<vtkPolyData> input2;

	std::string strOperation( BOOL_OPERATION );

	if( 4 == argc )
	{
		vtkSmartPointer<vtkPolyData> poly1 = ReadPolyData( argv[ 1 ] );
		vtkSmartPointer<vtkTriangleFilter> triangle1 = vtkSmartPointer<vtkTriangleFilter>::New();
		triangle1->SetInputData( poly1 );
		triangle1->Update();
		vtkSmartPointer<vtkCleanPolyData> clean1 = vtkSmartPointer<vtkCleanPolyData>::New();
		clean1->SetInputConnection( triangle1->GetOutputPort() );
		clean1->Update();
		input1 = clean1->GetOutput();

		vtkSmartPointer<vtkPolyData> poly2 = ReadPolyData( argv[ 3 ] );
		vtkSmartPointer<vtkTriangleFilter> triangle2 = vtkSmartPointer<vtkTriangleFilter>::New();
		triangle2->SetInputData( poly2 );
		triangle2->Update();
		vtkSmartPointer<vtkCleanPolyData> clean2 = vtkSmartPointer<vtkCleanPolyData>::New();
		clean2->SetInputConnection( triangle2->GetOutputPort() );
		clean2->Update();
		input2 = clean2->GetOutput();
		strOperation = argv[ 2 ];
	}
	else
	{
		vtkSmartPointer<vtkSphereSource> sphereSource1 = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource1->SetCenter( 0.25, 0, 0 );
		sphereSource1->SetPhiResolution( 21 );
		sphereSource1->SetThetaResolution( 21 );
		sphereSource1->Update();
		input1 = sphereSource1->GetOutput();

		vtkSmartPointer<vtkSphereSource> sphereSource2 = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource2->Update();
		input2 = sphereSource2->GetOutput();

		if( 2 == argc )
		{
			strOperation = argv[ 1 ];
		}
	}

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	vtkSmartPointer<vtkPolyDataMapper> inputMapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	inputMapper1->SetInputData( input1 );
	inputMapper1->ScalarVisibilityOff();
	vtkSmartPointer<vtkActor> inputActor1 = vtkSmartPointer<vtkActor>::New();
	inputActor1->SetMapper( inputMapper1 );
	inputActor1->GetProperty()->SetDiffuseColor( colors->GetColor3d( "Tomato" ).GetData() );
	inputActor1->GetProperty()->SetSpecular( 0.6 );
	inputActor1->GetProperty()->SetSpecularPower( 20 );
	inputActor1->SetPosition( input1->GetBounds()[ 1 ] - input1->GetBounds()[ 0 ], 0, 0 );

	vtkSmartPointer<vtkPolyDataMapper> inputMapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	inputMapper2->SetInputData( input2 );
	inputMapper2->ScalarVisibilityOff();
	vtkSmartPointer<vtkActor> inputActor2 = vtkSmartPointer<vtkActor>::New();
	inputActor2->GetProperty()->SetDiffuseColor( colors->GetColor3d( "Mint" ).GetData() );
	inputActor2->GetProperty()->SetSpecular( 0.6 );
	inputActor2->GetProperty()->SetSpecularPower( 20 );
	inputActor2->SetPosition( -(input1->GetBounds()[ 1 ] - input1->GetBounds()[ 0 ]), 0, 0 );


	vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOpration = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
	if( "union" == strOperation )
	{
		booleanOpration->SetOperationToUnion();
	}
	else if( "intersection" == strOperation )
	{
		booleanOpration->SetOperationToIntersection();
	}
	else if( "difference" == strOperation )
	{
		booleanOpration->SetOperationToDifference();
	}
	else
	{
		std::cout << "Unknown operation: " << strOperation << std::endl;
		system( "pause" );
		return EXIT_FAILURE;
	}
	booleanOpration->SetInputData( 0, input1 );
	booleanOpration->SetInputData( 1, input2 );

	vtkSmartPointer<vtkPolyDataMapper> booleanOprationMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	booleanOprationMapper->SetInputConnection( booleanOpration->GetOutputPort() );
	booleanOprationMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> booleanOprationActor = vtkSmartPointer<vtkActor>::New();
	booleanOprationActor->SetMapper( booleanOprationMapper );
	booleanOprationActor->GetProperty()->SetDiffuseColor( colors->GetColor3d( "Banana" ).GetData() );
	booleanOprationActor->GetProperty()->SetSpecular( 0.6 );
	booleanOprationActor->GetProperty()->SetSpecularPower( 20 );

	//Visualize
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddViewProp( inputActor1 );
	renderer->AddViewProp( inputActor2 );
	renderer->AddViewProp( booleanOprationActor );
	renderer->SetBackground( colors->GetColor3d( "Silver" ).GetData() );

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize( 640, 480 );
	double viewUp[ 3 ] = { 0.0, 0.0, 1.0 };
	double position[ 3 ] = { 0.0, -1.0, 0.0 };

	PositionCamera( renderer, viewUp, position );
	renderer->GetActiveCamera()->Dolly( 1.5 );
	renderer->ResetCameraClippingRange();

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderWindow->Render();
	interactor->Start();
	return EXIT_SUCCESS;
}

namespace
{
	vtkSmartPointer<vtkPolyData> ReadPolyData( const char* fileName )
	{
		vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
		std::string strExtension = vtksys::SystemTools::GetFilenameExtension( fileName );
		if( ".ply" == strExtension )
		{
			vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
			reader->SetFileName( fileName );
			reader->Update();
			polyData = reader->GetOutput();
		}
		else if( ".vtp" == strExtension )
		{
			vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
			reader->SetFileName( fileName );
			reader->Update();
			polyData = reader->GetOutput();
		}
		else if( ".obj" == strExtension )
		{
			vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
			reader->SetFileName( fileName );
			reader->Update();
			polyData = reader->GetOutput();
		}
		else if( ".stl" == strExtension )
		{
			vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
			reader->SetFileName( fileName );
			reader->Update();
			polyData = reader->GetOutput();
		}
		else if( ".vtk" == strExtension )
		{
			vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
			reader->SetFileName( fileName );
			reader->Update();
			polyData = reader->GetOutput();
		}
		else if( ".g" == strExtension )
		{
			vtkSmartPointer<vtkBYUReader> reader = vtkSmartPointer<vtkBYUReader>::New();
			reader->SetFileName( fileName );
			reader->Update();
			polyData = reader->GetOutput();
		}
		else
		{
			vtkSmartPointer<vtkSphereSource> source = vtkSmartPointer<vtkSphereSource>::New();
			source->Update();
			polyData = source->GetOutput();
		}

		return polyData;
	}

	void PositionCamera( vtkSmartPointer<vtkRenderer>& renderer, double* viewUp, double* position )
	{
		renderer->GetActiveCamera()->SetFocalPoint( 0.0, 0.0, 0.0 );
		renderer->GetActiveCamera()->SetViewUp( viewUp );
		renderer->GetActiveCamera()->SetPosition( position );
		renderer->ResetCamera();
		return;
	}
}