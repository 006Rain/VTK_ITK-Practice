/*
Description:
This example takes a plain text file of coordinates and reads them into 
a vtkPoints, which is then put into a vtkPolyData and displayed on the
screen using a vtkVertexGlyphFilter.
An example file may look like:
1 2 3
4 5 6
9 9 -9
7 -8 5
...
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkVersion.h"
#include "vtkPoints.h"
#include "vtkVertexGlyphFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include <sstream>

using namespace std;

int main()
{
	string strFile = "../Bin/ExampleFiles/ReadTextFile.txt";
	ifstream fileStream( strFile.c_str() );
	
	string strLine;
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

	while( getline( fileStream, strLine ) )
	{
		double dX, dY, dZ;
		stringstream lineStream;
		lineStream << strLine;
		lineStream >> dX >> dY >> dZ;
		points->InsertNextPoint( dX, dY, dZ );
	}
	fileStream.close();

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints( points );

	vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();

#if VTK_MAJOR_VERSION <= 5
	glyphFilter->SetInputConnection( polyData->GetProducerPort() );
#else
	glyphFilter->SetInputData( polyData );
#endif
	glyphFilter->Update();

	//Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection( glyphFilter->GetOutputPort() );
	mapper->SetScalarRange( -50000, 20000 );//设置标量的最大最小值，修改这个可以看出渲染效果的不同  

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper( mapper );

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( actor );
	renderer->SetBackground( 0.1, 0.5, 0.8 );//background color

	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer( renderer );

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow( renderWindow );

	renderWindowInteractor->Start();
	return EXIT_SUCCESS;
}
