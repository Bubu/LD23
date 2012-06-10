#ifndef __Player_H_INCLUDED__
#define __Player_H_INCLUDED__
#include <Genie.h>
#include <Matrix3x3f.h>
//class TriangleGraph;
class World;
class Player
{
  public:
	Player(Genie& _genie, World& world);	
	void getTransformation(Vector3f& pos, Matrix3x3f& rot)const;
	void getTransformation(float m[16])const;
	void getInverseTransformation(Vector3f& pos, Matrix3x3f& rot)const;
	void getInverseTransformation(float m[16])const;
	Vector3f cameraPos()const;
	float cameraAngle()const;
	//inline float roty()const{return _roty;}
	inline float h()const {return _h;}
	void tick(float time, float move, bool jump, float roty, bool shoot);
	inline int trinagle()const{return _trinagle;} 
	void setTriangle(int i);
	Vector3f DEB_trace[1000];
	int DEB_currenttrace;
	float DEB_sumtime;
  private:
	void _addRoty(float f);
	void _moveForward(float f);
	void _jump(bool jump, float t);	
	float _h;
	float _v,_vElevator;
	int _trinagle;
	Matrix3x3f _R;
	Genie& _genie;
	World& _world;
	float _camh;
	float _jumpCharge;
	bool  _jumpButtonState;
	int _jumpState;//Ground,Charge,first,second,...
	
	
	
};
#endif
