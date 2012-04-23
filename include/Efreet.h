#ifndef __Efreet_h_included__
#define __Efreet_h_included__
#include <Vector3f.h>
#include <Matrix3x3f.h>
class Efreet
{
  public:
	Efreet();	
	void draw();
	void setRotation(const Matrix3x3f r){_rotation=r;}
	void setTranlation(const Vector3f t){_translate=t;}
	void setScale(float s){_scale=s;}
	void getPositions(Vector3f points[10])const;
	inline int triangle()const {return _triangleId;}
	inline void setTriangle(int i){_triangleId=i;}
  private:	
	
	int _triangleId;
	Matrix3x3f _rotation;
	Vector3f _translate;
	float _scale;
	float _swirlHeight;
	float _shoulderHeight;
	float _shoulderWidth;
	float _upperArmLength;
	float _forearmLength;
	float _headHeight;
	
	float _armAngle[2][3];
	float _neckAngle;
	
	
	
	
	
};
#endif
