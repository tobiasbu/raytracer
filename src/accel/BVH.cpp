
#include "accel/BVH.h"


#include <algorithm>
#include <chrono>
#include "others/log.h"
#include "render/mesh.h"


struct CompareToMid {
	CompareToMid(int d, float m) { dim = d; mid = m; }
	int dim;
	float mid;
	bool operator()(const BVHEntry &a) const {
		return a.aabb.centroid()[dim] < mid;
	}
};

struct ComparePoints {
	ComparePoints(int d) { dim = d; }
	int dim;
	bool operator()(const BVHEntry &a,
		const BVHEntry &b) const {
		return a.aabb.centroid()[dim] < b.aabb.centroid()[dim];
	}
};

BVHEntry::BVHEntry(const Bounds & aabb, unsigned int index)
	:
	//obj(obj),
	aabb(aabb),
	index(index)
{

}

BVHBuildNode::BVHBuildNode()
	:
	//parent(nullptr),
	childRight(nullptr),
	childLeft(nullptr),
	//object(nullptr),
	depth(0)
{

}



void BVHBuildNode::makeChilds(const uint32_t & splitAxis, BVHBuildNode * childA, BVHBuildNode * childB)
{
	childLeft = childA;
	childRight = childB;
	aabb = Bounds::merge(childA->aabb, childB->aabb);
	this->splitAxis = splitAxis;
	nPrimitives = 0;
}

void BVHBuildNode::makeLeaf(uint32_t first, uint32_t nPrimitives, const Bounds & bounds)
{
	this->nPrimitives = nPrimitives;
	firstOffset = first;
	aabb = bounds;
	childLeft = nullptr;
	childRight = nullptr;
}

BVH::BVH(Mesh * mesh, const SplitMethod & split, const int32_t & maxPrimitivesInNode)
{
	_objects = mesh->getTrianglesPrimitives();
	_splitMethod = split;
	_maxPrimitivesInNode = maxPrimitivesInNode;
	childNodes = 0;
	leafNodes = 0;
	_aabbTests = 0;
	_triangleTests = 0;
	_intersectionTests = 0;
}

BVH::BVH(Scene * scene, const SplitMethod & split, const int32_t & maxPrimitivesInNode)
{
	//this->scene = scene;

	set(scene,split,maxPrimitivesInNode);

	/*_objects = scene->getPrimitives();
	_splitMethod = split;

	_maxPrimitivesInNode = std::min(maxPrimitivesInNode, 255);
	childNodes = 0;
	leafNodes = 0;
	_aabbTests = 0;
	_triangleTests = 0;
	_intersectionTests = 0;*/

	// verify the scene extents
	/*if (!scene->getPrimitives().empty())
	{
	tree = new BVHNode();

	std::list<const Bounds &> aabbs_list;
	// for each object
	for (Primitive * obj : scene->getPrimitives())
	{

	const Bounds & aabb = obj->getBounds();

	aabbs_list.push_back(aabb);
	/*BVHNode * node = new BVHNode(0, tree, obj, aabb);
	tree->insert(node);*


	// scene extents
	// approach #1 - merge aabb tree - BAD
	tree->aabb.merge(aabb);
	// approach #2 - plane normal

	// approach #3 - frustrum
	}



	// SAH OPTIMIZATION 1
	float SAH = fInfinity;

	// sort aabbs by aabb.max
	//aabbs_list.sort(sortAABBMax);




	}	*/
}

bool BVH::set(Scene * scene, const SplitMethod & split, const int32_t & maxPrimitivesInNode)
{
	//this->scene = scene;
	//_objects.clear();
	_objects = scene->getPrimitives();
	_splitMethod = split;

	_maxPrimitivesInNode = std::min(maxPrimitivesInNode, 255);
	childNodes = 0;
	leafNodes = 0;
	_aabbTests = 0;
	_triangleTests = 0;
	_intersectionTests = 0;

	return true;
}

BVH::~BVH()
{
	
}

bool BVH::build()
{

	//_root = new BVHNode();

	if (_objects.empty())
	{
		nodes = NULL;
		return false;
	}
	auto timeStart = std::chrono::high_resolution_clock::now();

	std::vector<BVHEntry> entries;
	entries.reserve(_objects.size());

	for (int i = 0; i < _objects.size(); i++)
	{
		const Bounds & aabb = _objects[i]->getBounds();
		entries.push_back(BVHEntry(aabb, i));
	}


	std::vector<Primitive*> orderedPrimitives;
	orderedPrimitives.reserve(_objects.size());

	// merge aabb tree
	//_root->aabb.merge(aabb);

	totalNodes = 0;
	BVHBuildNode * root = recursiveBVHBuild(0, _objects.size(), totalNodes, entries, orderedPrimitives);

	_objects.swap(orderedPrimitives);

	nodes = new BVHFlatNode[totalNodes];
	for (uint32_t i = 0; i < totalNodes; ++i)
		new (&nodes[i]) BVHFlatNode;

	uint32_t offset = 0;
	flattenBVH(root, &offset);

	
	auto timeEnd = std::chrono::high_resolution_clock::now();
	auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
	

	Debug::log("--- BVH:\n", totalNodes);
	Debug::log("Construction Time: %.4f (seconds)\n", passedTime / 1000);
	Debug::log("Split method: %d | Max Primitves in Node: %d\n", _splitMethod,_maxPrimitivesInNode);
	Debug::log("Total nodes: %d | Childs: %d | Leafs: %d\n", totalNodes, childNodes, leafNodes);
	//Debug::log("Child nodes: %d\n", childNodes);
	//Debug::log("Leaf nodes: %d\n", leafNodes);


	return true;
}

BVHBuildNode * BVH::recursiveBVHBuild(int32_t start, int32_t end, uint32_t & totalNodes, std::vector<BVHEntry> & entries, std::vector<Primitive*> & ordered)
{

	

	BVHBuildNode * node = new BVHBuildNode();
	totalNodes++;

	// node bounds
	Bounds aabb;

	for (uint32_t i = start; i < end; ++i)
		aabb = Bounds::merge(aabb, entries[i].aabb);

	uint32_t nPrimitives = end - start;

	if (nPrimitives == 1)
	{
		// create a leaf
		uint32_t firstPrimOffset = ordered.size();

		for (uint32_t i = start; i < end; ++i)
		{
			uint32_t objectIndex = entries[i].index;
			ordered.push_back(_objects[objectIndex]);
		}

		leafNodes++;
		node->makeLeaf(firstPrimOffset, nPrimitives, aabb);

		return node;
	}
	else
	{
		// Compute bound of primitive centroids, choose split dimension _dim_
		Bounds centroidBounds;

		for (uint32_t i = start; i < end; ++i)
			centroidBounds = Bounds::merge(centroidBounds, entries[i].aabb.centroid());

		// split axis
		int dim = centroidBounds.getMaxAxis();

		// partition objects into two sets and build children
		uint32_t middle = (start + end) / 2;

		if (centroidBounds.getMax()[dim] == centroidBounds.getMin()[dim])
		{
			// If nPrimitives is no greater than maxPrimsInNode,
			// then all the nodes can be stored in a compact bvh node.
			//if (nPrimitives <= maxPrimsInNode)
			//{
			uint32_t firstPrimOffset = ordered.size();

			for (uint32_t i = start; i < end; ++i)
			{
				uint32_t objectIndex = entries[i].index;
				ordered.push_back(_objects[objectIndex]);
			}

			leafNodes++;
			node->makeLeaf(firstPrimOffset, nPrimitives, aabb);

			return node;
			//}
			/*else
			{
			// else if nPrimitives is greater than maxPrimsInNode, we
			// need to split it further to guarantee each node contains
			// no more than maxPrimsInNode primitives.
			node->makeChilds(dim,
			recursiveBVHBuild(start, middle, totalNodes, entries, ordered),
			recursiveBVHBuild(middle, end, totalNodes, entries, ordered)
			);

			return node;
			}*/

		}
		else
		{

			// Partition primitives based on splitMethod
			switch (_splitMethod)
			{

			case SplitMethod::Middle:
			{
				// Partition primitives through node's midpoint
				float pmid = (centroidBounds.getMin()[dim] + centroidBounds.getMax()[dim]) / 2.0f;

				// Reorders the elements in the range [first, last) in such a way that all elements 
				// for which the predicate p returns true precede the elements for which predicate 
				// p returns false. Relative order of the elements is not preserved.

				BVHEntry *midPtr = std::partition(
					&entries[start],
					&entries[end - 1] + 1,
					[dim, pmid](const BVHEntry & be) 
				{
					return be.aabb.centroid()[dim] < pmid;
				});
					//CompareToMid(dim, pmid));


				middle = midPtr - &entries[0];

				// partition failed -> fallback
				if (middle != start && middle != end)
					break;
			}

			case SplitMethod::Median:
			{
				// nth_element is a partial sorting algorithm that rearranges elements in [first, last) such that:
				// The element pointed at by nth is changed to whatever element would occur in that position if[first, last) was sorted.
				// All of the elements before this new nth element are less than or equal to the elements after the new nth element.
				// More formally, nth_element partially sorts the range [first, last) in ascending order so that the condition 

				// O(N) applications of the predicate, and O(N log N) swaps, where N = last - first.

				middle = (start + end) / 2;//start + (end - start) / 2;

				std::nth_element(
					&entries[start], &entries[middle],
					&entries[end - 1] + 1, 
					[dim](const BVHEntry & a, const BVHEntry & b)
				{
					return a.aabb.centroid()[dim] < b.aabb.centroid()[dim];
				});
					

				break;
			}
			case SplitMethod::SAH:
			{
				// caso tiver poucos objetos
				if (nPrimitives <= 2)
				{
					// então utilzamos partição mediana de objetos
					middle = (start + end) / 2;

					std::nth_element(
						&entries[start], &entries[middle],
						&entries[end - 1] + 1,
						[dim](const BVHEntry & a, const BVHEntry & b)
					{
						return a.aabb.centroid()[dim] < b.aabb.centroid()[dim];
					});
				}
				else 
				{

					// aloca entradas para divisão por meio da surface area heuristics
					GGE_CONSTEXPR int nSAHEntries = 12;
					SAHEntry sahEntries[nSAHEntries];

					// inicializa as entradas para partição
					for (int i = start; i < end; i++)
					{
						int b = nSAHEntries * centroidBounds.offset(entries[i].aabb.centroid())[dim];

						if (b == nSAHEntries)
							b = nSAHEntries - 1;

						sahEntries[b].count++;
						sahEntries[b].aabb = Bounds::merge(sahEntries[b].aabb, entries[i].aabb);
					}

					// computa custos da divisão de cada entrada de SAH
					// custo = custo_traversal + area * objects * custo_intersect 
					float costs[nSAHEntries - 1];
					float minCost = 0;
					int minCostSplitIndex = 0;

					float area = aabb.area();
					//calculateSAHCosts(sahEntries, aabb.area(), nSAHEntries, costs, minCost, minCostSplitIndex);
					for (int i = 0; i < nSAHEntries - 1; ++i)
					{
						Bounds b0, b1;
						int count0 = 0, count1 = 0;

						for (int j = 0; j <= i; ++j)
						{
							b0 = Bounds::merge(b0, sahEntries[j].aabb);
							count0 += sahEntries[j].count;
						}

						for (int j = i + 1; j < nSAHEntries; ++j)
						{
							b1 = Bounds::merge(b1, sahEntries[j].aabb);
							count1 += sahEntries[j].count;
						}

						costs[i] = 1 +
							(count0 * b0.area() +
							count1 * b1.area()) /
							area;
						}

					// computa custo minimo da métrica SAH
					minCost = costs[0];
					for (int i = 1; i < nSAHEntries - 1; ++i) {
						if (costs[i] < minCost) {
							minCost = costs[i];
							minCostSplitIndex = i;
						}
					}

					// cria nodo folha ou divida os objetos de acordo com SAH Entries
					float leafCost = nPrimitives;

					if (nPrimitives > _maxPrimitivesInNode || minCost < leafCost) 
					{
						BVHEntry *pmid = std::partition(
							&entries[start], 
							&entries[end - 1] + 1,
							[=](const BVHEntry & be) // lambda equal
						{
							int b = nSAHEntries * centroidBounds.offset(be.aabb.centroid())[dim];

							if (b == nSAHEntries)
								b = nSAHEntries - 1;
			
							return b <= minCostSplitIndex;
						});

						middle = pmid - &entries[0];
					}
					else 
					{
						// cria nodo folha

						int firstObjOffset = ordered.size();

						for (int i = start; i < end; ++i) 
						{
							int primNum = entries[i].index;
							ordered.push_back(_objects[primNum]);
						}
						leafNodes++;
						node->makeLeaf(firstObjOffset, nPrimitives, aabb);
						return node;
					}

				}

				break;
			}
			}

			childNodes += 2;
			node->makeChilds(dim,
				recursiveBVHBuild(start, middle, totalNodes, entries, ordered),
				recursiveBVHBuild(middle, end, totalNodes, entries, ordered));
		}
	}

	return node;

	// for each axis in aabb, find the longest axis and then split it
	/*for (int i = 0; i < 3; i++)
	{

	vec3 high = node->aabb.getMax()[i];
	vec3 low = node->aabb.getMin()[i];

	// check if the low corresponds to truly lowest axis
	if (low > high) // if not then swap lowest axis to highest
	std::swap(low, high);


	}*/
}

void BVH::calculateSAHCosts(const SAHEntry * sahEntries, const float & area, const int & nEntries, float * costs, float & minCost, int & minCostSplitIndex)
{
	for (int i = 0; i < nEntries - 1; ++i)
	{
		Bounds b0, b1;
		int count0 = 0, count1 = 0;

		for (int j = 0; j <= i; ++j) 
		{
			b0 = Bounds::merge(b0, sahEntries[j].aabb);
			count0 += sahEntries[j].count;
		}

		for (int j = i + 1; j < nEntries; ++j)
		{
			b1 = Bounds::merge(b1, sahEntries[j].aabb);
			count1 += sahEntries[j].count;
		}

		costs[i] = 1 +
			(count0 * b0.area() +
			count1 * b1.area()) /
			area;
	}

	// computa custo minimo da métrica SAH
	minCost = costs[0];
	for (int i = 1; i < nEntries - 1; ++i) {
		if (costs[i] < minCost) {
			minCost = costs[i];
			minCostSplitIndex = i;
		}
	}

}

uint32_t BVH::flattenBVH(BVHBuildNode *node, uint32_t *offset)
{
	BVHFlatNode * linearNode = &nodes[*offset];
	linearNode->aabb = node->aabb;

	uint32_t myOffset = (*offset)++;

	if (node->nPrimitives > 0) {
		//Assert(!node->children[0] && !node->children[1]);
		linearNode->objectOffset = node->firstOffset;
		linearNode->nPrimitives = node->nPrimitives;
	}
	else {
		// Creater interior flattened BVH node
		linearNode->axis = node->splitAxis;
		linearNode->nPrimitives = 0;
		flattenBVH(node->childLeft, offset);
		linearNode->secondChildOffset = flattenBVH(node->childRight, offset);
	}

	return myOffset;
}

bool BVH::intersects(const Ray & ray, RaycastHit & hit) 
{
	if (!nodes || nodes == NULL || nodes == nullptr)
		return false;

	hit.object = nullptr;

	vec3 invDir(1 / ray.direction.x, 1 / ray.direction.y, 1 / ray.direction.z);
	int dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };

	Ray bboxray(ray);
	const bool sign[] = { (ray.direction.x < 0), (ray.direction.y < 0), (ray.direction.z < 0) };
	bboxray.direction = 1.0f / bboxray.direction;

	// Follow ray through BVH nodes to find primitive intersections
	int toVisitOffset = 0, currentNodeIndex = 0;
	int nodesToVisit[128];

	float tNear = Infinity;
	float t = Infinity;
	float tMax = Infinity;

	while (true) 
	{
		const BVHFlatNode *node = &nodes[currentNodeIndex];
		
		// Check ray against BVH node
		//if (node->aabb.intersects(ray, invDir, dirIsNeg, t))
		_aabbTests++;
		if (node->aabb.intersects(bboxray, sign, t) && t < tMax)
		{
			//tMax = t;

			if (node->nPrimitives > 0)
			{
				for (int i = 0; i < node->nPrimitives; ++i)
				{
					Primitive * o = _objects[node->objectOffset + i];

					if (o->getType() != Primitive::PrimitiveType::Mesh && o->getType() != Primitive::PrimitiveType::_Triangle)
						_intersectionTests++;
					if (o->getType() == Primitive::PrimitiveType::_Triangle)
						_triangleTests++;
					

					if (o->intersects(ray, hit, tNear) && tNear < hit.distance)
					{
						hit.distance = tNear;
						hit.object = o;//_objects[node->objectOffset + i];
						hit.ray = &ray;
						tMax = tNear;
						//hit = true;
					}

					//_triangleTests += hit.tests;

				}

				if (toVisitOffset == 0) 
					break;

				currentNodeIndex = nodesToVisit[--toVisitOffset];
			}
			else 
			{
				if (sign[node->axis]) 
				{
					nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
					currentNodeIndex = node->secondChildOffset;
				}
				else {
					nodesToVisit[toVisitOffset++] = node->secondChildOffset;
					currentNodeIndex = currentNodeIndex + 1;
				}
			}
		}
		else 
		{
			

			if (toVisitOffset == 0) 
				break;

			currentNodeIndex = nodesToVisit[--toVisitOffset];
		}
	}

	return (hit.object != nullptr);
}