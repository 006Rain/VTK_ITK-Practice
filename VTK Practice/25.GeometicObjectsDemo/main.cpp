/*
Description:
	A demonstration of all geometric objects that work 
	the same way.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT( vtkRenderingFreeType );

#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkArrowSource.h"
#include "vtkConeSource.h"
#include "vtkCubeSource.h"
#include "vtkCylinderSource.h"
#include "vtkDiskSource.h"
#include "vtkLineSource.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRegularPolygonSource.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkCamera.h"

#include <array>
#include <vector>

int main()
{
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	//Set the background color
	std::array<unsigned char, 4> bkg{ { 51, 77, 102, 255 } };
	colors->SetColor( "BkgColor", bkg.data() );

	//Create container to hold the 3D object generators(sources)
	std::vector<vtkSmartPointer<vtkPolyDataAlgorithm>> geometricObjectSources;

	//Populate the container with the various object sources to be demonstrated
	geometricObjectSources.push_back( vtkSmartPointer<vtkArrowSource>::New() );
	geometricObjectSources.push_back( vtkSmartPointer<vtkConeSource>::New() );
	geometricObjectSources.push_back( vtkSmartPointer<vtkCubeSource>::New() );
	geometricObjectSources.push_back( vtkSmartPointer<vtkCylinderSource>::New() );
	geometricObjectSources.push_back( vtkSmartPointer<vtkDiskSource>::New() );
	geometricObjectSources.push_back( vtkSmartPointer<vtkLineSource>::New() );
	geometricObjectSources.push_back( vtkSmartPointer<vtkRegularPolygonSource>::New() );
	geometricObjectSources.push_back( vtkSmartPointer<vtkSphereSource>::New() );

	//Create containers for the remaining nodes of each pipeline
	std::vector<vtkSmartPointer<vtkPolyDataMapper>> mappers;
	std::vector<vtkSmartPointer<vtkActor>> actors;
	std::vector<vtkSmartPointer<vtkTextMapper>> textMappers;
	std::vector<vtkSmartPointer<vtkActor2D>> textActors;

	//Create a common text property
	vtkSmartPointer<vtkTextProperty> textProperty = vtkSmartPointer<vtkTextProperty>::New();
	textProperty->SetFontSize( 16 );
	textProperty->SetJustificationToCentered();

	//Create a mapper and actor for each object and the corresponding
	//text label
	for( unsigned int n = 0; n < geometricObjectSources.size(); n++ )
	{
		geometricObjectSources[ n ]->Update();

		mappers.push_back( vtkSmartPointer<vtkPolyDataMapper>::New() );
		mappers[ n ]->SetInputConnection( geometricObjectSources[ n ]->GetOutputPort() );

		actors.push_back( vtkSmartPointer<vtkActor>::New() );
		actors[ n ]->SetMapper( mappers[ n ] );
		actors[ n ]->GetProperty()->SetColor( colors->GetColor3d( "Seashell" ).GetData() );

		textMappers.push_back( vtkSmartPointer<vtkTextMapper>::New() );
		textMappers[ n ]->SetInput( geometricObjectSources[ n ]->GetClassName() );//Set text label to the name of the object source
		textMappers[ n ]->SetTextProperty( textProperty );

		textActors.push_back( vtkSmartPointer<vtkActor2D>::New() );
		textActors[ n ]->SetMapper( textMappers[ n ] );
		textActors[ n ]->SetPosition( 120, 16 );//Note: the position of an Actor2D is specified in display coordinates
	}

	//Define size of the grid that will hold the objects
	int gridCols = 3, gridRows = 3;

	//Define side length(in pixels) of each renderer square
	int rendererSize = 300;

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetWindowName( "Geometric Objects Demo" );
	renderWindow->SetSize( rendererSize*gridCols, rendererSize*gridRows );

	//Set up a grid of viewports for each renderer
	for( int row = 0; row < gridRows; row++ )
	{
		for( int col = 0; col < gridCols; col++ )
		{
			int index = row*gridCols + col;

			//Create a renderer for this grid cell
			vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
			renderer->SetBackground( colors->GetColor3d( "BkgColor" ).GetData() );

			//Set the renderer's viewport dimensions(xmin, ymin, xmax, ymax) within
			//the renderwindow. Note that for the Y values, we need to subtract the
			//row index from gridRows because the viewport Y axis points upwards, but we
			//want to draw the grid from top to down.
			double viewPort[ 4 ] = 
			{
				static_cast<double>(col)/gridCols,
				static_cast<double>(gridRows - row -1)/gridRows,
				static_cast<double>(col+1)/gridCols,
				static_cast<double>(gridRows - row) / gridRows
			};
			renderer->SetViewport( viewPort );

			//Add the corresponding actor and label for this grid cell,
			//if they exist
			if( index < geometricObjectSources.size() )
			{
				renderer->AddActor( actors[ index ] );
				renderer->AddActor( textActors[ index ] );
				renderer->ResetCameraClippingRange();
			}

			renderWindow->AddRenderer( renderer );
		}
	}

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}