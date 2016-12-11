#include <list> //std::list
#include <vector> //std::vector
#include <random> //std::mt19937, std::uniform_real_distribution
#include <limits> //std::numeric_limits
#include <utility> //std::pair
#include <chrono> //std::chrono, ::system_clock, ::to_time_t, ::now
#include <istream> //std::istream
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits.h>

#include "Vertex.h"
#include "MinHeap.h"

using std::chrono::system_clock;

// INF
static const double MAX = std::numeric_limits<double>::max();

// Typedef PIRV
typedef std::pair<double, Vertex&> PIRV;

// Seed random number generator
std::mt19937 generator(system_clock::to_time_t(system_clock::now()));

class Graph {

	std::vector<Vertex> graphVector;

	int numVertex, numEdges;

	// Add an edge to a vertex
	void addEdge(int v, int b, double len)
	{
		graphVector[v].edges.emplace_back(len, graphVector[b]);
	} // https://goo.gl/JTXCjz

	void printGraph() {
		std::cout << "ID: weight, ID" << std::endl;
		for (int i = 0; i < numVertex; ++i) {
			std::list<PIRV>& edges = graphVector[i].edges;
			for (std::list<PIRV>::iterator it = edges.begin(); it != edges.end(); ++it) {
				std::cout << graphVector[i].ID << ": " << std::get<0>(*it) << ", " << std::get<1>(*it).ID << std::endl;
			}
		}
	}
	  
	void printArr(std::vector<double> &dist, int n)
	{
		printf("Vertex Distance from Source\n");
		for (int i = 0; i < n; ++i)
			std::cout << i << " \t\t " << dist[i] << std::endl;//printf("%d \t\t %d\n", i, dist[i]);
	}

	// Function to print shortest path from source to j
	// using parent array
	void printPath(std::vector<int> &parent, int j)
	{
		// Base Case : If j is source
		if (parent[j] == -1)
			return;

		printPath(parent, parent[j]);

		std::cout << j << " "; // printf("%d ", j);
	}

	// A utility function to print the constructed distance
	// array
	void printSolution(std::vector<double> &dist, int n, std::vector<int> &parent)
	{
		int src = 0;
		int d = 15; // d is distance between columns
		std::cout << std::setw(d) << "Vertex |" << std::setw(d) << "Distance | " << "Path";//printf("Vertex\t\t Distance\t\tPath");
		for (int i = 1; i < n; i++)
		{
			std::cout << std::endl << std::setw(d-7) << src << " -> " << i << " |" << std::setw(d-3) << dist[i] << " | " << src << " ";//printf("\n%d -> %d \t\t %d\t\t%d ", src, i, dist[i], src);
			printPath(parent, i);
		}
		std::cout << std::endl << std::endl;
	}
	


public:
	// Constructors
	// This constructor creates a randomly generated graph given:
	// density, number_of_vertex, and maximum edge length
	Graph(const double density, const double distance_range,
		const int numVert) : numVertex(numVert), numEdges(0) {

		// https://goo.gl/hvgQhk
		std::uniform_real_distribution<double> edge_exist(0, 1);
		std::uniform_real_distribution<double> edge_len(1, distance_range);

		graphVector.resize(numVertex, Vertex{ std::list<PIRV>(), INT_MAX, 0 });  // nullptr });

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

	// This constructor creates a graph from
	// an input file
	// Read from input file constructor
	/* format is:
	number of vertex

	vertex_i vertex_j cost_i_to_j
	vertex_i vertex_j cost_i_to_j
	...
	*/
	Graph(std::ifstream& inFile) : numEdges(0) {
		int i, j, cost, inc = 1, index = 0;
		//iss >> numVertex; // https://goo.gl/Sm62mC
		//std::cout << numVertex << std::endl;
		numVertex = 9;
		graphVector.resize(numVertex, Vertex{ std::list<PIRV>(), INT_MAX, 0 }); // https://goo.gl/tzNYaz

		//Assign a number to each vertex for unique identification
		for (auto &k : graphVector) k.ID = index++;

		/*
		std::cout << "wat" << std::endl;
		while (inFile >> i >> j >> cost) {
			std::cout << "i: " << i << " j: " << j << " cost: " << cost << std::endl;
			addEdge(i, j, cost);
			numEdges += (inc ^= 1);
		}*/
		std::string line;
		//std::getline(inFile, line);
		std::cout << line << std::endl;
		while (std::getline(inFile, line)) {
			std::istringstream iss(line);
			iss >> i >> j >> cost;
			addEdge(i, j, cost);
			numEdges += (inc ^= 1);/*
			if (iss >> i >> j >> cost) { 
				addEdge(i, j, cost);
				numEdges += (inc ^= 1);
			}*/
		}
	}

	void dijsktra(int src)
	{
		int V = numVertex;
		std::vector<double> dist; //int dist[V]; // dist = distances array
		std::vector<int> parent;

		dist.resize(V);
		parent.resize(V);
		parent[0] = -1;

		struct MinHeap* minHeap = createMinHeap(V);

		for (int v = 0; v < V; ++v) {
			dist[v] = INT_MAX;
			minHeap->array[v] = newMinHeapNode(v, dist[v]);
			minHeap->pos[v] = v;
		}

		minHeap->array[src] = newMinHeapNode(src, dist[src]);
		minHeap->pos[src]	= src;
		dist[src] = 0;
		decreaseKey(minHeap, src, dist[src]);

		minHeap->size = V;

		while (!isEmpty(minHeap)) {
			struct MinHeapNode* minHeapNode = extractMin(minHeap);
			int u = minHeapNode->v;
			//std::cout << u << std::endl; ==>DEBUG<==

			std::list<PIRV>& edges = graphVector[u].edges;
			
			for (std::list<PIRV>::iterator it = edges.begin(); it != edges.end(); ++it) {
				int v = std::get<1>(*it).ID;
				//std::cout << "v: " << v << " | u: " << u << std::endl; ==>DEBUG<==
				double w = std::get<0>(*it);
				/*std::cout << "w: " << w << " dist[" << u << "]: " << dist[u] << " dist[" << v << "]: " << dist[v] 
					<< " || w + dist[u] < dist[v]: " << (w + dist[u] < dist[v]) << std::endl;*/ // ==>DEBUG<==
				if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && w + dist[u] < dist[v]) {
					parent[v] = u;
					dist[v] = dist[u] + w;
					//std::cout << dist[v] << std::endl; ==>DEBUG<==

					decreaseKey(minHeap, v, dist[v]);
				}
			}
		}
		//for (int i = 0; i < V; ++i) std::cout << dist[i] << std::endl; ==>DEBUG<==
		//printGraph();
		//printArr(dist, V);
		printSolution(dist, V, parent);
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
		std::cout << "wat" << std::endl;
		Graph graph( dynamic_cast<std::istream &> (readFile) ); // chris
		graph.dijsktra(0);
	}
	readFile.close();*/
	
	
	std::ifstream readFile;
	readFile.open("input.txt", std::ios_base::in);
	/*
	std::string str;
	while (std::getline(readFile, str)) {
		std::cout << str << std::endl;
	}*/
	if (readFile.is_open()) {
		Graph graph(readFile); // http://stackoverflow.com/questions/1655912/problem-passing-in-istream-argument-to-a-class-constructor
		graph.dijsktra(0);
	}
	readFile.close();
	
	//std::cout << readFile << std::endl; // http://stackoverflow.com/questions/7868936/read-file-line-by-line
	//Graph graph(3, 5, 6);
	

	return 0;
}


// http://www.sanfoundry.com/cpp-program-implement-adjacency-list/
// https://ece.uwaterloo.ca/~cmoreno/ece250/2012-03-26--graphs-implementation.pdf
// https://github.com/sftrabbit/CppSamples-Samples

// We create a vector of Vertexes (graphVector) for each vertex