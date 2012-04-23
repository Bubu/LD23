#include <Player.h>
static const float pi=3.1415926535897932384626433832795f ;
Player::Player(Genie& genie):_genie(genie),_teta(pi/2.0),_phi(pi),_roty(0.0f)
{
	
	
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
void Player::addRoty(float f)
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
