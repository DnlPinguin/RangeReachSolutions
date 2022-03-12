#ifndef BOOSTTYPES_H
#define BOOSTTYPES_H
#include "../Structures/IntervalScheme.h"
#include "../Helper/clock.h"

#include <algorithm>
#include <thread>
#include <functional>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace std;
typedef float coordinates;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<coordinates, 2, bg::cs::cartesian> point;
typedef bg::model::point<coordinates, 3, bg::cs::cartesian> threeDimPoint;
typedef bg::model::segment<threeDimPoint> threeDimLine;
typedef bg::model::box<point> box;
typedef bg::model::box<threeDimPoint> plane;
// typedef pair<point, pair<IntervalScheme, int>> value;
typedef pair<point, int> SpatialNode;
typedef pair<box, pair<int, int>> SccWithMbr;

typedef pair<plane, int> cuboidStructure;
typedef bgi::rtree<cuboidStructure, bgi::linear<16>> rTreeCubes;

typedef pair<threeDimLine,int> lineStructureForrTree;
typedef pair<threeDimPoint,int> pointStructureForrTree;

typedef bgi::rtree<lineStructureForrTree, bgi::linear<16>> rTreeLines;
typedef bgi::rtree<pointStructureForrTree, bgi::linear<16>> rTreePoints;
typedef bgi::rtree<SccWithMbr, bgi::linear<16>> rTreeSccPlanes;
typedef bgi::rtree<SpatialNode, bgi::linear<16>> rTreePlanes;


const string ABSOLUTE_FILE_PATH = "C://Users//danie//OneDrive//Desktop//Universit�t//CurrentSemester//Bachelor Thesis//Code//BachelorThesis//data";

void printBox(box area);

struct hybridResult
{
	bool result;
	int objects_returned;
	hybridResult(bool _result, int _objects_returned)
	{
		result = _result;
		objects_returned = _objects_returned;
	}
};

struct socialFirstResult
{
	bool result;
	double time_social;
	double time_spatial;
	int reachable_nodes;
	int number_of_spatial_range_tests;
	
	socialFirstResult(){}
	
	socialFirstResult(bool _result, double _time_social, double _time_spatial, int _reachable_nodes, int _number_of_spatial_range_tests)
	{
		result = _result;
		time_social = _time_social;
		time_spatial = _time_spatial;
		reachable_nodes = _reachable_nodes;
		number_of_spatial_range_tests = _number_of_spatial_range_tests;
	}


	void print()
	{
		cout << "SocialFirst Result: \n";
		cout << this->result << "\t";
		cout << this->time_social << "\t";
		cout << this->reachable_nodes << "\t";
		cout << this->number_of_spatial_range_tests << "\t";
		cout << this->result << "\t";
	}
};

struct spatialFirstResult
{
	bool result;
	double time_spatial;
	double time_social;
	int nodes_inside_query_range;
	int number_of_reachability_tests;
	
	spatialFirstResult(){}

	spatialFirstResult(bool _result, double _time_spatial, double _time_social, int _nodes_inside_query_range, int _number_of_reachability_tests)
	{
		result = _result;
		time_spatial = _time_spatial;
		time_social = _time_social;
		nodes_inside_query_range = _nodes_inside_query_range;
		number_of_reachability_tests = _number_of_reachability_tests;
	}

	void print()
	{
		cout << "SocialFirst Result: \n";
		cout << this->result << "\t";
		cout << this->time_spatial << "\t";
		cout << this->time_social << "\t";
		cout << this->nodes_inside_query_range << "\t";
		cout << this->number_of_reachability_tests << "\t";
		cout << this->result << "\t";
	}
};


struct spatialMbrRelation 
{
	bool isMbr;
	vector<coordinates> spatialData;

	spatialMbrRelation() {
		isMbr = false;
		spatialData = {};
	}
	spatialMbrRelation(bool _isMBr, vector<coordinates> _spatialData)
	{
		isMbr = _isMBr;
		spatialData = _spatialData;
	}

	void print()
	{
		if (this->isMbr)
		{
			cout << "Is a strongly connected component \n";
		}
		else
		{ 
			cout << "Is not a strongly connected component \n";
		}

		cout << "Coordinates: ";
		for (coordinates i : this->spatialData)
		{
			cout << i << "\t";
		}
		cout << endl;
	}
};

struct queryParameter
{
	int queryNode;
	int nodeDegree;
	box spatialRegion;
	float spaceUsed;
	int cardinality;

	queryParameter(int _queryNode, int _degree, box _spatialRegion, float _spaceUsed, int _cardinality) {
		queryNode = _queryNode;
		spatialRegion = _spatialRegion;
		spaceUsed = _spaceUsed;
		nodeDegree = _degree;
		cardinality = _cardinality;
	}
};

#endif