#include <list> //std::list
#include <vector> //std::vector
#include <random> //std::mt19937, std::uniform_real_distribution
#include <limits> //std::numeric_limits
#include <utility> //std::pair
#include <chrono> //std::chrono, ::system_clock, ::to_time_t, ::now
#include <istream> //std::istream
#include <iostream>
#include <fstream>

using std::chrono::system_clock;

#include <list> //std::list
#include <utility> //std::pair

//INF
static const double KMAX = std::numeric_limits<double>::max();



struct Vertex {

	//List of edges connected to current vertex
	std::list<std::pair<double, Vertex&> > edges;

	//Smallest distance from source to this node
	double distance;

	//unique id to use for indexing this vertex
	int ID;
};

typedef std::pair<double, Vertex&> PIRV;


//Seed random number generator
std::mt19937 generator(system_clock::to_time_t(system_clock::now()));

//Sorting helpers for a vertex object
struct vertexComp {
	inline int operator () (const Vertex& lhs, const Vertex& rhs) const
	{
		return (double)(lhs.distance - rhs.distance);
	}
};

//Used for priority queue sorting
template<bool reverse>
struct vertexSort {
	inline bool operator () (Vertex *lhs, Vertex *rhs) const
	{
		return reverse ? lhs->distance > rhs->distance : lhs->distance < rhs->distance;
	}
};

class Graph {

	std::vector<Vertex> graphVector;

	int numVertex, numEdges;

	//Add an edge to a vertex
	void addEdge(int v, int b, double len)
	{
		graphVector[v].edges.emplace_back(len, graphVector[b]);
	} // https://goo.gl/JTXCjz

public:
	//Constructors
	//This constructor creates a randomly generated graph given:
	//density, number_of_vertex, and maximum edge length
	Graph(const double density, const double distance_range,
		const int numVert) : numVertex(numVert), numEdges(0) {

		// https://goo.gl/hvgQhk
		std::uniform_real_distribution<double> edge_exist(0, 1);
		std::uniform_real_distribution<double> edge_len(1, distance_range);

		graphVector.resize(numVertex, Vertex{ std::list<PIRV>(), KMAX, 0 });  // nullptr });

		for (int s = 0; s < numVertex; ++s) {
			graphVector[s].ID = s;
			for (int k = 0; k < s; ++k)
			if (edge_exist(generator) < density) {
				double edgeLength = edge_len(generator);
				++numEdges;
				addEdge(k, s, edgeLength);
				addEdge(s, k, edgeLength);
			}
		}
	}

	//This constructor creates a graph from
	//an input file
	//Read from input file constructor
	/* format is:
	number of vertex

	vertex_i vertex_j cost_i_to_j
	vertex_i vertex_j cost_i_to_j
	...
	*/
	Graph(std::istream &iss) : numEdges(0) {
		int i, j, cost, inc = 1, index = 0;
		iss >> numVertex; // https://goo.gl/Sm62mC
		//std::cout << numVertex << std::endl;
		graphVector.resize(numVertex, Vertex{ std::list<PIRV>(), KMAX, 0 }); // https://goo.gl/tzNYaz

		//Assign a number to each vertex for unique identification
		for (auto &k : graphVector) k.ID = index++;

		while (iss >> i >> j >> cost) {
			addEdge(i, j, cost);
			numEdges += (inc ^= 1);
		}
	}

	

	int numberOfVertex() const  { return numVertex; }
	int numberOfEdges() const { return numEdges; }


};

int main() {
	/*
	std::ifstream readFile ("input.txt", std::ifstream::in);
	//readFile.open("input.txt", std::ifstream::in); // http://stackoverflow.com/questions/14548962/passing-istream-intro-a-function
	
	char c = readFile.get();

	while (readFile.good()) {
		std::cout << c;
		c = readFile.get();
	}*/
	/*
	std::filebuf fb;
	std::istream is(&fb);
	if (fb.open("input.txt", std::ios::in)) {
		std::istream is(&fb);
		while (is) std::cout << char(is.get());
		fb.close();
	}*/


	/*
	std::ifstream readFile;
	readFile.open("input.txt", std::ios_base::in); // http://stackoverflow.com/questions/27501160/pass-a-reference-to-stdifstream-as-parameter
	if (readFile.is_open()) {
		Graph graph( dynamic_cast<std::istream &> (readFile) ); // chris
	}
	readFile.close();*/
	

	std::ifstream readFile("input.txt");
	Graph graph(dynamic_cast<std::istream &> (readFile)); // http://stackoverflow.com/questions/1655912/problem-passing-in-istream-argument-to-a-class-constructor

	//Graph(&is);


	return 0;
}


// http://www.sanfoundry.com/cpp-program-implement-adjacency-list/
// https://ece.uwaterloo.ca/~cmoreno/ece250/2012-03-26--graphs-implementation.pdf
// https://github.com/sftrabbit/CppSamples-Samples

// We create a vector of Vertexes (graphVector) for each vertex