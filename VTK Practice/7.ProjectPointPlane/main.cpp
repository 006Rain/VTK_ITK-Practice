/*
Description:
This example projects a point on to a plane. To use this example, 
you must have the CVSHEAD version of VTK(as of 02/02/2010).
*/

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkPlane.h"

int main()
{
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	plane->SetOrigin( 0.0, 0.0, 0.0 );
	plane->SetNormal( 0.0, 0.0, 1.0 );

	double dPoint[ 3 ] = { 23.1, 54.6, 9.2 };
	double dOrigin[ 3 ] = { 0.0, 0.0, 0.0 };
	double dNormal[ 3 ] = { 0.0, 0.0, 1.0 };
	double dProjected[ 3 ];

	plane->ProjectPoint( dPoint, dOrigin, dNormal, dProjected );

	cout << "Projected Point: " << dProjected[ 0 ] << " " << dProjected[ 1 ] << " " << dProjected[ 2 ] << endl;

	system( "pause" );
	return EXIT_SUCCESS;
}