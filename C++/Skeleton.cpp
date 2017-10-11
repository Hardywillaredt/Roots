#include "Skeleton.h"

#include <algorithm>
#include <iostream>
#include <fstream>


namespace Roots
{

	SkeletonEdge::SkeletonEdge()
		:v0(0), v1(0)	
	{
		attributes = RootAttributes();
	}

	SkeletonEdge::SkeletonEdge(size_t firstVert, size_t secondVert, RootAttributes aAttributes)
		: v0(firstVert), v1(secondVert), attributes(aAttributes) {}

	SkeletonEdge::SkeletonEdge(size_t firstVert, size_t secondVert, double * attributeData)
		: v0(firstVert), v1(secondVert), attributes(attributeData) {}



	Skeleton::Skeleton()
		: mVerts(), mEdges() {}


	Skeleton::Skeleton(vertList aVertices, edgeList aEdges)
		: mVerts(aVertices), mEdges(aEdges)
	{
		std::sort(mEdges.begin(), mEdges.end());
	}

	Skeleton::Skeleton(std::string filename)
	{
		Skeleton(filename.c_str());
	}

	Skeleton::Skeleton(char * filename)
	{
		std::ifstream file(filename);

		int numVertices, numEdges, numFaces;
		file >> numVertices >> numEdges >> numFaces;

		mVerts = vertList(numVertices, Point3d());
		double x, y, z;
		for (int i = 0; i < numVertices; ++i)
		{
			file >> x >> y >> z;
			mVerts[i] = Point3d(x, y, z);
		}

		mEdges = edgeList(numEdges, SkeletonEdge());
		int v0, v1;
		double attributes[RootAttributes::NumAttributes];
		for (int i = 0; i < numEdges; ++i)
		{
			file >> v0 >> v1;
			for (int att = 0; att < RootAttributes::NumAttributes; ++att)
			{
				file >> attributes[att];
			}

			mEdges[i] = SkeletonEdge(v0, v1, attributes);
		}

		if (!file.eof())
		{
			std::cout << "not all data read, thats kind of wierd " << std::endl;
		}

		/////////////////////////////////// TO DO ->  DEAL WITH FACES   ///////////////////////////
	}

	vertList Skeleton::getVertices()
	{
		return mVerts;
	}

	edgeList Skeleton::getEdges()
	{
		return mEdges;
	}

	edgeIter Skeleton::FindEdge(size_t v0, size_t v1)
	{
		SkeletonEdge toFind = SkeletonEdge(v0, v1);
		return FindEdge(toFind);
	}

	edgeIter Skeleton::FindEdge(SkeletonEdge toFind)
	{
		size_t L = 0;
		size_t R = mEdges.size();
		size_t m = (L + R) / 2;
		while (mEdges[m] != toFind)
		{
			m = (L + R) / 2;
			if (mEdges[m] < toFind)
			{
				L = m + 1;
			}
			else if (toFind < mEdges[m])
			{
				R = m - 1;
			}
			if (L > R)
			{
				return mEdges.end();
			}
		}

		return mEdges.begin() + m;
	}

	edgeIter Skeleton::FindNextEdge(SkeletonEdge toFind)
	{
		size_t L = 0;
		size_t R = mEdges.size() - 1;
		size_t m = (L + R) / 2;
		while (mEdges[m] != toFind)
		{
			m = (L + R) / 2;
			if (mEdges[m] < toFind)
			{
				L = m + 1;
			}
			else if (toFind < mEdges[m])
			{
				R = m - 1;
			}
			if (L > R)
			{
				if (toFind < mEdges[R])
				{
					return mEdges.begin() + R;
				}
				else
				{
					return mEdges.begin() + L;
				}
			}
		}

		return mEdges.begin() + m ;

	}

	void Skeleton::AddEdge(SkeletonEdge toAdd)
	{
		edgeIter nextEdge = FindNextEdge(toAdd);
		mEdges.insert(nextEdge, toAdd);
	}


	/*edgeList Skeleton::ShortestPath(size_t v0, size_t v1)
	{
		
	}*/
}