/*
Description:
Cut a surface with vtkCutter.
*/

/*
vtkCutter:

Cut vtkDataSet with user-specified implicit function.

vtkCutter is a filter to cut through data using any subclass of vtkImplicitFunction. That is, a polygonal surface is created 
corresponding to the implicit function F(x,y,z) = value(s), 
where you can specify one or more values used to cut with.

In VTK, cutting means reducing a cell of dimension N to a cut surface of dimension N-1. 
For example, a tetrahedron when cut by a plane (i.e., vtkPlane implicit function) will generate triangles. 
(In comparison, clipping takes a N dimensional cell and creates N dimension primitives.)

vtkCutter is generally used to "slice-through" a dataset, generating a surface that can be visualized. 
It is also possible to use vtkCutter to do a form of volume rendering. vtkCutter does this by generating multiple cut 
surfaces (usually planes) which are ordered (and rendered) from back-to-front. 
The surfaces are set translucent to give a volumetric rendering effect.

Note that data can be cut using either 1) the scalar values associated with the dataset or 2) an implicit function associated with this class. 
By default, if an implicit function is set it is used to clip the data set, otherwise the dataset scalars are used to perform the clipping.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkPlane.h"
#include "vtkProperty.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkNamedColors.h"

#include "vtkCutter.h"

#define CUTTER_NUM 3

int main()
{
	std::string strFile = "../Bin/ExampleFiles/cow.vtp";

	int nNumOfCuts = CUTTER_NUM;
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	//Reader
	vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
	reader->SetFileName( strFile.c_str() );
	reader->Update();

	double bounds[ 6 ];
	reader->GetOutput()->GetBounds( bounds );
	std::cout << "Bounds: " << bounds << ", " << bounds[1] << ", " << bounds[2] << ", " << bounds[3] << ", " << bounds[4] << ", " << bounds[5] << endl;

	//Cut function
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin( (bounds[ 0 ] + bounds[ 1 ])*0.5, (bounds[ 2 ] + bounds[ 3 ])*0.5, bounds[ 5 ] );
	plane->SetNormal( 1, 0, 0 );

	//Cutter
/*	double high = plane->EvaluateFunction( (bounds[ 0 ] + bounds[ 1 ])*0.5, (bounds[ 2 ] + bounds[ 3 ])*0.5, bounds[ 5 ] );*/
	vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
	cutter->SetInputConnection( reader->GetOutputPort() );
	cutter->SetCutFunction( plane );
#if 1
	cutter->GenerateValues( nNumOfCuts, 0.01, 0.99 );
#else
	cutter->SetValue( 0, 0.1 );
#endif

	vtkSmartPointer<vtkPolyDataMapper> cutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cutterMapper->SetInputConnection( cutter->GetOutputPort() );
	cutterMapper->ScalarVisibilityOff();

	//Create cut actor
	vtkSmartPointer<vtkActor> cutterActor = vtkSmartPointer<vtkActor>::New();
	cutterActor->SetMapper( cutterMapper );
	cutterActor->GetProperty()->SetLineWidth( 2 );
	cutterActor->GetProperty()->SetColor( colors->GetColor3d( "Gold" ).GetData() );

	//Create model actor
	vtkSmartPointer<vtkPolyDataMapper> modelMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	modelMapper->SetInputConnection( reader->GetOutputPort() );
	modelMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> modelActor = vtkSmartPointer<vtkActor>::New();
	modelActor->SetMapper( modelMapper );
	modelActor->GetProperty()->SetColor( colors->GetColor3d( "Gray" ).GetData() );

	//Visualization pipe line
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( cutterActor );
	renderer->AddActor( modelActor );

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	renderWindow->SetSize( 600, 600 );
	renderWindow->SetPosition( 500, 150 );

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