/*
Description:
Compute the center of a mass of points.
*/

#include "vtkSmartPointer.h"
#include "vtkCenterOfMass.h"
#include "vtkDoubleArray.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"

#include <limits>
#include <cmath>

int main()
{
	//Create a point set of a square
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint( 0, 0, 0 );
	points->InsertNextPoint( 1, 0, 0 );
	points->InsertNextPoint( 0, 1, 0 );
	points->InsertNextPoint( 1, 1, 0 );

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints( points );

	//Compute the center of mass
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
	centerOfMassFilter->SetInputData( polyData );
	centerOfMassFilter->SetUseScalarsAsWeights( false );
	centerOfMassFilter->Update();

	double dCenter[ 3 ];
	centerOfMassFilter->GetCenter( dCenter );

	std::cout << "Center of mass is :" << endl << dCenter[ 0 ] << " " << dCenter[ 1 ] << " " << dCenter[ 2 ] << endl;

	system( "pause" );
	return EXIT_SUCCESS;
}