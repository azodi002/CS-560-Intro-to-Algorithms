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

HexagonTile::HexagonTile()
{
	cout << "Default Constructor called" << endl;
	m_file_name = "";
	m_number_of_nodes = 0;
}
// --------------------------------------

HexagonTile::HexagonTile(string file_name, 
		        		 long number_of_nodes)
{
	m_file_name 	  = file_name;
	m_number_of_nodes = number_of_nodes;

	// will open and fill tile map
	// with corresponding weights 
	bool is_open_success = openFile();

	if(!is_open_success)
	{
		cout << "Error, Failed to generate tile map or open file";
	}
}

// --------------------------------------

HexagonTile::~HexagonTile()
{
	cout << "Destructor called" << endl;
}

// --------------------------------------
bool
HexagonTile::openFile()
{
	// private function called in constructor
	ifstream input_file;
	input_file.open(m_file_name);

	if(input_file.bad())
	{
		cout << "Failed to open input file: " << m_file_name << endl;
		return ERROR;
	}

	fillTileMap(input_file);

	return NO_ERROR;
}

// --------------------------------------
void 
HexagonTile::fillTileMap(ifstream& input_file)
{
	// private function used in ::openFile()
	int index_position = 0;
	int weight_of_node = 0;

	// filler, and -1 as weight, so can't be accessed
	m_tile_map[0] = -1; 

	for(int i = 1; i <= NUM_NODES; i++)
	{
		input_file >> index_position;
		input_file >> weight_of_node;

		m_tile_map[index_position] = weight_of_node;
	}
}

// --------------------------------------
bool
HexagonTile::printTileMap()
{
	// continue to print so we can verify
	// that the tile map was correctly produced with
	// the corresponding weights for each index position

	for(int i = 1; i <= NUM_NODES; i++)
	{
		// subtract one since input file says index 1...
		cout << "Index:  " << i << " ";
		cout << "Weight: " << m_tile_map[i] << endl;
	}
	return NO_ERROR;
}

// --------------------------------------
void
HexagonTile::findShortestPath(const int start_index, 
							  const int end_index)
{
	string final_print_minimal_cost  = "MINIMAL-COST PATH COSTS:";
	int total_cost_for_shortest_path = 0; 
	vector<int> shortest_path_indexes;
	// our start index should be 8 
	// our end index should be 226
	// since we are finding the shortest
	// path from top right of hexagonal rectangle
	// to the bottom left of the hexagonal rectangle

	/******
	How we determine the hexagon's FREE & BLOCK
	B = BLOCK;	

	   Free
		___
  Free /   \ Free      
  Free |_B_| Free
  Free /   \ Free
  Free |___| Free
    	Free

	*******/

	// based off rectangular diagram in spec.
	// label the paths that the hexagons on each corner
	// of the hexagonal map can move
	int node_1[HEXAGON_SIDES]       	= {BLOCK, BLOCK, FREE, FREE, BLOCK, BLOCK};
	int node_8[HEXAGON_SIDES]       	= {BLOCK, BLOCK, BLOCK, FREE, FREE, BLOCK};
	int node_226[HEXAGON_SIDES]   		= {FREE, FREE, BLOCK, BLOCK, BLOCK, BLOCK};
	int node_233[HEXAGON_SIDES] 		= {FREE, BLOCK, BLOCK, BLOCK, BLOCK, FREE};
	int nodes_2_to_7[HEXAGON_SIDES] 	= {BLOCK, BLOCK, FREE, FREE, FREE, BLOCK};
	int nodes_9_to_15[HEXAGON_SIDES] 	= {BLOCK, FREE, FREE, FREE, FREE, FREE};
	int nodes_219_to_225[HEXAGON_SIDES] = {FREE, FREE, FREE, BLOCK, FREE, FREE};

	// increments of 15, (i.e 16, 31, 46 ...)
	int nodes_16_to_211[HEXAGON_SIDES] = {FREE, FREE, FREE, FREE, BLOCK, BLOCK};

	// increments of 23, (i.e 23, 38, 53 ...)
	int nodes_23_to_218[HEXAGON_SIDES] = {FREE, BLOCK, BLOCK, FREE, FREE, FREE};

	int nodes_227_to_232[HEXAGON_SIDES] = {FREE, FREE, FREE, BLOCK, FREE, FREE};

	// will contain nodes that don't exist in the arrays above
	int remaining_nodes[HEXAGON_SIDES] = {FREE, FREE, FREE, FREE, FREE, FREE};

	// defining moves clockwise starting from top
	// side, (i.e. look at node 17 and compute these values)
	// we know from this the nodes around hexagon 17 
	/** are
	2 10 25 32 24 9 
	**/
	int adjacent_moves[6] = {-15, -7, 8, 15, 7, -8};

	bool intree[NUM_NODES];
	int distance[NUM_NODES];
	int parent[NUM_NODES];

	for(int n = 0; n < NUM_NODES; n++)
	{
		intree[n]  = false;
		distance[n] = INT_MAX;
		parent[n] = -1;
	}


	bool is_node_one        = false;
	bool is_node_eight      = false;
	bool is_node_226        = false;
	bool is_node_233        = false;
	bool is_node_2_to_7     = false;
	bool is_node_9_to_15    = false;
	bool is_node_219_to_225 = false;
	bool is_node_16_to_211  = false;
	bool is_node_23_to_218  = false;
	bool is_node_227_to_232 = false;
	bool is_node_remaining  = false;

	vector<int> nodes_from_16_to_211 = {16, 31, 46, 61, 
								        76, 91, 106, 121, 
								        136, 151, 166, 181, 
								        196, 211};

	vector<int> nodes_from_23_to_218 = {23, 38, 53, 68, 
								        83, 98, 113, 128, 
								        143, 158, 173, 188, 
								        203, 218};								      

	// assigning weight of start index (in this case 8)
    // to distance start index
	distance[start_index] = m_tile_map[start_index]; 

	// this is essentially our hexagon index position
	int position = start_index; 
	int new_index_of_checked_hexagon = 0;

	// used in last for loop in while
	int temp_distance = 0;

	while(!intree[position])
	{
		intree[position] = true;

		is_node_one   = (position == 1);
		is_node_eight = (position == 8);
		is_node_226   = (position == 226);
		is_node_233   = (position == 233);

		is_node_2_to_7 = (position >= 2 && position <=7);
		is_node_9_to_15 = (position >= 9 && position <= 15);
		is_node_219_to_225 = (position >= 219 && position <=225);

		std::vector<int>::iterator node_16_to_211_iter;
		node_16_to_211_iter = find(nodes_from_16_to_211.begin(), nodes_from_16_to_211.end(), position);
		if(node_16_to_211_iter != nodes_from_16_to_211.end())
		{
			// found a value that exists
			// from 16 to 211 with increments of 15
			is_node_16_to_211 = true;
		}
		else
		{
			is_node_16_to_211 = false;
		}

		std::vector<int>::iterator node_23_to_218_iter;
		node_23_to_218_iter = find(nodes_from_23_to_218.begin(), nodes_from_23_to_218.end(), position);
		if(node_23_to_218_iter != nodes_from_23_to_218.end())
		{
			// found a value that exists
			// from 23 to 218 with increments of 15
			is_node_23_to_218 = true;
		}
		else
		{
			is_node_23_to_218 = false;
		}

		is_node_227_to_232 = (position >= 227 && position <=232);

		if(!is_node_one || !is_node_eight || !is_node_226 ||
		   !is_node_233 || !is_node_2_to_7 || !is_node_9_to_15 ||
		   !is_node_219_to_225 || !is_node_16_to_211 || !is_node_23_to_218 ||
		   !is_node_227_to_232)
		{
			// the position is a hexagon tile that has all sides
			// free or adjacent to another hexagon
			is_node_remaining = true;
		}
		else
		{
			is_node_remaining = false;
		}

		// FREE or BLOCK array
		bool is_side_free = false;

		for(int n = 0; n < 6; n++)
		{
			if(is_node_one)
				is_side_free = node_1[n];
			else if(is_node_eight)
				is_side_free = node_8[n];
			else if(is_node_226)
				is_side_free = node_226[n];
			else if(is_node_233)
				is_side_free = node_233[n];
			else if(is_node_16_to_211)
				is_side_free = nodes_16_to_211[n];
			else if(is_node_23_to_218)
				is_side_free = nodes_23_to_218[n];
			else if(is_node_227_to_232)
				is_side_free = nodes_227_to_232[n];
			else if(is_node_2_to_7)
				is_side_free = nodes_2_to_7[n];
			else if(is_node_9_to_15)
				is_side_free = nodes_9_to_15[n];
			else if(is_node_219_to_225)
				is_side_free = nodes_219_to_225[n];
			else if(is_node_remaining)
				is_side_free = true;
			else
			{
				cout << "error, not a node we recognize...." << endl;
				is_side_free = false;
			}

			// if the side is not free just continue
			if(!is_side_free)
				continue;

			// at this point we know the side is free
			// now check if the hexagon's weight of the 
			// new adjacent index is -1 or not
			int index_of_adjacent_position = position + adjacent_moves[n];
			new_index_of_checked_hexagon = index_of_adjacent_position;
			int weight_of_checked_hexagon = m_tile_map[new_index_of_checked_hexagon];
			
			// as defined in the spec. -1 is blocked
			if(weight_of_checked_hexagon != -1)
			{
				int distance_plus_weight = distance[position] + weight_of_checked_hexagon;
				if(distance[new_index_of_checked_hexagon] > distance_plus_weight)
				{
					distance[new_index_of_checked_hexagon] = distance_plus_weight;
					parent[new_index_of_checked_hexagon] = position;
				}
			}
		}//for(int n = 0)

		position = 1;
		temp_distance = INT_MAX;
		for(int n = 1; n <= NUM_NODES; n++)
		{
			if(!intree[n] && (temp_distance > distance[n]))
			{
				temp_distance = distance[n];
				// reset our position to the new index
				position = n;
			}
		} 
	} //while(!intree)

	// print out output_path
	ofstream output_file("output_file.txt");
	// Print out shortest_path_indexes
	total_cost_for_shortest_path = distance[end_index];
	output_file << final_print_minimal_cost << endl;
	output_file << total_cost_for_shortest_path << endl << endl;

	for(int n = end_index; n != start_index; n = parent[n])
	{
		shortest_path_indexes.push_back(n);
	}
	
	for(int i = 0; i < shortest_path_indexes.size(); i++)
	{
		output_file << shortest_path_indexes[i] << endl;
	}

	// print 226 since that is the ending
	output_file << "226" << endl;


}

// --------------------------------------









