#include <Player.h>
#include <World.h>
#include <TriangleGraph.h>
#include <iostream>
static const float pi=3.1415926535897932384626433832795f ;
static const float _vElevatorMax=1.0f;
Player::Player(Genie& genie, World& world):
	_genie(genie),_teta(pi/2.0),_phi(pi),_roty(0.0f),_h(0.0f),_v(0.0f),_vElevator(_vElevatorMax),
	_world(world)
{
	_moveForward(0.0f);
	
}	



void Player::getTransformation(Vector3f& pos, Matrix3x3f& rot)const
{
	const float su=sin(_teta); const float cu=cos(_teta);
  	const float sv=sin(_phi); const float cv=cos(_phi);
  	pos=Vector3f(su*cv,su*sv,cu);
  	rot=Matrix3x3f(	cu*cv,pos.x,-sv,
	  				cu*sv,pos.y, cv,
					 -su ,pos.z,0.0f);
}
void Player::getTransformation(float m[16])const
{
	const float su=sin(_teta); const float cu=cos(_teta);
  	const float sv=sin(_phi); const float cv=cos(_phi);
  	m[ 0]=cu*cv; m[ 4]=su*cv; m[ 8]=-sv ; m[12]=su*cv;
  	m[ 1]=cu*sv; m[ 5]=su*sv; m[ 9]= cv ; m[13]=su*sv;
  	m[ 2]=-su  ; m[ 6]= cu  ; m[10]=0.0f; m[14]= cu  ;
  	m[ 3]= 0.0f; m[ 7]= 0.0f; m[11]=0.0f; m[15]= 1.0f;
}

void Player::getInverseTransformation(Vector3f& pos, Matrix3x3f& rot)const
{
	const float su=sin(_teta); const float cu=cos(_teta);
  	const float sv=sin(_phi); const float cv=cos(_phi);
  	pos=Vector3f(0,-1,0);
	rot=Matrix3x3f(	cu*cv,cu*sv,-su,
	  				su*cv,su*sv, cu,
					  sv ,-cv  ,0.0f);
}

void Player::getInverseTransformation(float m[16])const
{
	const float su=sin(_teta); const float cu=cos(_teta);
  	const float sv=sin(_phi); const float cv=cos(_phi);
  	m[ 0]=cu*cv; m[ 4]=sv*cu; m[ 8]=-su ; m[12]= 0.0f;
  	m[ 1]=cv*su; m[ 5]=su*sv; m[ 9]= cu ; m[13]=-1.0f;
  	m[ 2]=  sv ; m[ 6]= -cv ; m[10]=0.0f; m[14]= 0.0f;
  	m[ 3]= 0.0f; m[ 7]= 0.0f; m[11]=0.0f; m[15]= 1.0f;
}
void Player::_addRoty(float f)
{
	_roty+=f*pi*2;
	if (_roty<  0.0  )_roty+=pi*2.0f;
	if (_roty>2.0f*pi)_roty-=pi*2.0f;	
}

Vector3f Player::cameraPos()const
{
	return Vector3f(0.0,+0.2,0.1 );	
}
float Player::cameraAngle()const
{
	return -60.0f/180.0f*pi;
}

void Player::_moveForward(float f)
{
	Vector3f x,p;
	Matrix3x3f R;
	getTransformation(x, R);
	/*Matrix3x3f R2=R*makeRotYMatrix3x3( _roty);
	Matrix3x3f RT=R2;
	RT.transpose();
	Matrix3x3f Rx=makeRotXMatrix3x3(f*pi*2);
	p=RT*(Rx*(R2*x));*/
	p=Vector3f(R.m02,R.m12,R.m22)*(cos( _roty)*f)+Vector3f(R.m00,R.m10,R.m20)*(sin( _roty)*f)+x;
	p.normalize();
	
	Matrix3x3f ry=makeRotYMatrix3x3(-_roty);
	R=R*ry;
	p=R*Vector3f(0.0f,cos(f),sin(f));
	_phi=atan2(p.y,p.x);
	_teta=acos(p.z/p.length());
	_trinagle =-1;
	const TriangleGraph& triangleGraph = _world.level(_world.current()).triangleGraph();
	for (int i=0;i<triangleGraph.size();i++)
	{
		if (triangleGraph[i].isInside(p))_trinagle=i;
	}
}

void Player::_jump(float f, float t)
{
	if (_h==0.0f && _vElevator==_vElevatorMax && f>0.0f)_v=0.03;
	const float newv=(_vElevator<f)?_vElevator:f;
	_vElevator-=newv;
	_v+=newv*0.6-0.15*t;
	_h+=_v*t*2.0;//-9.81f*f*f;
	//std::cout<<"_jump:("<<f<<","<<_h<<","<<_v<<")["<<_vElevator<<"]["<<newv<<"]\n";
	if (_h<0.0)
	{
		_h=0.0f;
		_vElevator=_vElevatorMax;
		_v=0.0f;
	}
}

void Player::tick(float time, float move, float jump, float roty, bool shoot)
{
	_jump(jump,time);
	_addRoty(roty);	
	_moveForward(move);
	//std::cout<<"("<<shoot<<","<<_world.attack().isAlive()<<")\n";
	if (shoot && !_world.attack().isAlive())
	{
		std::cout<< "huhu!"<<std::endl;
		Vector3f pos;
		Matrix3x3f rot;
		Matrix3x3f ry=makeRotYMatrix3x3(-_roty);
		getTransformation(pos,rot);//Inverse
		rot=rot*ry;
		std::cout<<"A("<<shoot<<","<<_world.attack().isAlive()<<")\n";
		_world.setAttack(0.01, 10.0f, rot ) ;
		std::cout<<"B("<<shoot<<","<<_world.attack().isAlive()<<")\n";
	}
}
