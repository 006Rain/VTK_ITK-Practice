/*
Description:
This example generate 10 random numbers between 0.0 and 10.0
from a uniform distribution.
*/

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkMath.h"
#include "time.h"

using namespace std;

int main()
{
	//Set the number of random numbers we wish to produce to 5.
	unsigned int nNumRand = 10;

	//Without this line, the random numbers will be the same every iteration.
	vtkMath::RandomSeed( time( NULL ) );

	cout << "Generate 10 uniform random number between 0.0 and 10.0: " << endl << endl;
	//Generate nNumRand random numbers from a uniform distribution between 0.0 and 10.0.
	for( int n = 0; n < nNumRand; n++ )
	{
		double dTemp = vtkMath::Random( 0.0, 10.0 );
		cout << dTemp << endl;
	}

	cout << endl;
	system( "pause" );
	return EXIT_SUCCESS;
}