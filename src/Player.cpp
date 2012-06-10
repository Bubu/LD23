#include <Player.h>
#include <World.h>
#include <TriangleGraph.h>
#include <iostream>
static const float pi=3.1415926535897932384626433832795f ;
static const float _maxJumpDuration=1.0f;

static const float _hmax=2.0f;
static const float _g=8.0f*_hmax/(_maxJumpDuration*_maxJumpDuration);
static const float _chargeTime=0.15f;
//static const float _vElevatorMax=100000;


Player::Player(Genie& genie, World& world):
	_genie(genie),/*_teta(pi/2.0),_phi(pi),_roty(0.0f),*/_h(0.0f),_v(0.0f),//_vElevator(_vElevatorMax),
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

void Player::_jump(bool jump, float t)
{
	const int edge=(jump==_jumpButtonState)?0:((_jumpButtonState)?-1:+1);
	const float base=(_world.currentLevel()[_trinagle].height-1)/0.05; 
	_jumpButtonState=jump;
	//_jumpCharge
//if (edge!=0)std::cout<<"Edge: "<<edge<<"\n";
	if (_jumpState==0)
	{
		if (base+0.00001>_h && edge==+1)_jumpState=1;
	}
	if (_jumpState==1)
	{
		if (edge>=0)
		{
			_jumpCharge+=t;
			const float oldJumpCharge=_jumpCharge;
			if (_jumpCharge>_chargeTime)
			{
				_jumpCharge=_chargeTime;
				_jumpState=2;	
				//DEB_sumtime=1.0f;
			}
			t-=	_jumpCharge-oldJumpCharge;
		}
		else _jumpState=2;	
	}
	if (_jumpState==2)
	{
		//_h=base;
		_v=sqrt(2.0f*_g*(_jumpCharge/_chargeTime)*_hmax);
		_jumpState++;
//std::cout<<"start Jump:"<<_v<<" "<<_v*t-0.5f*_g*t*t<<" "<< _h+_v*t-0.5f*_g*t*t	-base<<" \n";				
	}
	//if (_jumpState==2)
	//{
			
	//}
	_h+=_v*t-0.5f*_g*t*t;
	//_h+=_v*t;
	if (_v>0)DEB_sumtime+=_h-base;
//if (_jumpState)std::cout<<"h: "<< _h<<"\n";
	_v-=_g*t;
	
	
	
	//if (_h<=base && _vElevator==_vElevatorMax && f>0.0f)_v=_vMax;
//	_v-=_g*t;
//	const float newv=(_vElevator<f*t*(_vMax-_v))?_vElevator:f*t*(_vMax-_v);
//	_vElevator-=newv;
//	_v+=newv/t;
	//_v-=_g*t;
	//const float Emax=0.5f*(_vMax-_v)*(_vMax-_v);
	//const float dE=(_vElevator<f*t*Emax)?_vElevator:f*t*Emax;
	//const float dv=2.0f*sqrt(dE);
	//_vElevator-=dE;
	//_v+=dv;
	//const float a=(_vElevator<f*t)?_vElevator:f*t;
	//_vElevator-=a;
	//_v+=(1.0)*a*_g-_g*t;
	
	//if (t>0.0f)
	//_chargeTime
	//_h+=_v*t;//-9.81f*f*f;
	//std::cout<<"_jump:("<<_h<<","<<_v<<")["<<_vElevator<<"]["<<newv<<"]\n";
	
	if (_h<base )
	{
		_h=base;
		//_vElevator=_vElevatorMax;
		_v=0.0f;
		if (_jumpState!=1)
		{
			_jumpState=0;
			_jumpCharge=0.0f;
		}
		if (DEB_sumtime>0.0f)std::cout<<"_jump:["<<DEB_sumtime<<"]("<<base<<")\n";
		DEB_sumtime=0.0f;
		
	}
}

void Player::tick(float time, float move, bool jump, float roty, bool shoot)
{
	/*
	float move=0.0f; if (down)move-=t*0.01f; if (up)   move+=t*0.01f;
	float roty=0.0f; if (left)roty-=t*0.03f; if (right)roty+=t*0.03f;
	float jump=0.0f; if (jumped)jump+=t;
	*/
	_jump(jump,time);
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
