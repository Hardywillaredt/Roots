#include "MetaGraph.h"


namespace Roots
{
	MetaGraph::MetaGraph() 
	{ 
		mTopLevelEdges = {};
	}

	MetaGraph::MetaGraph(Skeleton skel, size_t rootNode, size_t nextNode)
	{
		mSkeleton = skel;
		mTopLevelEdges = {};
		size_t node1 = rootNode;
		size_t node2 = nextNode;
		bool hasNeighbor = false;
		if (node2 == node1)
		{
			if (mSkeleton.mNeighbors[node1].size() > 0)
			{
				node2 = mSkeleton.mNeighbors[node1][0];
				hasNeighbor = true;
			}
		}
		else
		{
			hasNeighbor = true;
		}

		MetaEdge *root;

		if (hasNeighbor)
		{
			root = new MetaEdge(&mSkeleton, node1, node2, MetaEdge::maxId + 1);
			mTopLevelEdges.push_back(root);
		}
		else
		{
			mIsolatedNodes.push_back(MetaNode(&mSkeleton, node1, ++MetaNode::maxId));
		}

		for (int i = 0; i < MetaEdge::vertsInMetaEdge.size(); ++i)
		{
			if (!MetaEdge::vertsInMetaEdge[i])
			{
				size_t node1 = i;
				hasNeighbor = false;
				if (mSkeleton.mNeighbors[node1].size() > 0)
				{
					node2 = mSkeleton.mNeighbors[node1][0];
					hasNeighbor = true;
				}
				
				if (hasNeighbor)
				{
					root = new MetaEdge(&mSkeleton, node1, node2, MetaEdge::maxId + 1);
					mTopLevelEdges.push_back(root);
				}
				else
				{
					mIsolatedNodes.push_back(MetaNode(&mSkeleton, node1, ++MetaNode::maxId));
				}
			}
		}
	}

	Json::Value MetaGraph::toJson()
	{
		Json::Value metaJson;
		
		metaJson["Skeleton"] = mSkeleton.toJson();
		metaJson["MetaNodes"] = Json::Value(Json::ValueType::arrayValue);


		return metaJson;
	}
}