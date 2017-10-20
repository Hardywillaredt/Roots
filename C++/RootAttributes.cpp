#include "RootAttributes.h"
#include <algorithm>
namespace Roots
{
	RootAttributes::RootAttributes()
	{
		data = new double[NumAttributes];
		for (size_t i = 0; i < NumAttributes; ++i)
		{
			data[i] = 0;
		}
	}

	RootAttributes::RootAttributes(double * aData)
	{
		data = new double[NumAttributes];
		for (size_t i = 0; i < NumAttributes; ++i)
		{
			data[i] = aData[i];
		}
	}

	RootAttributes::RootAttributes(double aThickness, double aWidth, double aLength)
	{
		data = new double[NumAttributes];
		data[Thickness] = aThickness;
		data[Width] = aWidth;
		data[Length] = aLength;
	}

	RootAttributes::RootAttributes(Json::Value rootJson)
	{
		data = new double[NumAttributes];
		for (int i = 0; i < std::min((int)NumAttributes, (int)rootJson.size()); ++i)
		{
			data[i] = rootJson[i].asDouble();
		}
	}


	RootAttributes::~RootAttributes()
	{
		delete[] data;
	}

	double* RootAttributes::getData()
	{
		return data;
	}
}

	


