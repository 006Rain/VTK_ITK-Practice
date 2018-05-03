/*
Description:
Calculate the intersect point with function: IntersectWithLine.

IntersectWithLine: if there is an intersection between polygon 
	and line, it returns value ( != 0 ), else it returns 
	value( == 0 ).
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkPolygon.h"

int main()
{
	//Create a square in the XY plane
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint( 0.0, 0.0, 0.0 );
	points->InsertNextPoint( 1.0, 0.0, 0.0 );
	points->InsertNextPoint( 1.0, 1.0, 0.0 );
	points->InsertNextPoint( 0.0, 1.0, 0.0 );

	//Create the polygon
	vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();
	polygon->GetPoints()->DeepCopy( points );
	polygon->GetPointIds()->SetNumberOfIds( 4 );//4 corners of the square
	polygon->GetPointIds()->SetId( 0, 0 );
	polygon->GetPointIds()->SetId( 1, 1 );
	polygon->GetPointIds()->SetId( 2, 2 );
	polygon->GetPointIds()->SetId( 3, 3 );

	//Inputs
	double p1[ 3 ] = { 0.1, 0, -1.0 };
	double p2[ 3 ] = { 0.1, 0, 1.0 };
	double tolerance = 0.001;

	//Outputs
	double t; //Parametric coordinate of intersection( 0, (corresponding to p1) )
	double x[ 3 ];//The coordinate of the intersection
	double pcoords[ 3 ];
	int subId;

	vtkIdType iD = polygon->IntersectWithLine( p1, p2, tolerance, t, x, pcoords, subId );

	std::cout << "intersected: " << iD << std::endl;
	std::cout << "intersection: " << " x: " << x[ 0 ] << " y: " << x[ 1 ] << " z: " << x[ 2 ] << std::endl;

	system( "pause" );
	return EXIT_SUCCESS;
}