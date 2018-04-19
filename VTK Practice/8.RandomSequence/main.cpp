/*
Description:
This is the "new"(December, 2009) VTK way to produce random numbers.
It is actually a random sequence generator.
*/

#include <iostream>

#include "vtkMinimalStandardRandomSequence.h"
#include "vtkSmartPointer.h"

using namespace std;

int main()
{
	//Create a random sequence generator.
	vtkSmartPointer<vtkMinimalStandardRandomSequence> sequence = vtkSmartPointer<vtkMinimalStandardRandomSequence>::New();

	//initialize the sequence
	sequence->SetSeed( 1 );

	//Get 3 random numbers.
	double dX = sequence->GetValue();
	sequence->Next();
	double dY = sequence->GetValue();
	sequence->Next();
	double dZ = sequence->GetValue();

	cout << "Sequence Value: " << endl << "A: " << dX << " B: " << dY << " C: " << dZ << endl << endl;

	//set a range on the random values
	double dRangeValueA = sequence->GetRangeValue( 10, 100 );
	sequence->Next();
	double dRangeValueB = sequence->GetRangeValue( 10, 100 );
	sequence->Next();
	double dRangeValueC = sequence->GetRangeValue( 10, 100 );

	cout << "Sequence Range(10,100) Value:" << endl << "A: " << dX << " B: " << dRangeValueB << " C: " << dRangeValueC << endl << endl;

	system( "pause" );
	return EXIT_SUCCESS;
}