#include "Skeleton.h"


int main(int argc, char **argv)
{
	//assume if 1 argument that its the filename of the data
	if (argc == 2)
	{
		char * filename = argv[1];
		
		std::cout << std::endl << std::endl;
		std::cout << filename << std::endl;


		Roots::Skeleton skeleton(filename);

		Roots::vertList verts = skeleton.getVertices();
		Roots::edgeList edges = skeleton.getEdges();

	}
}