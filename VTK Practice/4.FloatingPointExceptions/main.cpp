//Actually, this program doesn't work.

/*
Description:
This example makes the illegal division by zero produce an error: "Error: Floating point exception detected. Signal 8 rather than simply store "inf" in y.
*/

#include <iostream>
#include "vtkFloatingPointExceptions.h"

//Avoid the "potential divide by 0" warning from Microsoft
//compiler, since this example intentionally divides by 0.
#ifdef _MSC_VER
#pragma warning (disable: 4723)
#endif

int main()
{
	//Disabled by default with gcc or visual studio.
	//Enabled by default by Borland CC;
	vtkFloatingPointExceptions::Enable();

	double x = 0.0;
	double y = 1.0 / x; //floating-point exception

	cout << "x: " << x << " y:" << y << endl;

	system( "pause" );
	return EXIT_SUCCESS;
}