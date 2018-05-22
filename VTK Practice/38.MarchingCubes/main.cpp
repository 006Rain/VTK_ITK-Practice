/*
Description:
Create a surface from a volume using Marching Cubes.
*/

/*
vtkMarchingCubes:
generate isosurface(s) from volume.

vtkMarchingCubes is a filter that takes as input a volume (e.g., 3D structured point set) and 
generates on output one or more isosurfaces. One or more contour values must be specified to 
generate the isosurfaces. Alternatively, you can specify a min/max scalar range and the number 
of contours to generate a series of evenly spaced contour values.

Warning:
This filter is specialized to volumes. If you are interested in contouring other types of data, 
use the general vtkContourFilter. If you want to contour an image (i.e., a volume slice), use 
vtkMarchingSquares.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkMarchingCubes.h"
#include "vtkVoxelModeller.h"
#include "vtkSphereSource.h"
#include "vtkImageData.h"
#include "vtkDICOMImageReader.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"

int main( int argc, char* argv[] )
{
	vtkSmartPointer<vtkImageData> volume = vtkSmartPointer<vtkImageData>::New();
	double isoValue;
	if( argc < 3 )
	{
		vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
		sphereSource->SetPhiResolution( 20 );
		sphereSource->SetThetaResolution( 20 );
		sphereSource->Update();

		double dBounds[ 6 ];
		sphereSource->GetOutput()->GetBounds( dBounds );
		for( int n = 0; n < 6; n+=2 )
		{
			double range = dBounds[ n + 1 ] - dBounds[ n ];
			dBounds[n] = dBounds[ n ] - 0.1*range;
			dBounds[ n + 1 ] = dBounds[ n + 1 ] + 0.1*range;
		}
		vtkSmartPointer<vtkVoxelModeller> voxelModeller = vtkSmartPointer<vtkVoxelModeller>::New();
		voxelModeller->SetSampleDimensions( 50, 50, 50 );
		voxelModeller->SetModelBounds( dBounds );
		voxelModeller->SetScalarTypeToFloat();
		voxelModeller->SetMaximumDistance( 0.1 );

		voxelModeller->SetInputConnection( sphereSource->GetOutputPort() );
		voxelModeller->Update();
		isoValue = 0.5;
		volume->DeepCopy( voxelModeller->GetOutput() );
	}
	else
	{
		vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
		reader->SetDirectoryName( argv[ 1 ] );
		reader->Update();
		volume->DeepCopy( reader->GetOutput() );
		isoValue = atof( argv[ 2 ] );
	}
	
	vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
	surface->SetInputData( volume );
	surface->ComputeNormalsOn();
	//surface->SetValue( 0, isoValue );
	surface->GenerateValues( 5, 0, 0.9 );

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( surface->GetOutputPort() );
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground( 0.1, 0.2, 0.3 );

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderer->AddActor( actor );
	renderWindow->Render(); 
	interactor->Start();

	return 0;
}