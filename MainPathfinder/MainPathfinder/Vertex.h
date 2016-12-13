#pragma once
#include <list>
#include <utility>

struct Vertex {

	// List of edges connected to current vertex
	std::list<std::pair<double, Vertex&> > edges;

	// Smallest distance from source to this node
	double distance;

	// unique id to use for indexing this vertex
	int ID;
};

// Used for priority queue sorting
template<bool reverse>
struct vertexSort {
	inline bool operator () (Vertex *lhs, Vertex *rhs) const
	{
		return reverse ? lhs->distance > rhs->distance : lhs->distance < rhs->distance;
	}
};

// Sorting helpers for a vertex object
struct vertexComp {
	inline int operator () (const Vertex& lhs, const Vertex& rhs) const
	{
		return (double)(lhs.distance - rhs.distance);
	}
};