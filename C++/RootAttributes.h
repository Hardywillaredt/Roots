#pragma once
#include "json\json.h"

namespace Roots
{

	struct RootAttributes
	{

	public:
		enum Attributes
		{
			Thickness = 0,
			Width,
			Length,


			NumAttributes
		};		

		RootAttributes();
		RootAttributes(double *attributeData);
		RootAttributes(double aThickness, double aWidth, double aLength);
		RootAttributes(Json::Value rootJson);
		~RootAttributes();
		double *getData();

		bool operator==(RootAttributes& second)
		{
			bool same = true;
			for (int i = 0; i < NumAttributes; ++i)
			{
				same = same && data[i] == second[i];
			}
			return same;
		}

		double operator[](const int index)
		{
			return data[index];
		}

	private:
		double *data;
	
	};
}
