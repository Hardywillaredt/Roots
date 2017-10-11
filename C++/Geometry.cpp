#include "Geometry.h"



Point3d::Point3d(double *aData)
{
	data[0] = aData[0];
	data[1] = aData[1];
	data[2] = aData[2];
}

Point3d::Point3d(double x, double y, double z)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

Point3d::Point3d()
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
}


double * Point3d::getData()
{
	return data;
}

double Point3d::x() 
{ 
	return data[0]; 
}
double Point3d::y()
{
	return data[1]; 
}
double Point3d::z() 
{
	return data[2]; 
}

