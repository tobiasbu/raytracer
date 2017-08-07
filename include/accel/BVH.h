
#ifndef _BVH_H_
#define _BVH_H_

//#include "systeminfo.h"
#include "math\bounds.h"
#include "entity\scene.h"
#include "entity\primitive.h"


#include "math\mathdef.hpp"
#include <atomic>


class Mesh;

// \struct BVHEntry
// holds object pointer, aabb, and index at objects scene array
// this is specifc for space partitioning BVH
struct BVHEntry
{
	Bounds aabb;
	//Primitive * obj;
	unsigned int index;

	BVHEntry::BVHEntry(const Bounds & aabb, unsigned int index);


};

struct BVHBuildNode
{


	Bounds aabb;
	//BVHNode * parent;
	BVHBuildNode * childLeft;
	BVHBuildNode * childRight;
	uint32_t firstOffset;
	uint32_t nPrimitives;
	uint32_t splitAxis;
	int depth;


	BVHBuildNode();

	void makeChilds(const uint32_t & splitAxis, BVHBuildNode * childA, BVHBuildNode * childB);
	void makeLeaf(uint32_t first, uint32_t nPrimitives, const Bounds & bounds);


};

struct BVHFlatNode
{
	Bounds aabb;

	union {
		uint32_t objectOffset;    // leaf
		uint32_t secondChildOffset;   // childs
	};

	uint8_t nPrimitives;  // 0 > childs nodes
	uint8_t axis;         // interior node: xyz
	uint8_t pad[2];       // ensure 32 byte total size
};

struct SAHEntry
{
	Bounds aabb;
	int count = 0;
};

class BVH 
{
public:

	enum SplitMethod 
	{
		Middle, // centroid
		Median, 
		SAH,

	};

private:


	
	SplitMethod _splitMethod;
	//Scene * scene;
	BVHFlatNode * nodes = NULL;
	std::vector<Primitive*> _objects;

	int32_t _maxPrimitivesInNode = 4;
	uint32_t totalNodes = 0;
	uint32_t leafNodes;
	uint32_t childNodes;
	uint32_t objectsPerNodes;

	std::atomic<uint32_t> _aabbTests;
	std::atomic<uint32_t> _triangleTests;
	std::atomic<uint32_t> _intersectionTests;

	BVHBuildNode * recursiveBVHBuild(int32_t start, int32_t end, uint32_t & totalNodes, std::vector<BVHEntry> & entries, std::vector<Primitive*> & ordered);
	uint32_t flattenBVH(BVHBuildNode *node, uint32_t * offset);

	void calculateSAHCosts(const SAHEntry * sahEntries, const float & area, const int & nEntries, float * costs, float & minCost, int & minCostSplitIndex);
	

public:

	BVH() { }
	BVH(Scene * scene, const SplitMethod & split, const int32_t & maxPrimitivesInNode = 1);
	BVH(Mesh * mesh, const SplitMethod & split, const int32_t & maxPrimitivesInNode = 1);
	~BVH();

	bool set(Scene * scene, const SplitMethod & split, const int32_t & maxPrimitivesInNode = 1);
	bool build();
	bool intersects(const Ray &ray, RaycastHit & hit);

	const uint32_t & getAABBTests() const
	{
		return _aabbTests;
	}

	const uint32_t & getIntersectionTests() const
	{
		return _intersectionTests;
	}

	const uint32_t & getTriangleTests() const
	{
		return _triangleTests;
	}

	void clear()
	{
		

		if (nodes || nodes != NULL || nodes != nullptr) 
		{
			//for (int i = 0; i < totalNodes; i++)
				//delete &nodes[i];

			delete nodes;
		}

		nodes = NULL;
		_objects.clear();

		 _aabbTests = 0;
		_triangleTests = 0;
		_intersectionTests = 0;
	}
};

#endif