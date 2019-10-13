#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CGL { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.

  
  return false;


}

bool Sphere::intersect(const Ray& r) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.

	float a = dot(r.d,r.d);
	float b = dot(2*(r.o-o),r.d);
	float c = dot((r.o - o),(r.o-o)) - r2;

	float x2=(b*b)-(4*a*c);
	float t1=(-b-sqrt(x2))/(2*a);
	float t2=(-b+sqrt(x2))/(2*a);

	if(t1>=r.min_t && t1<= r.max_t){
		r.max_t = t1;
	}

	else if(t2 >= r.min_t && t2 <= r.max_t){
		r.max_t = t2;
	}

	else{
		return false;
	}

	return true;
}

bool Sphere::intersect(const Ray& r, Intersection *i) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.
  
  
	float a = dot(r.d,r.d);
		float b = dot(2*(r.o-o),r.d);
		float c = dot((r.o - o),(r.o-o)) - r2;

		float x2=(b*b)-(4*a*c);
		float t1=(-b-sqrt(x2))/(2*a);
		float t2=(-b+sqrt(x2))/(2*a);
		float t;

		if(t1>=r.min_t && t1<= r.max_t){
			r.max_t = t1;
			t = t1;
		}

		else if(t2 >= r.min_t && t2 <= r.max_t){
			r.max_t = t2;
			t=t2;
		}

		else{
			return false;
		}

		i->t = t;
	    i->n = normal(r.o + t*r.d);
	    i->primitive = this;
	    i->bsdf = get_bsdf();

		return true;

  
}

void Sphere::draw(const Color& c, float alpha) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c, float alpha) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CGL
