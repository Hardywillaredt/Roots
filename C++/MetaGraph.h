#pragma once

#include "MetaEdge.h"
#include "Skeleton.h"
#include "SkeletonEdge.h"


namespace Roots
{

	class MetaGraph
	{
	public:
		MetaGraph();
		MetaGraph(Skeleton skel, size_t rootNode = 0, size_t nextNode = 0);
		
		Json::Value toJson();

	private:
		Skeleton mSkeleton;
		std::vector<MetaEdge*> mTopLevelEdges;
		std::vector<MetaNode> mIsolatedNodes;

	};

}