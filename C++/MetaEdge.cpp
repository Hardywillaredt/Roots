#include "MetaEdge.h"
#include <algorithm>

std::map<size_t, Roots::MetaNode> Roots::MetaEdge::vertNodeMap = std::map<size_t, Roots::MetaNode>();
std::vector<bool> Roots::MetaEdge::vertsInMetaEdge = std::vector<bool>();
size_t Roots::MetaEdge::maxId = -1;

size_t Roots::MetaNode::maxId = -1;
namespace Roots
{

	MetaNode::MetaNode()
	{
		mSrcSkeleton = nullptr;
		mIdx = 0;
		mSrcVert = 0;
	}

	MetaNode::MetaNode(Skeleton *srcSkeleton, size_t srcVert, size_t idx)
	{
		mSrcSkeleton = srcSkeleton;
		mSrcVert = srcVert;
		mIdx = idx;
	}
	
	Json::Value MetaNode::toJson()
	{
		return Json::Value(mSrcVert);
	}

	MetaEdge::MetaEdge()
	{
		mVertices = {};
		mSrcSkel = nullptr;
		mParentNode = nullptr;
		mChildNode = nullptr;
		mParent = nullptr;
		mChildren = {};
		mIdx = 0;
		mOrder = 0;
	}

	MetaEdge::MetaEdge(Skeleton *srcSkel, size_t startVertex, size_t secondVertex, size_t idx, size_t order, MetaEdge *parent)
	{
		mVertices = {};
		mSrcSkel = srcSkel;
		mParentNode = findOrCreateNode(startVertex);
		mParent = parent;
		mChildren = {};
		mIdx = idx;
		mOrder = order;
		
		maxId = std::max(mIdx, maxId);
		if (idx == 0)
		{
			vertsInMetaEdge = std::vector<bool>(mSrcSkel->mNumVertices, false);
		}
		
		
		size_t vPrev = startVertex;
		size_t vNext = secondVertex;

		
		mVertices.push_back(vPrev);

		while (mSrcSkel->mNeighbors[vNext].size() == 2)
		{
			mVertices.push_back(vNext);

			for (int i = 0; i < 2; ++i)
			{
				if (mSrcSkel->mNeighbors[vNext][i] != vPrev)
				{
					vPrev = vNext;
					vNext = mSrcSkel->mNeighbors[vNext][i];
				}
			}
		}

		mVertices.push_back(vNext);

		for each(size_t vertId in mVertices)
		{
			vertsInMetaEdge[vertId] = true;
		}

		mChildNode = findOrCreateNode(vNext);

		mChildNode->mNeighbors.push_back(mParentNode->mIdx);
		mParentNode->mNeighbors.push_back(mChildNode->mIdx);
	}

	MetaEdge::MetaEdge(const MetaEdge &toCopy)
	{
		mOrder = toCopy.mOrder;
		mIdx = toCopy.mIdx;
		mVertices = toCopy.mVertices;
		mSrcSkel = toCopy.mSrcSkel;
	}


	Json::Value MetaEdge::toJson()
	{
		Json::Value edgeJson;
	/*	edgeJson["OrderedVertices"] = Json::Value(Json::ValueType::arrayValue);
		edgeJson["OrderedVertices"].resize(mEdges.size());

		for (int i = 0; i < mEdges.size(); ++i)
		{
			edgeJson["OrderedVertices"][i] = mEdges[i].toJson();
		}
*/
		return edgeJson;
	}

	MetaNode* MetaEdge::findOrCreateNode(size_t srcVert)
	{
		//if the meta node isn't mapped, create it
		if (vertNodeMap.count(srcVert) == 0)
		{
			MetaNode node = MetaNode(mSrcSkel, srcVert, ++MetaNode::maxId);
			vertNodeMap[srcVert] = node;
		}

		return &(vertNodeMap[srcVert]);
	}


	void MetaEdge::buildDescendants()
	{
		size_t secondLastVert = mVertices[mVertices.size() - 2];

		for each(size_t endNeighbor in mSrcSkel->mNeighbors[mChildNode->mSrcVert])
		{
			//if the provided vertex is not already in a meta edge, add it to the list of children
			if (!vertsInMetaEdge[endNeighbor])
			{
				MetaEdge *child = new MetaEdge(mSrcSkel, mChildNode->mSrcVert, endNeighbor, ++maxId, mOrder + 1, this);
				child->buildDescendants();
				mChildren.push_back(child);
			}
			else
			{
				if (endNeighbor != secondLastVert)
				{
					std::cout << "Loop found" << std::endl;
					mPartOfLoop = true;
				}
			}
		}

		if (mOrder == 0)
		{
			size_t secondVert = mVertices[1];
			for each(size_t neighborVert in mSrcSkel->mNeighbors[mParentNode->mSrcVert])
			{
				if (!vertsInMetaEdge[neighborVert])
				{
					MetaEdge *child = new MetaEdge(mSrcSkel, mParentNode->mSrcVert, neighborVert, ++maxId, mOrder + 1, this);
					child->buildDescendants();
					mChildren.push_back(child);
				}
				else
				{
					if (neighborVert != secondVert)
					{
						std::cout << "Loop found" << std::endl;
						mPartOfLoop = true;
					}
				}
			}
		}
	}

	//std::vector<FlatMetaEdge> MetaEdge::flatten()
	//{
	//	if (mOrder != 0)
	//	{
	//		return mParent->flatten();
	//	}
	//	else
	//	{
	//		std::vector<FlatMetaEdge> result = {};
	//		AddToFlatTree(result);
	//		return result;
	//	}
	//}

	//void MetaEdge::AddToFlatTree(std::vector<FlatMetaEdge> &flatTree)
	//{
	//	flatTree.push_back(FlatMetaEdge(*this));
	//	for each(MetaEdge *child in mChildren)
	//	{
	//		child->AddToFlatTree(flatTree);
	//	}
	//}


	//FlatMetaEdge::FlatMetaEdge(MetaEdge src)
	//{
	//	mVertices = src.GetVertices();
	//	mChildEdges = {};

	//	for (int i = 0; i < src.GetChildren().size(); ++i)
	//	{
	//		mChildEdges.push_back(src.GetChildren()[i]->GetId());
	//	}
	//	mParentEdge = src.GetParent()->GetId();
	//	mIdx = src.GetId();
	//	mOrder = src.GetOrder();
	//	mPartOfLoop = src.isInLoop();

	//}
}