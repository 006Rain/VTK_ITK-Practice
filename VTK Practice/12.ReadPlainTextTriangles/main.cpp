/*
Description:
Here is an example of a custom file-formate reader that produces a VTK XML
PolyData from a plain-text input formate.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkVersion.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkXMLPPolyDataWriter.h"

using namespace std;

vtkPolyData* CustomerReader( istream& inStream )
{
	vtkIdType number_of_points, number_of_triangles;
	inStream >> number_of_points >> number_of_triangles;

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->SetNumberOfPoints( number_of_points );
	for( vtkIdType n = 0; n < number_of_points; n++ )
	{
		double dX, dY, dZ;
		inStream >> dX >> dY >> dZ;
		points->SetPoint( n, dX, dY, dZ );
	}

	vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
	for( vtkIdType n = 0; n < number_of_triangles; n++ )
	{
		vtkIdType a, b, c;
		inStream >> a >> b >> c;
		cellArray->InsertNextCell( 3 );
		cellArray->InsertCellPoint( a );
		cellArray->InsertCellPoint( b );
		cellArray->InsertCellPoint( c );
	}

	vtkPolyData* polyData = vtkPolyData::New();
	polyData->SetPoints( points );
	polyData->SetPolys( cellArray );
	return polyData;
}

int main()
{
	string strFile = "../Bin/ExampleFiles/ReadPlainTextTriangles.txt";
	ifstream fileStream( strFile );
	
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::Take( CustomerReader( fileStream ) );

	vtkSmartPointer<vtkXMLPPolyDataWriter> xmlWriter = vtkSmartPointer<vtkXMLPPolyDataWriter>::New();

#if VTK_MAJOR_VERSION <= 5
	xmlWriter->SetInput( polyData );
#else
	xmlWriter->SetInputData( polyData );
#endif
	xmlWriter->SetFileName( "x.vtp" );
	xmlWriter->Write();

	return EXIT_SUCCESS;
}