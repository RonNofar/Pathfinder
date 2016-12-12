#pragma once
#include <list> //std::list
#include <vector> //std::vector
#include <random> //std::mt19937, std::uniform_real_distribution
#include <limits> //std::numeric_limits
#include <utility> //std::pair, ::get
#include <chrono> //std::chrono, ::system_clock, ::to_time_t, ::now
#include <iostream> //std::cout, ::endl
#include <fstream> // std::getline, ::ifstream
#include <sstream> // std::istringstream
#include <string> // std::string
#include <iomanip> // std::setw
#include <limits.h>  // INT_MAX

#include "Vertex.h"
//#include "MinHeap.h" // declaring here causes LNK2005 and LNK1169 errors for an unknown reason, must declare in Graph.cpp

class Graph {
private:
	std::vector<Vertex> graphVector;

	int numVertex, numEdges;

	void addEdge(int v, int b, double len);

	void printGraph();
	void printArr(std::vector<double> &dist, int n);
	void printPath(std::vector<int> &parent, int j);
	void printSolution(int src, std::vector<double> &dist, 
		int n, std::vector<int> &parent);

public:
	Graph(std::ifstream& inFile);
	Graph(const double, const double, const int);

	void dijsktra(int src);

	int numberOfVertex() const { return numVertex; };
	int numberOfEdges() const { return numEdges; };

	std::vector<double> dist;
	std::vector<int> parent;
};