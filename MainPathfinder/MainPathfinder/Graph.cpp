#include "Graph.h"
#include "MinHeap.h"

using std::chrono::system_clock;

// Infinity
static const double MAX = std::numeric_limits<double>::max();

// Typedef PRV
typedef std::pair<double, Vertex&> PRV;

// Seed random number generator
std::mt19937 generator(system_clock::to_time_t(system_clock::now()));

#pragma region Constructors
// This constructor creates a graph from
// an input file
// Read from input file constructor
/* The format is:
	number of vertices
	vertex_i vertex_j cost_i_to_j
	vertex_i vertex_k cost_i_to_k
	etc.
	...
*/
Graph::Graph(std::ifstream& inFile) : numEdges(0) {
	int i, j, cost, inc = 1, index = 0;

	// Gets number of vertices by reading the first line in input.txt
	std::string line;
	std::getline(inFile, line);
	std::istringstream iss(line);
	iss >> numVertex;

	graphVector.resize(numVertex, Vertex{ std::list<PRV>(), INT_MAX, 0 });

	//Assign a number to each vertex for unique identification
	for (auto &k : graphVector) k.ID = index++;

	// Parse through file
	while (std::getline(inFile, line)) {
		std::istringstream iss(line);
		iss >> i >> j >> cost;
		addEdge(i, j, cost);
		numEdges += (inc ^= 1);
	}
}

// This constructor creates a randomly generated graph given:
// density, number_of_vertex, and maximum edge length

Graph::Graph(const double density, const double max_range,
	const int numVert) : numVertex(numVert), numEdges(0) {

	std::uniform_real_distribution<double> edge_exist(0, 1);
	std::uniform_real_distribution<double> edge_len(1, max_range);

	graphVector.resize(numVertex, Vertex{ std::list<PRV>(), INT_MAX, 0 });

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
#pragma endregion

#pragma region Functions
void Graph::dijsktra(int src)
{
	int V = numVertex;
	//std::vector<double> dist; // put outside scope to save for later
	//std::vector<int> parent;  // put outside scope to save for later

	dist.resize(V);
	parent.resize(V);
	parent[src] = -1;

	struct MinHeap* minHeap = createMinHeap(V);

	for (int v = 0; v < V; ++v) {
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}

	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	minHeap->size = V;

	while (!isEmpty(minHeap)) {
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;

		std::list<PRV>& edges = graphVector[u].edges;

		for (std::list<PRV>::iterator it = edges.begin(); it != edges.end(); ++it) {
			int v = std::get<1>(*it).ID;
			double w = std::get<0>(*it);
			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && w + dist[u] < dist[v]) {
				parent[v] = u;
				dist[v] = dist[u] + w;

				decreaseKey(minHeap, v, dist[v]);
			}
		}
	}
	//printGraph();
	//printArr(dist, V);
	printSolution(src, dist, V, parent);
}

// Add an edge to a specified vertex
void Graph::addEdge(int v, int b, double len)
{
	graphVector[v].edges.emplace_back(len, graphVector[b]);
}

// Function to print graph in an almost human-readable level
// Mainly used for debugging
void Graph::printGraph() {
	std::cout << "ID: weight, ID" << std::endl;
	for (int i = 0; i < numVertex; ++i) {
		std::list<PRV>& edges = graphVector[i].edges;
		for (std::list<PRV>::iterator it = edges.begin(); it != edges.end(); ++it) {
			std::cout << graphVector[i].ID << ": " << std::get<0>(*it)
				<< ", " << std::get<1>(*it).ID << std::endl;
		}
	}
}

void Graph::printArr(std::vector<double> &dist, int n)
{
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; ++i)
		std::cout << i << " \t\t " << dist[i] << std::endl;
}

// Function to print shortest path from source to j
// using parent array
void Graph::printPath(std::vector<int> &parent, int j)
{
	// Base Case : If j is source
	if (parent[j] == -1)
		return;

	printPath(parent, parent[j]);

	std::cout << j << " ";
}

// A utility function to print the constructed distance
// array
void Graph::printSolution(int src, std::vector<double> &dist,
	int n, std::vector<int> &parent)
{
	int d = 15; // d is distance between columns
	int v = 7; // for valid seperating
	std::cout << std::setfill(' ') << std::setw(d) << "Vertex |" 
		<< std::setw(d) << "Distance | " << "Path";
	for (int i = 0; i < n; i++)
	{
		if (i >= 10) v = 8;
		if (i >= 100) v = 9;
		if (i >= 1000) v = 10;
		std::cout << std::endl << std::setw(d - v) << src << " -> " << i 
			<< " |" << std::setw(d - 3) << dist[i] << " | " << src << " ";
		printPath(parent, i);
	}
	std::cout << std::endl << std::endl;
}


#pragma endregion