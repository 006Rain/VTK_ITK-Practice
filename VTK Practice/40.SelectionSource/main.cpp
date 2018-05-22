/*
Description:
Specify a selection.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkPointSource.h"
#include "vtkExtractSelection.h"
#include "vtkPolyData.h"
#include "vtkSelectionNode.h" //For POINT and INDICES enum values
#include "vtkSelectionSource.h"
#include "vtkSmartPointer.h"

int main()
{
	//Note - this generates 50 pointsand a single poly-vertex cell.
	vtkSmartPointer<vtkPointSource> pointSource = vtkSmartPointer<vtkPointSource>::New();
	pointSource->SetNumberOfPoints( 50 );
	pointSource->Update();

	std::cout << "There are " << pointSource->GetOutput()->GetNumberOfPoints() << " input points." << std::endl;

	vtkSmartPointer<vtkSelectionSource> selectionSource = vtkSmartPointer<vtkSelectionSource>::New();
	selectionSource->SetFieldType( vtkSelectionNode::POINT );
	selectionSource->SetContentType( vtkSelectionNode::INDICES );

	//Without this line, all points are passed through because the 11 points
	//we will select below are some of the points of the poly-vertex created
	//by the PointSource, so the cell(by default) gets passed through since
	//it contains some selected points, so all the points (the 500 belong to 
	//the poly-vertex) also get passed through, which is not what we are trying
	//to demonstrate.
	selectionSource->SetContainingCells( false );

	for( vtkIdType n = 10; n <= 20; n++ )
	{
		selectionSource->AddID( 0, n );
	}
	selectionSource->Update();

	vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
	extractSelection->SetInputConnection( 0, pointSource->GetOutputPort() );
	extractSelection->SetInputConnection( 1, selectionSource->GetOutputPort() );
	extractSelection->Update();

	vtkDataSet* dataSet = vtkDataSet::SafeDownCast( extractSelection->GetOutput() );
	std::cout << "There are " << dataSet->GetNumberOfPoints() << " output points." << std::endl;

	system( "pause" );
	return EXIT_SUCCESS;
}