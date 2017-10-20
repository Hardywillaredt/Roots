#pragma once

#include "Geometry.h"
#include "RootAttributes.h"
#include "SkeletonEdge.h"

#include <vector>
#include "json\json.h"



namespace Roots
{

	typedef std::vector<SkeletonEdge> edgeList;
	typedef edgeList::iterator edgeIter;

	typedef std::vector<SkeletonEdge*> edgePtrList;
	typedef edgePtrList::iterator edgePtrIter;

	typedef std::vector<Point3d> vertList;
	typedef vertList::iterator vertIter;



	class Skeleton
	{
	public:
		Skeleton();
		Skeleton(vertList aVertices, std::vector<edgePtrList> aEdges);

		void LoadFromTextFile(char *filename);
		void LoadFromJson(Json::Value skelJson);
		void LoadFromJsonFile(char *filename);
		

		vertList getVertices();
		std::vector<edgePtrList> getEdges();


		void AddEdge(SkeletonEdge toAdd);

		void RemoveEdges(edgeList toRemove, bool careAboutAttributes = false);
		void RemoveEdge(SkeletonEdge toRemove, bool careAboutAttributes = false);
		void RemoveEdge(size_t v0, size_t v1);

		Json::Value toJson();

		friend std::ostream& operator<<(std::ostream &out, Skeleton &skeleton)
		{

			out << skeleton.toJson().toStyledString();
			return out;
		}

	private:

		//vertex list, potentially sorted in some way, but for now, nada
		vertList mVerts;

		//list of edges sorted by the id of the first vertex, then the second
		std::vector<edgePtrList> mEdges;
		
	public:
		std::vector<std::vector<size_t>> mNeighbors;
		size_t mNumVertices;
		size_t mNumEdges;
	};

	/*
	This function will take a single root (edgePtrList) and order that list of edgePtrs from beginning to end.  If no edge with a degree 1 vertex is provided as
	the forward end, then the returned edge-list will be ordered by the lower degree 1 vertex id.  Will return the 
	*/
	edgePtrList tidyUpRoot(edgePtrList toTidy, bool &valid, size_t &end1, size_t &end2, SkeletonEdge *beginning = nullptr);

	
}