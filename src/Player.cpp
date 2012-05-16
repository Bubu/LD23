#include <Player.h>
#include <World.h>
#include <TriangleGraph.h>
#include <iostream>
static const float pi=3.1415926535897932384626433832795f ;
static const float _vElevatorMax=1.0f;
Player::Player(Genie& genie, World& world):
	_genie(genie),/*_teta(pi/2.0),_phi(pi),_roty(0.0f),*/_h(0.0f),_v(0.0f),_vElevator(_vElevatorMax),
	_world(world)
{
	
	setTriangle(_world.currentLevel().startTile());
	
}	



void Player::getTransformation(Vector3f& pos, Matrix3x3f& rot)const
{
	rot=_R;
	pos=Vector3f(_R.m01,_R.m11,_R.m21);
}
void Player::getTransformation(float m[16])const
{
  	m[ 0]=_R.m00; m[ 4]=_R.m01; m[ 8]=_R.m02; m[12]=_R.m01;
  	m[ 1]=_R.m10; m[ 5]=_R.m11; m[ 9]=_R.m12; m[13]=_R.m11;
  	m[ 2]=_R.m20; m[ 6]=_R.m21; m[10]=_R.m22; m[14]=_R.m21;
  	m[ 3]= 0.0f ; m[ 7]= 0.0f ; m[11]= 0.0f ; m[15]= 1.0f ;
}

void Player::getInverseTransformation(Vector3f& pos, Matrix3x3f& rot)const
{
  	pos=Vector3f(0,-1,0);
  	rot=_R;
  	rot.transpose();
}

void Player::getInverseTransformation(float m[16])const
{
  	m[ 0]=_R.m00; m[ 4]=_R.m10; m[ 8]=_R.m20; m[12]= 0.0f ;
  	m[ 1]=_R.m01; m[ 5]=_R.m11; m[ 9]=_R.m21; m[13]=-1.0f ;
  	m[ 2]=_R.m02; m[ 6]=_R.m12; m[10]=_R.m22; m[14]= 0.0f ;
  	m[ 3]= 0.0f ; m[ 7]= 0.0f ; m[11]= 0.0f ; m[15]= 1.0f ;
}
void Player::_addRoty(float f)
{
	const float ds=f*pi*2;
	_R=_R*makeRotYMatrix3x3(ds);	
}

Vector3f Player::cameraPos()const
{
	return Vector3f(0.0,_camh,0.1 );	
}
float Player::cameraAngle()const
{
	return -60.0f/180.0f*pi;
}

void Player::_moveForward(float f)
{
	Matrix3x3f R=_R*makeRotXMatrix3x3(f*pi*2);
	Vector3f p=Vector3f(R.m01,R.m11,R.m21);
	int t_trinagle =-1;
	const TriangleGraph& triangleGraph = _world.level(_world.current()).triangleGraph();
	for (int i=0;i<triangleGraph.size();i++)
	{
		if (triangleGraph[i].isInside(p))t_trinagle=i;
	}
	
	float diff=1.0f+(_h)*0.05-_world.currentLevel()[t_trinagle].height;
	const bool blocking=diff<0.000||_world.currentLevel()[t_trinagle].type==Tile::water;
	if (t_trinagle>=0 &&  !blocking)
	{
		_trinagle=t_trinagle;
		_R=R;
		_genie.setTriangle(_trinagle);
	}
}

void Player::_jump(float f, float t)
{
	const float base=(_world.currentLevel()[_trinagle].height-1)/0.05; 
	if (_h<=base && _vElevator==_vElevatorMax && f>0.0f)_v=0.03;
	const float newv=(_vElevator<f)?_vElevator:f;
	_vElevator-=newv;
	_v+=newv*0.6-0.15*t;
	_h+=_v*t*2.0;//-9.81f*f*f;
	//std::cout<<"_jump:("<<f<<","<<_h<<","<<_v<<")["<<_vElevator<<"]["<<newv<<"]\n";
	
	if (_h<base)
	{
		_h=base;
		_vElevator=_vElevatorMax;
		_v=0.0f;
	}
}

void Player::tick(float time, float move, float jump, float roty, bool shoot)
{
	/*
	float move=0.0f; if (down)move-=t*0.01f; if (up)   move+=t*0.01f;
	float roty=0.0f; if (left)roty-=t*0.03f; if (right)roty+=t*0.03f;
	float jump=0.0f; if (jumped)jump+=t;
	*/
	_jump(jump*10,time*10);
	_addRoty(roty*0.3333f*time);	
	_moveForward(move*0.04f*time);
	//std::cout<<"("<<shoot<<","<<_world.attack().isAlive()<<")\n";
	if (shoot && !_world.attack().isAlive())
	{
		std::cout<< "huhu!"<<std::endl;
		Vector3f pos;
		Matrix3x3f rot;
		/*Matrix3x3f ry=makeRotYMatrix3x3(-_roty);
		getTransformation(pos,rot);//Inverse*/
		//rot=rot*ry;
		std::cout<<"A("<<shoot<<","<<_world.attack().isAlive()<<")\n";
		_world.setAttack(0.04, 1.0f, _R ) ;
		std::cout<<"B("<<shoot<<","<<_world.attack().isAlive()<<")\n";/**/
	}
	const float d_camh=(0.2-1.0f+_world.currentLevel()[_trinagle].height-_camh)/30.0f;
	_camh+=d_camh;
	DEB_currenttrace=(DEB_currenttrace+1)%1000;
	DEB_trace[DEB_currenttrace]=Vector3f(_R.m01,_R.m11,_R.m21)*(1+_h*0.05);
}

void Player::setTriangle(int i)
{
	_trinagle=i;
	const TriangleGraph& triangleGraph=_world.currentLevel().triangleGraph();
	const Vector3f v=triangleGraph[i].centerPoint().normalize();
	const Vector3f a=triangleGraph[i].a;
	const Vector3f u=cross(v,v-a).normalize();
	const Vector3f w=cross(u,v);
	_R=Matrix3x3f(u.x, v.x, w.x, u.y, v.y, w.y ,u.z, v.z, w.z);
	_camh=0.2-1.0f+_world.currentLevel()[_trinagle].height;
	//_phi=atan2(p.y,p.x);
	//_teta=acos(p.z/p.length());
}
