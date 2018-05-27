/*******************************
Names:
[1]. Omid Azodi
[2]. Jorge Chavez

Group #: Number 1

Project: Hexagonal Shortest Path

Class: CS560 (Algorithms & Their Analysis)

Professor: Professor Root

Language: C++

How to compile: 
[1]. g++ HexagonTile.cpp HexagonRun.cpp -std=c++11 -o project.out
or 
[2]. g++ *.cpp -std=c++11

How to run:
[1]. ./project.out
or 
[2]. ./a.out
*******************************/


#include "HexagonTile.h"

#define START_INDEX 8
#define END_INDEX 226

int main(int argc, char* argv[])
{
	HexagonTile hexagon_tile("INPUT.txt",
							 NUM_NODES);	

// debugging statement to check what is in tile map
#if 0
	bool is_print_success = hexagon_tile.printTileMap();	

	if(!is_print_success)
	{
		cout << "Failed to show populated tile map" << endl;
	}
#endif
	// need to determine shortest path from 8 to 226
	// from top right of hexagonal rectangle to 
	// bottom left of hexagonal rectangle 

	hexagon_tile.findShortestPath(END_INDEX,
							      START_INDEX);


	return 0;
}


