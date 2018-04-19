/*
Description:
This example computes the distance from a point to a line and find the closest point on the line.
*/

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkLine.h"
#include "vtkPoints.h"

int main()
{
	double line_p0[ 3 ] = { 0.0, 0.0, 0.0 };
	double line_p1[ 3 ] = { 2.0, 0.0, 0.0 };

	double p0[ 3 ] = { 1.0, 0, 0 };
	double p1[ 3 ] = { 1.0, 2.0, 0 };

	{
		double dDist0 = vtkLine::DistanceToLine( p0, line_p0, line_p1 );
		cout << "Dist0: " << dDist0 << endl;

		double dDist1 = vtkLine::DistanceToLine( p1, line_p0, line_p1 );
		cout << "Dist1: " << dDist1 << endl;
	}

	{
		double t;
		double dClosest[ 3 ];
		double dDis0 = vtkLine::DistanceToLine( p0, line_p0, line_p1, t, dClosest );
		cout << "Dist0: " << dDis0 << "  closest point: " << dClosest[ 0 ] << " " << dClosest[ 1 ] << " " << dClosest[ 2 ] << endl;

		double dDist1 = vtkLine::DistanceToLine( p1, line_p0, line_p1, t, dClosest );
		cout << "Dist1: " << dDist1 << "  closest point: " << dClosest[ 0 ] << " " << dClosest[ 1 ] << " " << dClosest[ 2 ] << endl;
	}

	system( "pause" );
	return EXIT_SUCCESS;
}