#ifndef __Player_H_INCLUDED__
#define __Player_H_INCLUDED__
#include <Genie.h>
#include <Matrix3x3f.h>
class Player
{
  public:
	Player(Genie& _genie);	
	void getTransformation(Vector3f& pos, Matrix3x3f& rot)const;
	void getTransformation(float m[16])const;
	void getInverseTransformation(Vector3f& pos, Matrix3x3f& rot)const;
	void getInverseTransformation(float m[16])const;
	Vector3f cameraPos()const;
	float cameraAngle()const;
	inline float roty()const{return _roty;}
	void addRoty(float f);
  private:	
	float _teta,_phi,_roty;

	Genie& _genie;
	
	
	
};
#endif
