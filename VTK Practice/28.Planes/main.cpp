/*
Description:
Create a hull with planes
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkVersion.h"
#include "vtkActor.h"
#include "vtkLine.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkPlanes.h"
#include "vtkHull.h"

#include <iostream>

using namespace std;

#define NUMBER_OF_PLANES 88

int main()
{
	vtkSmartPointer<vtkMath> mathObj = vtkSmartPointer<vtkMath>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkFloatArray> normals = vtkSmartPointer<vtkFloatArray>::New();
	normals->SetNumberOfComponents( 3 );
	float radius, theta, phi, x, y, z;
	int nNumOfPlanes = NUMBER_OF_PLANES;

	//创建NUMBER_OF_PLANES个随机平面，每个平面中心坐标为(x,y,z),
	//其法向量为(x,y,z).无论怎么随机，这些平面都与半径为radius
	//的球面相切，并包括该平面。
	for( int n = 0; n < nNumOfPlanes; n++ )
	{
		radius = 1;
		theta = mathObj->Random( 0, mathObj->Pi() * 2 );
		phi = mathObj->Random( 0, mathObj->Pi() );
		x = radius*sin( phi )*cos( theta );
		y = radius*sin( phi )*sin( theta );
		z = radius*cos( phi );
		points->InsertPoint( n, x, y, z );
		normals->InsertTuple3( n, x, y, z );
	}

	vtkSmartPointer<vtkPlanes> planes = vtkSmartPointer<vtkPlanes>::New();
	planes->SetPoints( points );
	planes->SetNormals( normals );

	cout << "Planes Number: " << planes->GetNumberOfPlanes() << endl;

	vtkSmartPointer<vtkHull> hull = vtkSmartPointer<vtkHull>::New();
	hull->SetPlanes( planes );
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	hull->GenerateHull( polyData, -1, 1, -1, 1, -1, 1 );

	vtkSmartPointer<vtkPolyDataMapper> hullMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	hullMapper->SetInputData( polyData );

	vtkSmartPointer<vtkActor> hullActor = vtkSmartPointer<vtkActor>::New();
	hullActor->SetMapper( hullMapper );

	//render
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> rendWin = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> rendWinInteract = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renderer->AddActor( hullActor );
	rendWin->AddRenderer( renderer );
	rendWinInteract->SetRenderWindow( rendWin );

	renderer->ResetCamera();
	rendWin->Render();
	rendWinInteract->Start();

	return EXIT_SUCCESS;
}

