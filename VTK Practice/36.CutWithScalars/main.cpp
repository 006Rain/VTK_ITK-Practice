/*
Description:
This example cuts a surface model of the skin with a series 
of planes produces contour lines.
Lines are wrapped with tubes for clarify.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkContourFilter.h"
#include "vtkDoubleArray.h"
#include "vtkNamedColors.h"
#include "vtkPlane.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"

#define CUT_NUMBER 16

int main()
{
	std::string strFile = "../Bin/ExampleFiles/cow.vtp";

	int nNumOfCuts = CUT_NUMBER;

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	//Reader
	vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
	reader->SetFileName( strFile.c_str() );
	reader->Update();

	double dBounds[ 6 ] = { 0 };
	reader->GetOutput()->GetBounds( dBounds );

	//Create function
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin( (dBounds[ 0 ] + dBounds[ 1 ])*0.5, (dBounds[ 2 ] + dBounds[ 3 ])*0.5, dBounds[ 5 ] );
	plane->SetNormal( 1, 0, 0 );

	//Create Scalars
	vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
	int nNumOfPoints = reader->GetOutput()->GetNumberOfPoints();
	scalars->SetNumberOfTuples( nNumOfPoints );
	vtkPoints *pPoints = reader->GetOutput()->GetPoints();
	for( int n = 0; n < nNumOfPoints; n++ )
	{
		double dPoint[ 3 ];
		pPoints->GetPoint( n, dPoint );
		scalars->SetTuple1( n, plane->EvaluateFunction( dPoint ) );
	}
	reader->GetOutput()->GetPointData()->SetScalars( scalars );

	double dScalarRange[ 2 ];
	reader->GetOutput()->GetPointData()->GetScalars()->GetRange( dScalarRange );

	//Create cutter
	vtkSmartPointer<vtkContourFilter> cutter = vtkSmartPointer<vtkContourFilter>::New();
	cutter->SetInputConnection( reader->GetOutputPort() );
	cutter->ComputeScalarsOff();
	cutter->ComputeNormalsOff();
	cutter->GenerateValues( nNumOfCuts, 0.99*dScalarRange[ 0 ], 0.99*dScalarRange[ 1 ] );

	vtkSmartPointer<vtkPolyDataMapper> cutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cutterMapper->SetInputConnection( cutter->GetOutputPort() );
	cutterMapper->ScalarVisibilityOff();

	//Create cut actor
	vtkSmartPointer<vtkActor> cutterActor = vtkSmartPointer<vtkActor>::New();
	cutterActor->GetProperty()->SetColor( colors->GetColor3d( "Banana" ).GetData() );
	cutterActor->GetProperty()->SetLineWidth( 5 );
	cutterActor->SetMapper( cutterMapper );

	//Create model actor
	vtkSmartPointer<vtkPolyDataMapper> modelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	modelMapper->SetInputConnection( reader->GetOutputPort() );
	modelMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> modelActor = vtkSmartPointer<vtkActor>::New();
	modelActor->GetProperty()->SetColor( colors->GetColor3d( "Flesh" ).GetData() );
	modelActor->SetMapper( modelMapper );

	//Create render pipe line
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( cutterActor );
	renderer->AddActor( modelActor );

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize( 600, 600 );
	renderWindow->SetPosition( 500, 120 );

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderer->SetBackground( colors->GetColor3d( "Burlywood" ).GetData() );
	renderer->GetActiveCamera()->SetPosition( 0, -1, 0 );
	renderer->GetActiveCamera()->SetFocalPoint( 0, 0, 0 );
	renderer->GetActiveCamera()->SetViewUp( 0, 0, 1 );
	renderer->GetActiveCamera()->Azimuth( 30 );
	renderer->GetActiveCamera()->Elevation( 30 );

	renderer->ResetCamera();
	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}