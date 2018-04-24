/*
Description:
This example takes a plain text file of coordinates and 
normals(x y z nx ny nz) and reads them into a vtkPolyData
and displays them on the screen. This can be easily changed 
to reading a file with any delimiter by changing the argument
of Reader->SetFieldDelimiterCharacters("");

Here is an example:
0.0 0.0 0.0 1.0 2.0 3.0 
1.0 0.0 0.0 4.0 5.0 6.1 
0.0 1.0 0.0 7.2 8.3 9.4
*/

/*
Class vtkVertexGlyphFilter:
	Make a vtkPolyData with a vertex on each point.
	This filter throws away all of the cells in the input and replaced them 
	with a vertex on each point. The intended use of this filter is roughly
	equivalent to the vtkGlyph3D filter, except this filter is specifically
	for data that has many vertices, making the rendered result faster and
	less cluttered than the glyph filter. This filter may take a graph or point
	as input.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkDelimitedTextReader.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkDoubleArray.h"
#include "vtkTable.h"
#include "vtkPointData.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkVersionMacros.h" //for version macros
#include "vtkVertexGlyphFilter.h"
#include "vtkNamedColors.h"

using namespace std;

int main()
{
	string strFile = "../Bin/ExampleFiles/DelimitedTextReader.txt";

	vtkSmartPointer<vtkDelimitedTextReader> reader = vtkSmartPointer<vtkDelimitedTextReader>::New();
	reader->SetFileName( strFile.c_str() );
	reader->DetectNumericColumnsOn();
	reader->SetFieldDelimiterCharacters( " " );
	reader->Update();

	vtkTable* table = reader->GetOutput();

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkDoubleArray> normals = vtkSmartPointer<vtkDoubleArray>::New();

	normals->SetNumberOfComponents( 3 ); //3d normals (ie x, y, z)
	cout << "Table has " << table->GetNumberOfRows()
		<< " rows." << endl;

	cout << "Table has " << table->GetNumberOfColumns()
		<< " columns." << endl;

	for( vtkIdType i = 0; i < table->GetNumberOfRows(); i++ )
	{
		double dX = table->GetValue( i, 0 ).ToDouble();
		double dY = table->GetValue( i, 1 ).ToDouble();
		double dZ = table->GetValue( i, 2 ).ToDouble();
		cout << "x: " << dX << " y: " << dY << " z: " << dZ;
		points->InsertNextPoint( dX, dY, dZ );

		double dTemp[ 3 ];
		dTemp[ 0 ] = table->GetValue( i, 3 ).ToDouble();
		dTemp[ 1 ] = table->GetValue( i, 4 ).ToDouble();
		dTemp[ 2 ] = table->GetValue( i, 5 ).ToDouble();

		cout << "n: " << dTemp[ 0 ] << " " << dTemp[ 1 ] << " " << dTemp[ 2 ] << endl;
		normals->InsertNextTuple( dTemp );
	}

	cout << "There are " << points->GetNumberOfPoints() << " points." << endl;

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints( points );
	polyData->GetPointData()->SetNormals( normals );

	vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();

#if VTK_MAJOR_VERSION <= 5
	glyphFilter->SetInputConnection( polyData->GetProducerPort() );
#else
	glyphFilter->SetInputData( polyData );
#endif
	glyphFilter->Update();

	//Visualize
	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( glyphFilter->GetOutputPort() );

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );
	actor->GetProperty()->SetPointSize( 10 );
	actor->GetProperty()->SetColor( colors->GetColor3d( "Tomato" ).GetData() );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( colors->GetColor3d( "Mint" ).GetData() );

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );
	
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow( renderWindow );

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}