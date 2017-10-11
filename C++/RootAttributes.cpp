#include "RootAttributes.h"

namespace Roots
{
	RootAttributes::RootAttributes()
	{
		for (size_t i = 0; i < NumAttributes; ++i)
		{
			data[i] = 0;
		}
	}

	RootAttributes::RootAttributes(double * aData)
	{
		for (size_t i = 0; i < NumAttributes; ++i)
		{
			data[i] = aData[i];
		}
	}

	RootAttributes::RootAttributes(double aThickness, double aWidth, double aLength)
	{
		data[Thickness] = aThickness;
		data[Width] = aWidth;
		data[Length] = aLength;
	}

	double* RootAttributes::getData()
	{
		return data;
	}
}

	


