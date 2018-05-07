/*
Description:

*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkNamedColors.h"
#include "vtkParametricKuen.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSliderRepresentation2D.h"
#include "vtkSliderWidget.h"
#include "vtkSmartPointer.h"

#include <array>

//These callbacks do the actual work.
//Callbacks for the interactions

class SliderCallbackMinimumU : public vtkCommand
{
public:

};

int main()
{


	return 0;
}