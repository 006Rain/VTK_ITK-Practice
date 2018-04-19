/*
Description:
This example generates 3 random numbers from a Gaussian distribution with 
mean 0.0 and  standard deviation 2.0.
*/

#include <iostream>

#include "vtkMath.h"
#include "time.h"

int main()
{
	//The number of random numbers we wish to produce
	unsigned int nNumRand = 3;

	//without this line, the random numbers will be the same in each case
#if  1
	vtkMath::RandomSeed( time( NULL ) );
#endif

	//Generate nNumRand random numbers from a Gaussian distribution with 
	//mean 0.0 and standard deviation 2.0
	for( unsigned int n = 0; n < nNumRand; n++ )
	{
		double dTemp = vtkMath::Gaussian( 0.0, 2.0 );
		cout << dTemp << endl;
	}

	system( "pause" );
	return EXIT_SUCCESS;
}