/*
Warning: Program Error.
*/

/*
Descriptions:
Create a contour from a structured point set(image).
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkMarchingSquares.h"
#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPNGReader.h"
#include "vtkDataSetMapper.h"
#include "vtkCamera.h"
#include "vtkProperty.h"

int main()
{
	std::string strFile = "../Bin/ExampleFiles/arrow_left.png";
	int isoValue = 100;

	vtkSmartPointer<vtkPNGReader> reader = vtkSmartPointer<vtkPNGReader>::New();
	if( !reader->CanReadFile( strFile.c_str() ) )
	{
		std::cerr << "Error: Could not read " << strFile << ".\n";
		return EXIT_FAILURE;
	}

	reader->SetFileName( strFile.c_str() );
	reader->Update();

	vtkSmartPointer<vtkMarchingSquares> isoSurface = vtkSmartPointer<vtkMarchingSquares>::New();
	isoSurface->SetInputConnection( reader->GetOutputPort() );
	isoSurface->SetValue( 0, isoValue );

	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputConnection( isoSurface->GetOutputPort() );
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->SetColor( 0.8900, 0.8100, 0.3400 );

	//Render
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetMultiSamples( 0 );
	renderWindow->AddRenderer( renderer );

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow( renderWindow );

	renderer->AddActor( actor );
	renderWindow->Render();
	interactor->Initialize();
	interactor->Start();

	return 0;
}