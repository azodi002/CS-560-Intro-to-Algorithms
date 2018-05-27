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

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#define NO_ERROR true
#define ERROR false
#define NUM_NODES 233 // defined in spec. of project
#define HEXAGON_SIDES 6
#define BLOCK 0
#define FREE 1
using namespace std;

class HexagonTile
{

public:
	HexagonTile();
	HexagonTile(string file_name, 
		        long number_of_nodes);
	~HexagonTile();

	bool
	printTileMap();

	// will find shortest path from
	// index 8 to 226 in the hexagonal rectangle
	// and will output the following:
	/*
		[1]. The path (indexes) of the shortest path
	  	     (Example): 8 15 18 20 30 ... 226, etc.

		[2]. Followed by the total COST of the
		     shortest path. and the string MINIMAL-COST PATH COSTS.
	*/ 
	void
	findShortestPath(const int start_index,
					 const int end_index);

private:
	string m_file_name;
	long m_number_of_nodes;

	int m_tile_map[NUM_NODES + 1];

	bool 
	openFile(); // private called in constructor

	void 
	fillTileMap(ifstream& input_file);
};


