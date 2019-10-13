#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CGL {

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {

  // TODO (Part 2.2):
  // Implement ray - bounding box intersection test
  // If the ray intersected the bounding box within the range given by
  // t0, t1, update t0 and t1 with the new intersection times.

	double tmin_x, tmax_x, tmin_y, tmax_y, tmin_z,tmax_z;

	tmin_x = (min[0]-r.o[0])/r.d[0];
	tmax_x = (max[0]-r.o[0])/r.d[0];
	tmin_y = (min[1]-r.o[1])/r.d[1];
	tmax_y = (max[1]-r.o[1])/r.d[1];
	tmin_z = (min[2]-r.o[2])/r.d[2];
	tmax_z = (max[2]-r.o[2])/r.d[2];

	if(tmin_x > tmax_x){
		double temp = tmin_x;
		tmin_x = tmax_x;
		tmax_x = temp;
	}

	if(tmin_y > tmax_y){
			double temp = tmin_y;
			tmin_y = tmax_y;
			tmax_y = temp;
		}

	if(tmin_z > tmax_z){
			double temp = tmin_z;
			tmin_z = tmax_z;
			tmax_z = temp;
		}

	double tmin = std::max({tmin_x,tmin_y,tmin_z});
	double tmax = std::min({tmax_x,tmax_y,tmax_z});

	if(tmin < 0 || tmax<0){
		return false;
	}

	if(tmin>=tmax){
		return false;
	}

//	if(tmin < r.min_t || tmax < r.min_t){
//			return false;
//	}
//
//	if(tmax > r.max_t || tmin > r.max_t){
//		return false;
//	}

	t0 = tmin;
	t1 = tmax;

  return true;
}

void BBox::draw(Color c, float alpha) const {

  glColor4f(c.r, c.g, c.b, alpha);

  // top
  glBegin(GL_LINE_STRIP);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(max.x, max.y, max.z);
  glEnd();

  // bottom
  glBegin(GL_LINE_STRIP);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, min.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glEnd();

  // side
  glBegin(GL_LINES);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(min.x, min.y, max.z);
  glEnd();

}

std::ostream& operator<<(std::ostream& os, const BBox& b) {
  return os << "BBOX(" << b.min << ", " << b.max << ")";
}

} // namespace CGL
