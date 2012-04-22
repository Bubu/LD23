#ifndef RAY3F_H
#define RAY3F_H

#include <Vector3f.h>

class Ray3f
{
  public:	
	inline Ray3f(Vector3f o, Vector3f d):o(o),d(d){}
	inline Ray3f():o(Vector3f()),d(Vector3f()){}
	inline Ray3f(const Ray3f& other):o(other.o),d(other.d){}
	Vector3f d;
	Vector3f o;	
};

inline Ray3f createRayPerspective(const Vector3f& u, const Vector3f& v, const Vector3f& w, const Vector3f& eye, float width, float height, float x, float y, float fovy)
{
	const float b=tan((fovy/360.0f)*3.1415926535897932384626433832795f);
	const float c=width*b/height;
	const float X= 2.0f*(x*c)/width-c ; 
	const float Y= 2.0f*((height-y)*b)/height-b;
	Vector3f d=u*X+v*Y-w;
	return Ray3f(eye,d);
}

inline Ray3f createRayOrtho(const Vector3f& u, const Vector3f& v, const Vector3f& w, const Vector3f& eye, float width, float height, float x, float y, float scale)
{
	const float X= (2.0f*x/height-width/height)/scale ; 
	const float Y= (2.0f*(height-y)/height-1.0)/scale;
	const Vector3f d=Vector3f()-w;
	const Vector3f e=eye+u*X+v*Y;
	return Ray3f(e,d);
}

#endif
