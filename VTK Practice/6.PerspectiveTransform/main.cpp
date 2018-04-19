//The matrix is always not easy to understand!

/*
Description:
The perspective transform automatically appends the "homogeneous 1"
to the point before it is multiplied by the matrix. It then divides the 
first 3 coordinates of the result by the 4th coordinate of the result.
For comparison, the standard projection( vtkTransform ) is also performed
with the same matrix and the same point. Note that the result are 
much different.
*/

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkPerspectiveTransform.h"
#include "vtkTransform.h"
#include "vtkMatrix4x4.h"

using namespace std;

int main()
{
	vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
	mat->SetElement( 0, 0, 1 );
	mat->SetElement( 0, 1, 2 );
	mat->SetElement( 0, 2, 3 );
	mat->SetElement( 0, 3, 4 );

	mat->SetElement( 1, 0, 2 );
	mat->SetElement( 1, 1, 2 );
	mat->SetElement( 1, 2, 3 );
	mat->SetElement( 1, 3, 4 );

	mat->SetElement( 2, 0, 3 );
	mat->SetElement( 2, 1, 2 );
	mat->SetElement( 2, 2, 3 );
	mat->SetElement( 2, 3, 4 );

	mat->SetElement( 3, 0, 4 );
	mat->SetElement( 3, 1, 2 );
	mat->SetElement( 3, 2, 3 );
	mat->SetElement( 3, 3, 4 );


	vtkSmartPointer<vtkPerspectiveTransform> perspectiveTransform = vtkSmartPointer<vtkPerspectiveTransform>::New();
	perspectiveTransform->SetMatrix( mat );

	vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
	trans->SetMatrix( mat );

	double dP[ 3 ] = { 1.0, 2.0, 3.0 };

	double dNormalProjection[ 3 ]; //coordinate after projection transform
	trans->TransformPoint( dP, dNormalProjection );

	cout << "Standard projection: " << dNormalProjection[ 0 ] << " "
		<< dNormalProjection[ 1 ] << " " << dNormalProjection[ 2 ] << endl;
	
	double dPerspectiveProjection[ 3 ];//coordinate after perspective projection transform
	perspectiveTransform->TransformPoint( dP, dPerspectiveProjection );
	cout << "Perspective projection: " << dPerspectiveProjection[0] << " "
		<< dPerspectiveProjection[ 1 ] << " " << dPerspectiveProjection[ 2 ] << endl;

	system( "pause" );
	return EXIT_SUCCESS;
}