#include "Skeleton.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>

namespace
{
	std::string edgeString = "Edges";
	std::string vertexString = "Vertices";
}


namespace Roots
{


	Skeleton::Skeleton()
		: mVerts(), mEdges(), mNumEdges(), mNumVertices(), mNeighbors() {}


	Skeleton::Skeleton(vertList aVertices, std::vector<edgePtrList> aEdges)
		: mVerts(aVertices), mEdges(aEdges)
	{
		mNumEdges = 0;
		mNumVertices = mVerts.size();
		mNeighbors = std::vector<std::vector<size_t>>(mVerts.size());
		for (int i = 0; i < mEdges.size(); ++i)
		{
			mNumEdges += mEdges[i].size();
			for (int j = 0; j < mEdges[i].size(); ++j)
			{
				SkeletonEdge *edge = mEdges[i][j];
				mNeighbors[edge->v0].push_back(edge->v1);
				mNeighbors[edge->v1].push_back(edge->v0);
			}
		}
	}

	void Skeleton::LoadFromTextFile(char *filename)
	{
		std::ifstream file(filename);

		int numVertices, numEdges, numFaces;
		file >> numVertices >> numEdges >> numFaces;

		mNumEdges = numEdges;
		mNumVertices = numVertices;

		mVerts = vertList(numVertices, Point3d());
		double x, y, z;
		for (int i = 0; i < numVertices; ++i)
		{
			file >> x >> y >> z;
			mVerts[i] = Point3d(x, y, z);
		}

		mEdges = std::vector<edgePtrList>(numVertices, edgePtrList());
		int v0, v1;
		double *attributes = new double[RootAttributes::NumAttributes];
		for (int i = 0; i < numEdges; ++i)
		{
			file >> v0 >> v1;
			for (int att = 0; att < RootAttributes::NumAttributes; ++att)
			{
				file >> attributes[att];
			}
			AddEdge(SkeletonEdge(v0, v1, attributes));
			//mEdges[v0].push_back(new SkeletonEdge(v0, v1, attributes));
			std::cout << "Adding edge (" << v0 << " " << v1 << ")" << std::endl;
			std::cout << "edge added " << (mEdges[v0][mEdges[v0].size()-1]->v0) << " " << (mEdges[v1][mEdges[v1].size() - 1]->v1) << std::endl;
		}

		for (int i = 0; i < mEdges.size(); ++i)
		{
			std::sort(mEdges[i].begin(), mEdges[i].end());
		}

		if (!file.eof())
		{
			std::cout << "not all data read, thats kind of wierd " << std::endl;
		}

		/////////////////////////////////// TO DO ->  DEAL WITH FACES   ///////////////////////////
	}

	void Skeleton::LoadFromJson(Json::Value skelJson)
	{
		int numVertices = skelJson[vertexString].size();
		int numEdges = skelJson[edgeString].size();

		mNumEdges = numEdges;
		mNumVertices = numVertices;

		mVerts = vertList(numVertices, Point3d());
		Json::Value vertJson = skelJson[vertexString];
		for (int i = 0; i < numVertices; ++i)
		{

			mVerts[i] = Point3d(vertJson[0].asDouble(), vertJson[1].asDouble(), vertJson[2].asDouble());
		}

		Json::Value edgeJson = skelJson[edgeString];
		mEdges = std::vector<edgePtrList>(numVertices, edgePtrList());
		for (int i = 0; i < numEdges; ++i)
		{
			SkeletonEdge edge = SkeletonEdge(edgeJson[i]);
			AddEdge(edge);
			//mEdges[edge.v0].push_back(new SkeletonEdge(edge));
		}

		for (int i = 0; i < mEdges.size(); ++i)
		{
			std::sort(mEdges[i].begin(), mEdges[i].end());
		}
	}

	void Skeleton::LoadFromJsonFile(char *filename)
	{
		Json::Value skelJson;
		std::ifstream file(filename);
		file >> skelJson;
		LoadFromJson(skelJson);
	}

	vertList Skeleton::getVertices()
	{
		return mVerts;
	}

	std::vector<edgePtrList> Skeleton::getEdges()
	{
		return mEdges;
	}


	//int Skeleton::FindEdgePos(size_t v0, size_t v1)
	//{
	//	SkeletonEdge toFind = SkeletonEdge(v0, v1);
	//	return FindEdgePos(toFind);
	//}
	//
	//int Skeleton::FindEdgePos(SkeletonEdge toFind, bool careAboutAttributes)
	//{
	//	size_t L = 0;
	//	size_t R = mEdgePtrs.size();
	//	size_t m = (L + R) / 2;
	//	while (mEdgePtrs[m][0] != toFind)
	//	{
	//		m = (L + R) / 2;
	//		if (mEdgePtrs[m][0] < toFind)
	//		{
	//			L = m + 1;
	//		}
	//		else if (toFind < mEdgePtrs[m][0])
	//		{
	//			R = m - 1;
	//		}
	//		if (L > R)
	//		{
	//			std::cout << "Edge not found" << std::endl;
	//			return -1;
	//		}
	//	}
	//
	//	if (!careAboutAttributes)
	//	{
	//		return m;
	//	}
	//
	//	else
	//	{
	//		while (!(toFind < mEdgePtrs[m][0] || mEdgePtrs[m][0] < toFind) && m < mEdgePtrs.size())
	//		{
	//			if (mEdgePtrs[m][0] == toFind)
	//			{
	//				return m;
	//			}
	//			++m;
	//		}
	//		std::cout << "Edge not found " << std::endl;
	//		return -1;
	//	}		
	//}
	//
	////we will never care about attributes here 
	//int Skeleton::FindNextEdgePos(SkeletonEdge toFind)
	//{
	//	size_t L = 0;
	//	size_t R = mEdgePtrs.size() - 1;
	//	size_t m = (L + R) / 2;
	//	while (mEdgePtrs[m][0] != toFind)
	//	{
	//		m = (L + R) / 2;
	//		if (mEdgePtrs[m][0] < toFind)
	//		{
	//			L = m + 1;
	//		}
	//		else if (toFind < mEdgePtrs[m][0])
	//		{
	//			R = m - 1;
	//		}
	//		if (L > R)
	//		{
	//			if (toFind < mEdgePtrs[R][0])
	//			{
	//				return R;
	//			}
	//			else
	//			{
	//				return L;
	//			}
	//		}
	//	}
	//	return m ;
	//}
	//
	//edgePtrIter Skeleton::GetPosIter(int pos)
	//{
	//	return mEdgePtrs.begin() + pos;
	//}

	void Skeleton::AddEdge(SkeletonEdge toAdd)
	{
		mNeighbors[toAdd.v0].push_back(toAdd.v1);
		mNeighbors[toAdd.v1].push_back(toAdd.v0);
		for (int i = 0; i < mEdges[toAdd.v0].size(); ++i)
		{
			if ((toAdd.v1 < mEdges[toAdd.v0][i]->v1))
			{
				mEdges[toAdd.v0].insert(mEdges[toAdd.v0].begin() + i, new SkeletonEdge(toAdd));
				break;
			}
		}
	}


	void Skeleton::RemoveEdges(edgeList toRemove, bool careAboutAttributes)
	{
		for each(SkeletonEdge edge in toRemove)
		{
			RemoveEdge(edge, careAboutAttributes);
		}
	}

	void Skeleton::RemoveEdge(size_t v0, size_t v1)
	{
		RemoveEdge(SkeletonEdge(v0, v1), false);
	}

	void Skeleton::RemoveEdge(SkeletonEdge toRemove, bool careAboutAttributes)
	{
		size_t v0 = toRemove.v0;
		size_t v1 = toRemove.v1;
		for (std::vector<size_t>::iterator iter = mNeighbors[v0].begin(); iter < mNeighbors[v0].end(); ++iter)
		{
			if (iter[0] == v1)
			{
				mNeighbors[v0].erase(iter, iter + 1);
			}
		}
		for (std::vector<size_t>::iterator iter = mNeighbors[v1].begin(); iter < mNeighbors[v1].end(); ++iter)
		{
			if (iter[0] == v0)
			{
				mNeighbors[v1].erase(iter, iter + 1);
			}
			
		}

		bool edgeFound = false;
		if (careAboutAttributes)
		{
			for (int i = 0; i < mEdges[v0].size(); ++i)
			{
				if (mEdges[v0][i]->operator==(toRemove))
				{
					delete mEdges[v0][i];
					mEdges[v0].erase(mEdges[v0].begin() + i, mEdges[v0].begin() + i + 1);
					edgeFound = true;
				}
			}
		}
		else
		{
			for (int i = 0; i < mEdges[v0].size(); ++i)
			{
				if (mEdges[v0][i]->v1 == v1)
				{
					delete mEdges[v0][i];
					mEdges[v0].erase(mEdges[v0].begin() + i, mEdges[v0].begin() + i + 1);
					edgeFound = true;
				}
			}
		}
		if (edgeFound)
		{
			mNumEdges -= 1;
		}
	}

	Json::Value Skeleton::toJson()
	{
		Json::Value skelJson;
		skelJson["Vertices"] = Json::Value(Json::ValueType::arrayValue);
		skelJson["Vertices"].resize(mVerts.size());

		for (int i = 0; i < mVerts.size(); ++i)
		{
			skelJson["Vertices"][i] = mVerts[i].toJson();
		}

		skelJson["Edges"] = Json::Value(Json::ValueType::arrayValue);

		for (int i = 0; i < mEdges.size(); ++i)
		{
			for (int j = 0; j < mEdges[i].size(); ++i)
			{
				SkeletonEdge *skelEdge = mEdges[i][j];
				skelJson["Edges"].append(skelEdge->toJson());
			}
		}

		return skelJson;
	}


	edgePtrList tidyUpRoot(edgePtrList toTidy, bool &valid, size_t &end1, size_t &end2, SkeletonEdge *beginning)
	{
		std::sort(toTidy.begin(), toTidy.end(), edgePtrLessThan);

		std::map<size_t, std::vector<SkeletonEdge*>> vertEdges;
		for (int i = 0; i < toTidy.size(); ++i)
		{
			SkeletonEdge *edge = toTidy[0];
			if (!edge)
			{
				valid = false;
				return edgePtrList();
			}

			if (vertEdges.count(edge->v0) == 0)
			{
				vertEdges[edge->v0] = {edge};
			}
			else
			{
				vertEdges[edge->v0].push_back(edge);
			}
			if (vertEdges.count(edge->v1) == 0)
			{
				vertEdges[edge->v1] = { edge };
			}
			else
			{
				vertEdges[edge->v1].push_back(edge);
			}
		}

		std::vector<size_t> endpoints;
		for each(auto keyValPair in vertEdges)
		{
			if (keyValPair.second.size() == 1)
			{
				endpoints.push_back(keyValPair.first);
			}
			if (keyValPair.second.size() > 2)
			{
				valid = false;
				return edgePtrList();
			}
		}

		//if the number of endpoints (vertices degree 1 or less) is not identically 2, the root cannot be valid
		if (endpoints.size() != 2)
		{
			valid = false;
			return edgePtrList();
		}

		
		size_t nextVert = 0;

		if (beginning != nullptr)
		{
			if (endpoints[0] == beginning->v0 || endpoints[0] == beginning->v1)
			{
				nextVert = endpoints[0];
			}
			else if (endpoints[1] == beginning->v1 || endpoints[1] == beginning->v1)
			{
				nextVert = endpoints[1];
			}
			else
			{
				nextVert = std::min(endpoints[0], endpoints[1]);
			}
		}
		else
		{
			nextVert = std::min(endpoints[0], endpoints[1]);
		}

		end1 = nextVert;

		SkeletonEdge *priorEdge = nullptr;
		bool hasNext = true;
		
		edgePtrList result = edgePtrList();
		while (hasNext)
		{
			if (vertEdges[nextVert].size() == 1)
			{
				if (vertEdges[nextVert][0] != priorEdge)
				{
					priorEdge = vertEdges[nextVert][0];
					result.push_back(priorEdge);
				}
				else
				{
					hasNext = false;
				}
			}
			else
			{
				if (vertEdges[nextVert][0] != priorEdge)
				{
					priorEdge = vertEdges[nextVert][0];
					result.push_back(priorEdge);
				}
				else
				{
					priorEdge = vertEdges[nextVert][1];
					result.push_back(priorEdge);
				}
			}
			
			nextVert = priorEdge->v0 != nextVert ? priorEdge->v0 : priorEdge->v1;
		}

		end2 = nextVert;
		valid = true;
		return result;
	}
}