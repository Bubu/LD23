#include <TriangleGraph.h>
#include <math.h>
#include <iostream>

TriangleGraph::TriangleGraph(int n):_size(20 * (int)pow(4.0,n))
{
	const float phi = 1 + sqrt(5.0) / 2;
	_triangles = new Triangle[_size];
	_triangles_new = new Triangle[_size];
	int currentSize = 20;
	Triangle* startTriangles = calculateStartTriangles();
	normalize(startTriangles, currentSize);
	link_triangles(startTriangles);
	for(int i = 0; i < currentSize; i++){_triangles[i] = startTriangles[i]; }
	
	for(int tes = 0 ; tes <n; tes++)
	{
		for(int i = 0; i < currentSize; i++)
		{
			Triangle new_triangles [4];
			subdivide(_triangles[i], new_triangles[0], new_triangles[1], new_triangles[2], new_triangles[3]);
			for(int ii = 0; ii < 4; ii++)
			{
				_triangles_new[new_triangles[ii].id] = new_triangles[ii];
			}
		}
		currentSize = currentSize * 4;
		for(int i = 0; i < currentSize; i++){_triangles[i] = _triangles_new[i]; }
		normalize(_triangles, currentSize);
	}
}

void TriangleGraph::subdivide(const Triangle& tin, Triangle& tout0, Triangle& tout1, Triangle& tout2, Triangle& tout3)
{
	tout0.a = (tin.a + tin.b) / 2;
	tout0.b = (tin.b + tin.c) / 2;
	tout0.c = (tin.c + tin.a) / 2;
	tout0.id = tin.id*4;
	
	tout1.a = tout0.a;
	tout1.b = tin.b;
	tout1.c = tout0.b;
	tout1.id = tout0.id+1;

	tout2.a = tout0.c;
	tout2.b = tout0.b;
	tout2.c = tin.c;
	tout2.id = tout0.id+2;

	tout3.a = tin.a;
	tout3.b = tout0.a;
	tout3.c = tout0.c;
	tout3.id = tout0.id+3;

	tout0.n0 = tout3.id;
	tout0.n1 = tout1.id;
	tout0.n2 = tout2.id;

	int neighborId; 

	//tout1 neighbor n0
	tout1.n0 = tout0.id;

	//tout1 neighbor n1
	neighborId = _triangles[tin.n1].id*4;
	if(_triangles[tin.n1].n0 == tin.id)
		tout1.n1 = neighborId + 2;
	else if(_triangles[tin.n1].n1 == tin.id)
		tout1.n1 = neighborId + 3;
	else if(_triangles[tin.n1].n2 == tin.id)
		tout1.n1 = neighborId + 1;

	//tout1 neighbor n2
	neighborId = _triangles[tin.n2].id*4;
	if(_triangles[tin.n2].n0 == tin.id)
		tout1.n2 = neighborId + 3;
	else if(_triangles[tin.n2].n1 == tin.id)
		tout1.n2 = neighborId + 1;
	else if(_triangles[tin.n2].n2 == tin.id)
		tout1.n2 = neighborId + 2;

	//tout2 neighbor n0
	neighborId = _triangles[tin.n0].id*4;
	if(_triangles[tin.n0].n0 == tin.id)
		tout2.n0 = neighborId + 3;
	else if(_triangles[tin.n0].n1 == tin.id)
		tout2.n0 = neighborId + 1;
	else if(_triangles[tin.n0].n2 == tin.id)
		tout2.n0 = neighborId + 2;

	//tout2 neighbor n1
	tout2.n1 = tout0.id;

	//tout2 neighbor n2
	neighborId = _triangles[tin.n2].id*4;
	if(_triangles[tin.n2].n0 == tin.id)
		tout2.n2 = neighborId + 2;
	else if(_triangles[tin.n2].n1 == tin.id)
		tout2.n2 = neighborId + 3;
	else if(_triangles[tin.n2].n2 == tin.id)
		tout2.n2 = neighborId + 1;

	//tout3 neighbor n0
	neighborId = _triangles[tin.n0].id*4;
	if(_triangles[tin.n0].n0 == tin.id)
		tout3.n0 = neighborId + 2;
	else if(_triangles[tin.n0].n1 == tin.id)
		tout3.n0 = neighborId + 3;
	else if(_triangles[tin.n0].n2 == tin.id)
		tout3.n0 = neighborId + 1;	

	//tout3 neighbor n1
	neighborId = _triangles[tin.n1].id*4;
	if(_triangles[tin.n1].n0 == tin.id)
		tout3.n1 = neighborId + 3;
	else if(_triangles[tin.n1].n1 == tin.id)
		tout3.n1 = neighborId + 1;
	else if(_triangles[tin.n1].n2 == tin.id)
		tout3.n1 = neighborId + 2;

	//tout3 neighbor n2
	tout3.n2 = tout0.id;
	
}

TriangleGraph* TriangleGraph::tesselate(TriangleGraph tg)
{
	return 0;
}


void TriangleGraph::link_triangles(Triangle* triangles)
{
	int adjacentFaceIndices[30][2] = {{9, 10}, {6, 7}, {7, 8}, {6, 10}, {8, 9}, {4, 5}, {2, 3}, {1, 2}, 
		{3,4}, {1, 5}, {12, 20}, {12, 19}, {10, 20}, {9, 19}, {14, 17}, {15, 17}, {4, 14}, {5, 15},
		{7, 17}, {6, 16}, {14, 16}, {8, 18}, {15, 18}, {2, 12}, {13, 20}, {3, 13}, {11, 19}, {1, 11},
		{13, 16}, {11, 18}};
	for(int i = 0; i < 30; i++) for(int ii = 0; ii < 2; ii++) adjacentFaceIndices[i][ii]-=1;
		
	for(int i = 0; i < 30; i++) 
	{
		Triangle &t1 = triangles[adjacentFaceIndices[i][0]];
		Triangle &t2 = triangles[adjacentFaceIndices[i][1]];

		int otherPoint;
		if(!(same(t1.a,t2.a,0.000001) || same(t1.a,t2.b,0.000001) || same(t1.a,t2.c,0.000001)))
		otherPoint = 0;
		if(!(same(t1.b,t2.a,0.000001) || same(t1.b,t2.b,0.000001) || same(t1.b,t2.c,0.000001)))
		otherPoint = 1;
		if(!(same(t1.c,t2.a,0.000001) || same(t1.c,t2.b,0.000001) || same(t1.c,t2.c,0.000001)))
		otherPoint = 2;

		switch(otherPoint)
		{
		case 0: t1.n2 = adjacentFaceIndices[i][1]; 	break;
		case 1: t1.n0 = adjacentFaceIndices[i][1];	break;
		case 2: t1.n1 = adjacentFaceIndices[i][1];	break;
		}

		std::cout<<"otherpoint: "<<otherPoint<<"\n";
		if(!(same(t2.a,t1.a,0.000001) || same(t2.a,t1.b,0.000001) || same(t2.a,t1.c,0.000001)))
		otherPoint = 0;
		if(!(same(t2.b,t1.a,0.000001) || same(t2.b,t1.b,0.000001) || same(t2.b,t1.c,0.000001)))
		otherPoint = 1;
		if(!(same(t2.c,t1.a,0.000001) || same(t2.c,t1.b,0.000001) || same(t2.c,t1.c,0.000001)))
		otherPoint = 2;

		switch(otherPoint)
		{
		case 0: t2.n2 = adjacentFaceIndices[i][0]; 	break;
		case 1: t2.n0 = adjacentFaceIndices[i][0];	break;
		case 2: t2.n1 = adjacentFaceIndices[i][0];	break;
		}

	}

}

void TriangleGraph::normalize(Triangle* triangles, int lenght)
{
	for(int i = 0; i < lenght; i++)
	{
		triangles[i].a.normalize();
		triangles[i].b.normalize();
		triangles[i].c.normalize();
	}
}

TriangleGraph::Triangle* TriangleGraph::calculateStartTriangles()
{ 
	Triangle* startTriangles = new Triangle[20];
	Vector3f vertices[12] = 
		{Vector3f(0,0,-5/sqrt(50 - 10*sqrt(5.))),
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
	{
		startTriangles[i] = Triangle(vertices[faceIndices[i][0]], vertices[faceIndices[i][1]], vertices[faceIndices[i][2]],i);
	}
	return startTriangles;	
}