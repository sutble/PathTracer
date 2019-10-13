#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  BBox bb(p1);
  bb.expand(p2); 
  bb.expand(p3);
  return bb;

}

bool Triangle::intersect(const Ray& r) const {

  // TODO (Part 1.3):
  // implement ray-triangle intersection

	 Vector3D p0(mesh->positions[v1]), p1(mesh->positions[v2]), p2(mesh->positions[v3]);

	  Vector3D e1 = p1-p0;
	  Vector3D e2 = p2-p0;
	  Vector3D s1 = cross(r.d,e2);
	  Vector3D s = r.o - p0;
	  Vector3D s2 = cross(s,e1);

	  float det = dot(e1,s1);

	  float u = dot(s,s1)/det; //barycentric coordinate
	  float v = dot(r.d,s2)/det;
	  float t = dot(e2,s2)/det;

	  if (u<0 || u>1 || v<0 || u+v > 1 || v > 1){
		  return false;
	  }

	  if(t>= r.min_t && t<= r.max_t){
	 		  r.max_t = t;
	 	  }
	 	  else {
	 		  return false;
	 	  }


	  return true;

}

bool Triangle::intersect(const Ray& r, Intersection *isect) const {
  
  // TODO (Part 1.3):
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly

  Vector3D p0(mesh->positions[v1]), p1(mesh->positions[v2]), p2(mesh->positions[v3]);
  Vector3D n0(mesh->normals[v1]), n1(mesh->normals[v2]), n2(mesh->normals[v3]);
  
  Vector3D e1 = p1-p0;
  Vector3D e2 = p2-p0;
  Vector3D s = r.o - p0;
  Vector3D s1 = cross(r.d,e2);
  Vector3D s2 = cross(s,e1);

  float det = dot(e1,s1);

  float u = dot(s,s1)/det; //barycentric coordinate
  float v = dot(r.d,s2)/det;
  float t = dot(e2,s2)/det;

  if (u<0 || u>1 || v<0 || u+v > 1 || v > 1){
	  return false;
  }

  if(t>= r.min_t && t<= r.max_t){
  	  r.max_t = t;
    }
    else {
  	  return false;
    }

  isect->t = t;
  isect->n = (1-u-v)*n0 + u*n1 + v*n2;
  isect->primitive = this;
  isect->bsdf = get_bsdf();

  return true;
}

void Triangle::draw(const Color& c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CGL
