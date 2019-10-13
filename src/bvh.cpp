#include "bvh.h"

#include "CGL/CGL.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  root = construct_bvh(_primitives, max_leaf_size);

}

BVHAccel::~BVHAccel() {
  if (root) delete root;
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

void BVHAccel::draw(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->draw(c, alpha);
  } else {
    draw(node->l, c, alpha);
    draw(node->r, c, alpha);
  }
}

void BVHAccel::drawOutline(BVHNode *node, const Color& c, float alpha) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->drawOutline(c, alpha);
  } else {
    drawOutline(node->l, c, alpha);
    drawOutline(node->r, c, alpha);
  }
}

BVHNode *BVHAccel::construct_bvh(const std::vector<Primitive*>& prims, size_t max_leaf_size) {
  
  // TODO (Part 2.1):
  // Construct a BVH from the given vector of primitives and maximum leaf
  // size configuration. The starter code build a BVH aggregate with a
  // single leaf node (which is also the root) that encloses all the
  // primitives.

  BBox centroid_box, bbox;
  Vector3D c_total(0.,0.,0.);


  for (Primitive *p : prims) {
      BBox bb = p->get_bbox();
      bbox.expand(bb);
      Vector3D c = bb.centroid();
      c_total = c_total + c;
      //centroid_box.expand(c);
  }

  BVHNode *node = new BVHNode(bbox);

  if(prims.size() <= max_leaf_size){ //This is a leaf node

	  node->prims = new vector<Primitive *>(prims);
	  return node;
  }

  //else we need to divide the box

  //This section finds the largest axis to split by (which we will construct l/r later)
  Vector3D extent = bbox.extent;
  int axis = 0;
  double largest = -INF_D;

  for(int i = 0; i<3; i++){
	  if (extent[i] > largest){
		  largest = extent[i];
		  axis = i;
	  }
  }

  //Vector3D split_point = centroid_box.centroid();
  Vector3D split_point = c_total/prims.size();


  vector<Primitive *> left;
  vector<Primitive *> right;

  for (Primitive *p : prims) {

	  if(p->get_bbox().centroid()[axis] <= split_point[axis]){
		  left.push_back(p);
	  }
	  else{
		  right.push_back(p);
	  }

    }
  node->l = construct_bvh(left,max_leaf_size);
  node->r = construct_bvh(right,max_leaf_size);

  return node;

}


bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {

  // TODO (Part 2.3):
  // Fill in the intersect function.
  // Take note that this function has a short-circuit that the
  // Intersection version cannot, since it returns as soon as it finds
  // a hit, it doesn't actually have to find the closest hit.

	double tmin = 3;
	double tmax;


	if (!(node->bb.intersect(ray,tmin,tmax))){
		return false;
	}

	if(tmin > ray.max_t || ray.min_t > tmax ){
		return false;
	}

	if(node->isLeaf()){
		for (Primitive *p : *(node->prims)) {
			total_isects++;
		    if (p->intersect(ray))
		      return true;
		  }
		return false;
	}

	bool hit1 = intersect(ray,node->l);
	bool hit2 = intersect(ray,node->r);

	return hit1 || hit2;

}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {

  // TODO (Part 2.3):
  // Fill in the intersect function.

	double tmin, tmax;

		if (!(node->bb.intersect(ray,tmin,tmax))){
			return false;
		}

		if(tmin > ray.max_t || ray.min_t > tmax ){
				return false;
			}

		if(node->isLeaf()){
			bool return_val = false;
			for (Primitive *p : *(node->prims)) {
				total_isects++;
			    if (p->intersect(ray,i))
			      return_val = true;
			  }
			return return_val;
		}

		bool hit1 = intersect(ray,i,node->l);
		bool hit2 = intersect(ray,i,node->r);

		return hit1 || hit2;
}

}  // namespace StaticScene
}  // namespace CGL
