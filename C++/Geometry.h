#pragma once
#include <iostream>
#include "json\json.h"



struct Point3d
{
public:

	Point3d(double *aData);

	Point3d(double x, double y, double z);

	Point3d();

	double *getData();

	double x(); 
	double y();
	double z();

	Json::Value toJson();

	friend Point3d operator-(Point3d &first, Point3d &second)
	{
		double resultData[3];
		for (int i = 0; i < 2; ++i)
		{
			resultData[i] = first.data[i] - second.data[i];
		}
		return Point3d(resultData);
	}

	friend Point3d operator+(Point3d &first, Point3d &second)
	{
		double resultData[3];
		for (int i = 0; i < 2; ++i)
		{
			resultData[i] = first.data[i] + second.data[i];
		}
		return Point3d(resultData);
	}

	friend std::ostream& operator<<(std::ostream &out, const Point3d& point)
	{
		out << "(" << point.data[0] << " " << point.data[1] << " " << point.data[2] << ")";
		return out;
	}

private:
	double data[3];
};


