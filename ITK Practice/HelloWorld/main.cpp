#include <iostream>

#include "itkImage.h"

using namespace std;

int main()
{
	using ImageType = itk::Image<unsigned short, 3>;
	ImageType::Pointer image = ImageType::New();

	cout << "ITK Hello, World !" << endl;
	cout << endl << image << endl;

	system( "pause" );

	return 0;
}