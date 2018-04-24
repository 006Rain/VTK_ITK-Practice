/*
Description:

*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );

#include <sstream>

#include "vtkVersion.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkDelimitedTextWriter.h"
#include "vtkSphereSource.h"
#include "vtkVariantArray.h"
#include "vtkTable.h"

using namespace std;

int main()
{
	string strDstFile = "./TestDelimitedWriter.txt";

	//construct an empty table
	vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

	for( int n = 0; n < 3; n++ )
	{
		vtkSmartPointer<vtkVariantArray> col = vtkSmartPointer<vtkVariantArray>::New();

		ostringstream oss;
		oss << "column-" << n;
		string colName = oss.str();
		col->SetName( colName.c_str() );

		col->InsertNextValue( vtkVariant( 0.0 ) );
		col->InsertNextValue( vtkVariant( 0.0 ) );
		col->InsertNextValue( vtkVariant( 0.0 ) );

		table->AddColumn( col );
	}

	//fill the table with values
	unsigned int nCounter = 0;
	for( vtkIdType r = 0; r < table->GetNumberOfRows(); r++ )
	{
		for( vtkIdType c = 0; c < table->GetNumberOfColumns(); c++ )
		{
			table->SetValue( r, c, vtkVariant( nCounter ) );
			nCounter++;
		}
	}

	vtkSmartPointer<vtkDelimitedTextWriter> writer = vtkSmartPointer<vtkDelimitedTextWriter>::New();
	writer->SetFileName( strDstFile.c_str() );

#if	VTK_MAJOR_VERSION <= 5
	writer->SetInputConnection( table->GetProducerPort() );
#else
	writer->SetInputData( table );
#endif

	writer->Write(); 
	writer->Update();

	return EXIT_SUCCESS;
}