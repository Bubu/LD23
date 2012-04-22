#ifndef __TriangleGraph_h_included__
#define __TriangleGraph_h_included__
#include <string>
#include <gl/glew.h>
#include <Vector3f.h>

class TriangleGraph
{
  public:
	TriangleGraph(int n);
	class Triangle
	{
	  public:
		  Vector3f a,b,c;
		  int n0,n1,n2;
		  int id;
		  inline Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, int id):a(a),b(b),c(c),n0(-1),n1(-1),n2(-1),id(id){}
		  inline Triangle():n0(-1),n1(-1),n2(-1),id(-1){}
		  inline Vector3f centerPoint()const{return (a + b + c)/3;}
	};
	//Triangle operator[](int i)const{return triangles[i]};
	inline Triangle& operator[](int i){return _triangles[i];}
	inline int size()const{return _size;}
  private:	
	void  subdivide(const Triangle& tin, Triangle& tout0, Triangle& tout1, Triangle& tout2, Triangle& tout3);
	TriangleGraph* tesselate(TriangleGraph tg);
	void link_triangles(Triangle* triangles);

	const int _size;
	int _currentSize;
	Triangle* _triangles;
	Triangle* _triangles_new;
	void normalize(Triangle* triangles, int lenght);
	TriangleGraph::Triangle* TriangleGraph::calculateStartTriangles();
};
#endif