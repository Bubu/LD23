#include <Genie.h>

Genie::Genie()
{
	//Matrix3x3f _rotation;
	//Vector3f _translate;
	//_scale=0.05*1.0f;
	_swirlHeight=0.05*1.0f;
	_swirlRadius=0.05*0.8f;
	_shoulderHeight=0.05*1.75f;
	_shoulderWidth=0.05*0.3f;
	_upperArmLength=0.05*0.3f;
	_forearmLength=0.05*0.4f;
	_headHeight=0.05*0.25f;
	
	_armAngle[0][0]=-30.0f; _armAngle[1][0]=-30.0f;
	_armAngle[0][1]=-10.0f; _armAngle[1][1]=+10.0f;
	_armAngle[0][2]=+20.0f; _armAngle[1][2]=-20.0f;
	
	_neckAngle=5.0f;
	
	_headRadius=0.05*0.15;
	_shoulderRadius=0.05*0.15;
	_handRadius=0.05*0.10;
	_elbowRadius=0.05*0.05;
	_bellyRadius=0.05*1.0f;
}

void Genie::getPositions(Vector3f points[10])const
{
	const float k=0.0174532925199432957692369076848860f; 
	points[0]=Vector3f();
	points[1]=Vector3f(0.0f,_swirlHeight,0.0f);
	points[2]=Vector3f(0.0f,_shoulderHeight,0.0f);
	points[3]=points[2]+Vector3f(0.0f,_headHeight*cos(k*_neckAngle),_headHeight*sin(k*_neckAngle));
	points[4]=points[2]+Vector3f(+_shoulderWidth,0.0f,0.0f);
	points[7]=points[2]+Vector3f(-_shoulderWidth,0.0f,0.0f);
	
	Matrix3x3f r00= makeRotXMatrix3x3(k*_armAngle[0][0])*makeRotYMatrix3x3(k*_armAngle[0][1]);
	points[5]=points[4]+r00*Vector3f(0.0f,0.0f,_upperArmLength);
	Matrix3x3f r01= r00*makeRotYMatrix3x3(k*_armAngle[0][2]);
	points[6]=points[5]+r01*Vector3f(0.0f,0.0f,_forearmLength);
	
	Matrix3x3f r10= makeRotXMatrix3x3(k*_armAngle[1][0])*makeRotYMatrix3x3(k*_armAngle[1][1]);
	points[8]=points[7]+r10*Vector3f(0.0f,0.0f,_upperArmLength);
	Matrix3x3f r11= r10*makeRotYMatrix3x3(k*_armAngle[1][2]);
	points[9]=points[8]+r11*Vector3f(0.0f,0.0f,_forearmLength);
	
}
