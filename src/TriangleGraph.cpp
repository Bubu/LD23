#include <TriangleGraph.h>
#include <math.h>

TriangleGraph::TriangleGraph(int n):_size(12 * (int)pow(4.0,n))
{

  _triangles = new Triangle[_size];
}