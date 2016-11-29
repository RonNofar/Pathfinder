#include <list> //std::list
#include <vector> //std::vector
#include <random> //std::mt19937, std::uniform_real_distribution
#include <limits> //std::numeric_limits
#include <utility> //std::pair
#include <chrono> //std::chrono, ::system_clock, ::to_time_t, ::now
#include <istream> //std::istream

using std::chrono::system_clock;

//Seed random number generator
std::mt19937 generator(system_clock::to_time_t(system_clock::now()));

//Sorting helpers for a vertex object
struct vertexComp {
	inline int operator () (const Vertex& lhs, const Vertex& rhs) const 
	{ return (lhs.distance - rhs.distance); }
};

//Used for priority queue sorting
template<bool reverse>
struct vertexSort {
	inline bool operator () ( Vertex *lhs, Vertex *rhs) const 
	{ return reverse ? lhs->distance > rhs->distance : lhs->distance < rhs->distance; }
};

class Graph {

	std::vector<Vertex> graphVector;
	
	int numVertex, numEdges;
	
	//Add an edge to a vertex
	void addEdge(int v, int b, double len) 
	{ graphVector[v].edges.emplace_back(len, graphVector[b]); }
	
public:
	//Constructors
	//This constructor creates a randomly generated graph given:
	//density, number_of_vertex, and maximum edge length
	Graph(const double density, const double distance_range, 
		const int numVert): numVertex(numVert), numEdges(0) {
		
		std::uniform_real_distribution<double> edge_exist(0, 1);
		std::uniform_real_distribution<double> edge_len(1, distance_range);
		
		graphVector.resize(numVertex, Vertex{ std::list<PIRV>(), KMAX, 0});  // nullptr });

		for (int s = 0; s < numVertex; ++s) {
			graphVector[s].ID = s;
			for (int k = 0; k < s; ++k )
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
		iss >> numVertex;
		graphVector.resize(numVertex, Vertex{ std::list<PIRV>(), KMAX, 0});
		
		//Assign a number to each vertex for unique identification
		for (auto &k: graphVector) k.ID = index++;
		
		while(iss >> i >> j >> cost) {
			addEdge(i, j, cost);
			numEdges += (inc ^= 1);
		}
	}
	
	//INF
	static constexpr double KMAX = std::numeric_limits<double>::infinity();
	
	int numberOfVertex() const  { return numVertex; }
	int numberOfEdges() const {return numEdges;}
	
	typedef std::pair<double, Vertex&> PIRV;
	
};
