/*
Description:
This example finds the squared distance and the Euclidean distance between 3D points.
*/

#include <iostream>

#include "vtkMath.h"

using namespace std;

int main()
{
	//Create two points.
	double p0[ 3 ] = { 0.0, 0.0, 0.0 };
	double p1[ 3 ] = { 1.0, 1.0, 1.0 };

	//Find the squared distance between the points.
	double dSquaredDistance = vtkMath::Distance2BetweenPoints( p0, p1 );

	//Take the square root to get the Euclidean distance between the points.
	double dDistance = sqrt( dSquaredDistance );

	//Output the results.
	cout << "SquaredDistance = " << dSquaredDistance << endl;
	cout << "SquaredDistance = " << dDistance << endl;

	system( "pause" );

	return EXIT_SUCCESS;
}