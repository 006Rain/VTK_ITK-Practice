/*
Description:
This sample creates an isosurface of sample sphere.
*/

/*
vtkSampleFunction:
sample an implicit function over a structured point set.

vtkSampleFunction is a source object that evaluates an implicit function and normals 
at each point in a vtkStructuredPoints. The user can specify the sample dimensions an 
location in space to perform the sampling. To create closed surfaces (in conjunction 
with the vtkContourFilter), capping can be turned on to set a particular value on the 
boundaries of the sample space.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkActor.h"
#include "vtkContourFilter.h"
#include "vtkImageData.h"
#include "vtkNamedColors.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSampleFunction.h"
#include "vtkSmartPointer.h"
#include "vtkSphere.h"

#include <algorithm>
#include <Array>

int main()
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
	std::array<unsigned char, 4> bkg{ { 51, 77, 102, 255 } };
	colors->SetColor( "BkgColor", bkg.data() );

	//Implicit function
	vtkSmartPointer<vtkSphere> sphere = vtkSmartPointer<vtkSphere>::New();
	
	//Sample the function
	vtkSmartPointer<vtkSampleFunction> sample = vtkSmartPointer<vtkSampleFunction>::New();
	sample->SetSampleDimensions( 50, 50, 50 );
	sample->SetImplicitFunction( sphere );
	double value = 2.0;
	double xmin = -value, xmax = value, ymin = -value, ymax = value, zmin = -value, zmax = value;
	sample->SetModelBounds( xmin, xmax, ymin, ymax, zmin, zmax );

	//Create ¦È isosurface
	vtkSmartPointer<vtkContourFilter> contours = vtkSmartPointer<vtkContourFilter>::New();
	contours->SetInputConnection( sample->GetOutputPort() );
	contours->GenerateValues( 3, 1, 15 );

	//Map the contours to graphical primitives
	vtkSmartPointer<vtkPolyDataMapper> contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	contourMapper->SetInputConnection( contours->GetOutputPort() );
	contourMapper->ScalarVisibilityOff();
	
	vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
	contourActor->SetMapper( contourMapper );
	contourActor->GetProperty()->SetColor( 0, 50, 90 );

	//Visualize
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderer->AddActor( contourActor );
	renderer->SetBackground( colors->GetColor3d( "BkgColor" ).GetData() );

	renderWindow->Render();
	interactor->Start();

	return 0;
}