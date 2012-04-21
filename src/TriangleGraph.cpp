#include <TriangleGraph.h>
#include <math.h>
#include <iostream>

TriangleGraph::TriangleGraph(int n):_size(20 * (int)pow(4.0,n))
{
	const float phi = 1 + sqrt(5.0) / 2;
	_triangles = new Triangle[_size];
	Triangle* startTriangles = new Triangle[30];

	Vector3f vertices[] = 
	{
		Vector3f(0,0,-5/sqrt(50 - 10*sqrt(5.))),
		Vector3f(0,0,5/sqrt(50 - 10*sqrt(5.))),
		Vector3f(-sqrt(2/(5 - sqrt(5.))),0,-(1/sqrt(10 - 2*sqrt(5.)))),
		Vector3f(sqrt(2/(5 - sqrt(5.))),0,1/sqrt(10 - 2*sqrt(5.))),

		Vector3f((1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),-0.5,-(1/sqrt(10 - 2*sqrt(5.)))),
		Vector3f((1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),0.5,-(1/sqrt(10 - 2*sqrt(5.)))),
		Vector3f(-(1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),-0.5,1/sqrt(10 - 2*sqrt(5.))),
		Vector3f(-(1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),0.5,1/sqrt(10 - 2*sqrt(5.))),

		Vector3f(-(-1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),-sqrt((5 + sqrt(5.))/(5 - sqrt(5.)))/2.,-(1/sqrt(10 - 2*sqrt(5.)))),
		Vector3f(-(-1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),sqrt((5 + sqrt(5.))/(5 - sqrt(5.)))/2.,-(1/sqrt(10 - 2*sqrt(5.)))),
		Vector3f((-1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),-sqrt((5 + sqrt(5.))/(5 - sqrt(5.)))/2.,1/sqrt(10 - 2*sqrt(5.))),
		Vector3f((-1 + sqrt(5.))/(2.*sqrt(10 - 2*sqrt(5.))),sqrt((5 + sqrt(5.))/(5 - sqrt(5.)))/2.,1/sqrt(10 - 2*sqrt(5.)))
	};

	int faceIndices[20][3] = {{2, 12, 8}, {2, 8, 7}, {2, 7, 11}, {2, 11, 4}, {2, 4, 12}, {5, 9, 1},
		{6, 5, 1}, {10, 6, 1}, {3, 10, 1}, {9, 3, 1}, {12, 10, 8}, {8, 3, 7},
		{7, 9, 11}, {11, 5, 4}, {4, 6, 12}, {5, 11, 9}, {6, 4, 5}, {10, 12, 6}, {3, 8, 10}, {9, 7, 3}};
	for(int i = 0; i < 20; i++) for(int ii = 0; ii < 3; ii++) faceIndices[i][ii]-=1;

	for(int i = 0; i < 20; i++)
		startTriangles[i] = Triangle(vertices[faceIndices[i][0]], vertices[faceIndices[i][1]], vertices[faceIndices[i][2]]);
	_triangles = startTriangles;
	link_triangles();

	/*for(int i = 0; i < currentSize; i++)
		{
			subdivide(startTriangles[i],);
		}*/
}

void TriangleGraph::subdivide(const Triangle& tin, Triangle& tout1, Triangle& tout2, Triangle& tout3)
{
	/*Triangle* currentTriangles = new Triangle[currentSize];
	int currentSize = 30;
	int newsize = currentSize<<2;
	std::cout<<"newsize: "<<newsize<<"\n";
	for(int i = 0; i < currentSize; i++)
	{
		subdivde(currentTriangles[i]);
	}*/
}

TriangleGraph* TriangleGraph::tesselate(TriangleGraph tg)
{
	return 0;
}


void TriangleGraph::link_triangles()
{
	int adjacentFaceIndices[30][2] = {{9, 10}, {6, 7}, {7, 8}, {6, 10}, {8, 9}, {4, 5}, {2, 3}, {1, 2}, 
		{3,4}, {1, 5}, {12, 20}, {12, 19}, {10, 20}, {9, 19}, {14, 17}, {15, 17}, {4, 14}, {5, 15},
		{7, 17}, {6, 16}, {14, 16}, {8, 18}, {15, 18}, {2, 12}, {13, 20}, {3, 13}, {11, 19}, {1, 11},
		{13, 16}, {11, 18}};
	for(int i = 0; i < 30; i++) for(int ii = 0; ii < 2; ii++) adjacentFaceIndices[i][ii]-=1;
		
	for(int i = 0; i < 30; i++) 
	{
		Triangle t1 = _triangles[adjacentFaceIndices[i][0]];
		Triangle t2 = _triangles[adjacentFaceIndices[i][1]];

		int otherPoint;
		if(same(t1.a,t2.a,0.000001) || same(t1.a,t2.b,0.000001))
		otherPoint = 2;
		if(same(t1.a,t2.a,0.000001) || same(t1.a,t2.c,0.000001))
		otherPoint = 1;
		if(same(t1.a,t2.b,0.000001) || same(t1.a,t2.c,0.000001))
		otherPoint = 0;

		switch(otherPoint)
		{
		case 0: t1.n2 = adjacentFaceIndices[i][1]; 	break;
		case 1: t1.n0 = adjacentFaceIndices[i][1];	break;
		case 2: t1.n1 = adjacentFaceIndices[i][1];	break;
		}

		if(same(t2.a,t1.a,0.000001) || same(t2.a,t1.b,0.000001))
		otherPoint = 2;
		if(same(t2.a,t1.a,0.000001) || same(t2.a,t1.c,0.000001))
		otherPoint = 1;
		if(same(t2.a,t1.b,0.000001) || same(t2.a,t1.c,0.000001))
		otherPoint = 0;

		switch(otherPoint)
		{
		case 0: t2.n2 = adjacentFaceIndices[i][0]; 	break;
		case 1: t2.n0 = adjacentFaceIndices[i][0];	break;
		case 2: t2.n1 = adjacentFaceIndices[i][0];	break;
		}

	}

}