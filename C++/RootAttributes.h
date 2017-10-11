#pragma once

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

		double *getData();

	private:
		double data[NumAttributes];
	
	};
}
