#pragma once
#include "RootAttributes.h"
#include "Listener.h"
#include <set>
#include <string>

#include "json\json.h"

namespace Roots
{
	struct SkeletonEdge
	{
	public:
		
		size_t v0, v1;
		RootAttributes attributes;
		

		SkeletonEdge()
		{
			v0 = 0;
			v1 = 0;
			attributes = RootAttributes();
		}

		SkeletonEdge(size_t firstVert, size_t secondVert, RootAttributes aAttributes = RootAttributes())
		{
			attributes = aAttributes;
			if (firstVert < secondVert)
			{
				v0 = firstVert;
				v1 = secondVert;
			}
			else
			{
				v1 = firstVert;
				v0 = secondVert;
			}

			std::cout << "constructor verts " << v0 << " " << v1 << std::endl;
		}

		SkeletonEdge(size_t firstVert, size_t secondVert, double * attributeData)
		{
			attributes = RootAttributes(attributeData);
			if (firstVert < secondVert)
			{
				v0 = firstVert;
				v1 = secondVert;
			}
			else
			{
				v1 = firstVert;
				v0 = secondVert;
			}
			
			std::cout << "constructor verts " << v0 << " " << v1 << std::endl;
			
		}

		SkeletonEdge(const SkeletonEdge& other)
		{
			mListeners = std::set<Listener*>();
			if (other.v0 < other.v1)
			{
				v0 = other.v0;
				v1 = other.v1;
			}
			else
			{
				v1 = other.v0;
				v0 = other.v1;
			}

			attributes = other.attributes;
		}

		Json::Value toJson()
		{
			Json::Value edge = Json::Value(Json::ValueType::arrayValue);
			edge.resize(2 + RootAttributes::NumAttributes);
			edge[0] = v0;
			edge[1] = v1;
			for (int att = 0; att < RootAttributes::NumAttributes; ++att)
			{
				edge[att + 2] = attributes[att];
			}
			return edge;
		}

		SkeletonEdge(Json::Value jsonEdge)
		{
			v0 = jsonEdge[0].asInt();
			v1 = jsonEdge[1].asInt();

			int tempV;
			if (v0 > v1)
			{
				tempV = v1;
				v1 = v0;
				v0 = tempV;
			}

			double *attributeData = new double[RootAttributes::NumAttributes];

			for (int i = 0; i < RootAttributes::NumAttributes; ++i)
			{
				attributeData[i] = jsonEdge[i + 2].asDouble();
			}
			attributes = RootAttributes(attributeData);

			delete[] attributeData;

		}

		~SkeletonEdge()
		{
			
			for each(Listener *listener in mListeners)
			{
				std::cout << "Edge deleted and signal sent" << std::endl;
				if (listener != nullptr)
				{
					listener->SpeakerDestroyed((void*)this, ClassName());
				}
			}
			mListeners.clear();
		}

		static std::string ClassName()
		{
			return "__Skeleton_Edge__";
		}

		void RegisterListener(Listener *listener)
		{
			std::cout << "listener registerd" << std::endl;
			mListeners.insert(listener);
		}

		void RemoveListener(Listener *listener)
		{
			mListeners.erase(listener);
		}

		void update(RootAttributes updatedAttributes)
		{
			for each(Listener *listener in mListeners)
			{
				if (listener != nullptr)
				{
					listener->SpeakerUpdated((void*)this, ClassName());
				}
			}
		}

		bool operator<(SkeletonEdge &other)
		{
			return v0 < other.v0 ? true : (other.v0 < v0 ? false : (v1 < other.v1 ? true : false));
		}

		friend bool edgePtrLessThan(SkeletonEdge *first, SkeletonEdge *second)
		{
			return (*first) < (*second);
		}

		//this operator ignores the attribution information for convenience in the edge finding and sorting process
		bool operator==(SkeletonEdge &other)
		{
			return (v0 == other.v0) && (v1 == other.v1) && (attributes == other.attributes);
		}

		bool operator!=(SkeletonEdge &other)
		{
			return !this->operator==(other);
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

	private:
		std::set<Listener*> mListeners;
	};

}
