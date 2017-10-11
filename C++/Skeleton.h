#pragma once

#include "Geometry.h"
#include "RootAttributes.h"

#include <vector>



namespace Roots
{


	struct SkeletonEdge
	{
	public:
		size_t v0, v1;
		RootAttributes attributes;
		SkeletonEdge();
		SkeletonEdge(size_t firstVert, size_t secondVert, RootAttributes aAttributes = RootAttributes());
		SkeletonEdge(size_t firstVert, size_t secondVert, double *aAttributeData);

		friend bool operator<(SkeletonEdge &first, SkeletonEdge &second)
		{
			return first.v0 < second.v0 ? true : (second.v0 < first.v0 ? false : (first.v1 < second.v1 ? true : false));
		}

		//this operator ignores the attribution information for convenience in the edge finding and sorting process
		friend bool operator==(SkeletonEdge &first, SkeletonEdge &second)
		{
			return first.v0 == second.v0 && first.v1 == second.v1;
		}

		friend bool operator!=(SkeletonEdge &first, SkeletonEdge &second)
		{
			return !(first == second);
		}

		friend std::ostream& operator<<(std::ostream& out, SkeletonEdge &edge)
		{
			double *data = edge.attributes.getData();
			out << "(" << edge.v0 << " " << edge.v1 << ") {";
			for (int i = 0; i < RootAttributes::NumAttributes - 1; ++i)
			{
				
				out << data[i] << ", ";
			}
			out << data[RootAttributes::NumAttributes - 1] << "}";
			return out;
		}
	};



	typedef std::vector<SkeletonEdge> edgeList;
	typedef edgeList::iterator edgeIter;

	typedef std::vector<Point3d> vertList;
	typedef vertList::iterator vertIter;



	class Skeleton
	{
	public:
		Skeleton();
		Skeleton(vertList aVertices, edgeList aEdges);
		Skeleton(std::string filename);
		Skeleton(char * filename);

		vertList getVertices();
		edgeList getEdges();

		edgeIter FindEdge(size_t v0, size_t v1);
		edgeIter FindEdge(SkeletonEdge toFind);

		edgeIter FindNextEdge(SkeletonEdge toFind);

		void AddEdge(SkeletonEdge toAdd);


		//edgeList ShortestPath(size_t v0, size_t v1);

		//void RemoveEdges(edgeList toRemove);
		//void RemoveEdge(SkeletonEdge toRemove);
		//void RemoveEdge(size_t v0, size_t v1);

	private:

		//vertex list, potentially sorted in some way, but for now, nada
		vertList mVerts;

		//list of edges sorted by the id of the first vertex, then the second
		edgeList mEdges;

	};
}