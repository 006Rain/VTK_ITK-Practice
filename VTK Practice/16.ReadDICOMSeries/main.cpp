/*
Description:
This example demonstrates how to read a series of DICOM images and how to 
scroll with the mouse wheel or the the up/down keys through all slices.
*/

#include "vtkAutoInit.h"
VTK_MODULE_INIT( vtkRenderingOpenGL2 );
VTK_MODULE_INIT( vtkInteractionStyle );
VTK_MODULE_INIT( vtkRenderingFreeType );

#include <sstream>

#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkActor2D.h"
#include "vtkTextProperty.h"
#include "vtkTextMapper.h"

using namespace std;

//helper class to format slice status message
class StatusMessage
{
public:
	static string Format( int nSlice, int nMaxSlice )
	{
		stringstream strStreamTmp;
		strStreamTmp << "Slice Number: " << nSlice + 1 << "/" << nMaxSlice + 1;
		return strStreamTmp.str();
	}
};

//Define own interaction style
class myInteractorStyleImage : public vtkInteractorStyleImage
{
public:
	static myInteractorStyleImage* New();
	vtkTypeMacro( myInteractorStyleImage, vtkInteractorStyleImage );

protected:
	vtkImageViewer2* m_pImageViewer;
	vtkTextMapper* m_pStatusMapper;
	int m_nSlice;
	int m_nMinSlice;
	int m_nMaxSlice;

public:
	void SetImageViewer( vtkImageViewer2* pImgViewer )
	{
		m_pImageViewer = pImgViewer;
		m_nMinSlice = pImgViewer->GetSliceMin();
		m_nMaxSlice = pImgViewer->GetSliceMax();
		m_nSlice = m_nMinSlice;
		cout << "Slice Min = " << m_nMinSlice << ", Max = " << m_nMaxSlice << endl;
	}

	void SetStatusMapper( vtkTextMapper* pStatusMapper )
	{
		m_pStatusMapper = pStatusMapper;
	}

protected:
	void MoveSliceForward()
	{
		if( m_nSlice < m_nMaxSlice )
		{
			m_nSlice++;
			cout << "Move Slice Forward::Slice = " << m_nSlice << endl;
			m_pImageViewer->SetSlice( m_nSlice );
			string strMsg = StatusMessage::Format( m_nSlice, m_nMaxSlice );
			m_pStatusMapper->SetInput( strMsg.c_str() );
			m_pImageViewer->Render();
		}
	}

	void MoveSliceBackward()
	{
		if( m_nSlice > m_nMinSlice )
		{
			m_nSlice--;
			cout << "Move Slice Backward::Slice = " << m_nSlice << endl;
			m_pImageViewer->SetSlice( m_nSlice );
			string strMsg = StatusMessage::Format( m_nSlice, m_nMaxSlice );
			m_pStatusMapper->SetInput( strMsg.c_str() );
			m_pImageViewer->Render();
		}
	}

	virtual void OnKeyDown()
	{
		string strKey = this->GetInteractor()->GetKeySym();
		if( strKey.compare( "Up" ) == 0 )
		{
			MoveSliceForward();
		}
		else if( strKey.compare( "Down" ) == 0 )
		{
			MoveSliceBackward();
		}

		vtkInteractorStyleImage::OnKeyDown();
	}

	virtual void OnMouseWheelForward()
	{
		MoveSliceForward();
	}

	virtual void OnMouseWheelBackward()
	{
		MoveSliceBackward();
	}
};

#include "vtkObjectFactory.h"
vtkStandardNewMacro( myInteractorStyleImage );

int main()
{
	string strDir = "../Bin/ExampleFiles/DicomFile";

	//Read all the DICOM files in the specified directory
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName( strDir.c_str() );
	reader->Update();

	//Visualize
	vtkSmartPointer<vtkImageViewer2> imgViewer = vtkSmartPointer<vtkImageViewer2>::New();
	imgViewer->SetInputConnection( reader->GetOutputPort() );

	//Slice Status message
	vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize( 20 );
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	string strMsg = StatusMessage::Format( imgViewer->GetSliceMin(), imgViewer->GetSliceMax() );
	sliceTextMapper->SetInput( strMsg.c_str() );
	sliceTextMapper->SetTextProperty( sliceTextProp );

	vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper( sliceTextMapper );
	sliceTextActor->SetPosition( 15, 10 );

	//usage hint massage
	vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
	usageTextProp->SetFontFamilyToCourier();
	usageTextProp->SetFontSize( 14 );
	usageTextProp->SetVerticalJustificationToTop();
	usageTextProp->SetJustificationToLeft();

	vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	usageTextMapper->SetInput( "- Slice with mouse wheel \n or Up/Down-Key\n-Zoom with pressed right\n mouse button while dragging" );
	usageTextMapper->SetTextProperty( usageTextProp );

	vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
	usageTextActor->SetMapper( usageTextMapper );
	usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
	usageTextActor->GetPositionCoordinate()->SetValue( 0.05, 0.95 );

	//create an interactor with our own style(inherit from vtkInteractorImage)
	//in order to catch mouse wheel and key events
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<myInteractorStyleImage> myInteratorStyle = vtkSmartPointer<myInteractorStyleImage>::New();

	//make imageviewer2 and sliceTextMapper visible to our interatorStyle
	//to enable slice status message updates when scrolling through the slices
	myInteratorStyle->SetImageViewer( imgViewer );
	myInteratorStyle->SetStatusMapper( sliceTextMapper );

	imgViewer->SetupInteractor( renderWindowInteractor );

	//make the interator use our own interactorStyle
	//cause SetupInteractor() is defining it's own defalt interactorStyle
	//this must be called after SetupInteractor()
	renderWindowInteractor->SetInteractorStyle( myInteratorStyle );//**************Set Custom Interactor***************//

	//add slice status message and usage hint message to the renderer
	imgViewer->GetRenderer()->AddActor2D( sliceTextActor );
	imgViewer->GetRenderer()->AddActor2D( usageTextActor );

	imgViewer->GetRenderWindow()->SetSize( 600, 400 );
	imgViewer->Render();
	imgViewer->GetRenderer()->ResetCamera();
	imgViewer->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}