#pragma once

#include "Geometry.h"
#include "SkeletonEdge.h"
#include "Skeleton.h"

#include "json/json.h"
#include <map>

namespace Roots
{
	class MetaNode
	{
	public:
		static size_t maxId;
		MetaNode();
		MetaNode(Skeleton *srcSkeleton, size_t srcVert, size_t idx);

		Json::Value toJson();

		Skeleton *mSrcSkeleton;
		size_t mSrcVert;
		size_t mIdx;
		std::vector<size_t> mNeighbors;
	};

	class MetaEdge
	{
	public:
		static std::map<size_t, MetaNode> vertNodeMap;
		//set of indicators whether the vertex at each index in the skeleton vertex list is contained in a MetaEdge
		static std::vector<bool> vertsInMetaEdge;
		static size_t maxId;
		MetaEdge();
		MetaEdge(Skeleton *srcSkel, size_t startVertex, size_t secondVertex, size_t idx=0, size_t order=0, MetaEdge *parent=nullptr);
		MetaEdge(const MetaEdge &toCopy);

		Json::Value toJson();
		MetaNode* findOrCreateNode(size_t srcVert);


		void buildDescendants();

		//std::vector<FlatMetaEdge> flatten();

		//void AddToFlatTree(std::vector<FlatMetaEdge> &flattened);

		

	private:
		std::vector<size_t> mVertices;
		Skeleton *mSrcSkel;

		MetaNode *mParentNode, *mChildNode;

		MetaEdge *mParent;
		std::vector<MetaEdge*> mChildren;

		size_t mIdx;
		size_t mOrder;
		bool mPartOfLoop;

	public:
		std::vector<size_t> GetVertices() { return mVertices; }
		Skeleton* GetSkeleton() { return mSrcSkel; }
		MetaNode* GetParentNode() { return mParentNode; }
		MetaNode* GetChildNode() { return mChildNode; }
		std::vector<MetaEdge*> GetChildren() { return mChildren; }
		MetaEdge* GetParent() { return mParent; }
		size_t GetId() { return mIdx; }
		size_t GetOrder() { return mOrder; }
		bool isInLoop() { return mPartOfLoop; }

	};

	//class FlatMetaEdge
	//{
	//public:
	//	std::vector<size_t> mVertices;
	//	std::vector<size_t> mChildEdges;
	//	size_t mParentEdge;
	//	size_t mIdx;
	//	size_t mOrder;
	//	bool mPartOfLoop;
	//	FlatMetaEdge(MetaEdge src);
	//};
}