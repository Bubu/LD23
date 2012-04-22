#ifndef __Genie_h_included__
#define __Genie_h_included__
#include <Vector3f.h>
#include <Matrix3x3f.h>
class Genie
{
  public:
	Genie();	
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
