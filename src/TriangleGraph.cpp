#include <TriangleGraph.h>
#include <math.h>
#include <iostream>

TriangleGraph::TriangleGraph(int n):_size(30 * (int)pow(4.0,n))
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

	int faceIndices[30][3] = {{2, 12, 8}, {2, 8, 7}, {2, 7, 11}, {2, 11, 4}, {2, 4, 12}, {5, 9, 1},
		{6, 5, 1}, {10, 6, 1}, {3, 10, 1}, {9, 3, 1}, {12, 10, 8}, {8, 3, 7},
		{7, 9, 11}, {11, 5, 4}, {4, 6, 12}, {5, 11, 9}, {6, 4, 5}, {10, 12, 6}, {3, 8, 10}, {9, 7, 3}};
	for(int i = 0; i < 30; i++) for(int ii = 0; ii < 3; ii++) faceIndices[i][ii]-=1;

	for(int i = 0; i < 30; i++)
		startTriangles[i] = Triangle(vertices[faceIndices[i][0]], vertices[faceIndices[i][1]], vertices[faceIndices[i][2]]);

	_triangles = startTriangles;
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