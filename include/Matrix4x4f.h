#ifndef __Matrix4x4_h_included__
#define __Matrix4x4_h_included__
#include <Vector3f.h>
#include <Vector4f.h>
#include <Matrix3x3f.h>
class Matrix4x4f;
float det(const Matrix4x4f m);
class Matrix4x4f
 {
	//m00 m01 m02 m03
	//m10 m11 m12 m13
	//m20 m21 m22 m23
	//m30 m31 m32 m33
  public:  
   inline Matrix4x4f(float m00, float m01, float m02, float m03,
   					 float m10, float m11, float m12, float m13,
					 float m20, float m21, float m22, float m23,
					 float m30, float m31, float m32, float m33)
					 :m00(m00),m01(m01),m02(m02),m03(m03),
					  m10(m10),m11(m11),m12(m12),m13(m13),
					  m20(m20),m21(m21),m22(m22),m23(m23),
					  m30(m30),m31(m31),m32(m32),m33(m33){}
	inline Matrix4x4f(const Matrix3x3f& _m)
					 :m00(_m.m00),m01(_m.m01),m02(_m.m02),m03( 0.0f ),
					  m10(_m.m10),m11(_m.m11),m12(_m.m12),m13( 0.0f ),
					  m20(_m.m20),m21(_m.m21),m22(_m.m22),m23( 0.0f ),
					  m30( 0.0f ),m31( 0.0f ),m32( 0.0f ),m33( 1.0f ){}
	inline Matrix4x4f(const Matrix3x3f& _m, const Vector3f& _v)
					 :m00(_m.m00),m01(_m.m01),m02(_m.m02),m03( _v.x ),
					  m10(_m.m10),m11(_m.m11),m12(_m.m12),m13( _v.y ),
					  m20(_m.m20),m21(_m.m21),m22(_m.m22),m23( _v.z ),
					  m30( 0.0f ),m31( 0.0f ),m32( 0.0f ),m33( 1.0f ){}
	inline Matrix4x4f(const Vector3f& _v)
					 :m00( 1.0f ),m01( 0.0f ),m02( 0.0f ),m03( _v.x ),
					  m10( 0.0f ),m11( 1.0f ),m12( 0.0f ),m13( _v.y ),
					  m20( 0.0f ),m21( 0.0f ),m22( 1.0f ),m23( _v.z ),
					  m30( 0.0f ),m31( 0.0f ),m32( 0.0f ),m33( 1.0f ){}
	inline Matrix4x4f(float _m[16])
	{
  		m00=_m[ 0]; m01=_m[ 4]; m02=_m[ 8]; m03=_m[12];
		m10=_m[ 1]; m11=_m[ 5]; m12=_m[ 9]; m13=_m[13];
		m20=_m[ 2]; m21=_m[ 6]; m22=_m[10]; m23=_m[14];
		m30=_m[ 3]; m31=_m[ 7]; m32=_m[11]; m33=_m[15];
	}
   inline Matrix4x4f():m00(1.0f),m01(0.0f),m02(0.0f),m03(0.0f),
					   m10(0.0f),m11(1.0f),m12(0.0f),m13(0.0f),
					   m20(0.0f),m21(0.0f),m22(1.0f),m23(0.0f),
					   m30(0.0f),m31(0.0f),m32(0.0f),m33(1.0f){}        
   inline Matrix4x4f operator*(const Matrix4x4f& other)const
   {
		return Matrix4x4f(this->m00*other.m00 + this->m01*other.m10 + this->m02*other.m20 + this->m03*other.m30,
						  this->m00*other.m01 + this->m01*other.m11 + this->m02*other.m21 + this->m03*other.m31, 
						  this->m00*other.m02 + this->m01*other.m12 + this->m02*other.m22 + this->m03*other.m32,
						  this->m00*other.m03 + this->m01*other.m13 + this->m02*other.m23 + this->m03*other.m33,
						  this->m10*other.m00 + this->m11*other.m10 + this->m12*other.m20 + this->m13*other.m30, 
						  this->m10*other.m01 + this->m11*other.m11 + this->m12*other.m21 + this->m13*other.m31, 
						  this->m10*other.m02 + this->m11*other.m12 + this->m12*other.m22 + this->m13*other.m32,
						  this->m10*other.m03 + this->m11*other.m13 + this->m12*other.m23 + this->m13*other.m33,
						  this->m20*other.m00 + this->m21*other.m10 + this->m22*other.m20 + this->m23*other.m30,  
						  this->m20*other.m01 + this->m21*other.m11 + this->m22*other.m21 + this->m23*other.m31,  
						  this->m20*other.m02 + this->m21*other.m12 + this->m22*other.m22 + this->m23*other.m32,
						  this->m20*other.m03 + this->m21*other.m13 + this->m22*other.m23 + this->m23*other.m33,
						  this->m30*other.m00 + this->m31*other.m10 + this->m32*other.m30 + this->m33*other.m30,  
						  this->m30*other.m01 + this->m31*other.m11 + this->m32*other.m31 + this->m33*other.m31,  
						  this->m30*other.m02 + this->m31*other.m12 + this->m32*other.m32 + this->m33*other.m32,
						  this->m30*other.m03 + this->m31*other.m13 + this->m32*other.m33 + this->m33*other.m33  );
   }
   
   	inline Vector4f operator*(const Vector4f& other)const
   	{
		return Vector4f(other.x*this->m00 + other.y*this->m01 + other.z*this->m02 + other.w*this->m03,
						other.x*this->m10 + other.y*this->m11 + other.z*this->m12 + other.w*this->m13,
						other.x*this->m20 + other.y*this->m21 + other.z*this->m22 + other.w*this->m23,
						other.x*this->m30 + other.y*this->m31 + other.z*this->m32 + other.w*this->m33);								
   	}
   	
   	inline void transpose()
	{
		float t;
		t=m10;m10=m01;m01=t;
		t=m20;m20=m02;m02=t;
		t=m30;m30=m03;m03=t;
		t=m21;m21=m12;m12=t;
		t=m31;m31=m13;m13=t;
		t=m32;m32=m23;m23=t;	
	}  
	inline Matrix4x4f invert()
	{
		const float d=det(*this);
		const float d00=det(Matrix3x3f(m11,m12,m13,m21,m22,m23,m31,m32,m33));
		const float d10=det(Matrix3x3f(m10,m12,m13,m20,m22,m23,m30,m32,m33));
		const float d20=det(Matrix3x3f(m10,m11,m13,m20,m21,m23,m30,m31,m33));
		const float d30=det(Matrix3x3f(m10,m11,m12,m20,m21,m22,m30,m31,m32));
		const float d01=det(Matrix3x3f(m01,m02,m03,m21,m22,m23,m31,m32,m33));
		const float d11=det(Matrix3x3f(m00,m02,m03,m20,m22,m23,m30,m32,m33));
		const float d21=det(Matrix3x3f(m00,m01,m03,m20,m21,m23,m30,m31,m33));
		const float d31=det(Matrix3x3f(m00,m01,m02,m20,m21,m22,m30,m31,m32));
		const float d02=det(Matrix3x3f(m01,m02,m03,m11,m12,m13,m31,m32,m33));
		const float d12=det(Matrix3x3f(m00,m02,m03,m10,m12,m13,m30,m32,m33));
		const float d22=det(Matrix3x3f(m00,m01,m03,m10,m11,m13,m30,m31,m33));
		const float d32=det(Matrix3x3f(m00,m01,m02,m10,m11,m12,m30,m31,m32));
		const float d03=det(Matrix3x3f(m01,m02,m03,m11,m12,m13,m21,m22,m23));
		const float d13=det(Matrix3x3f(m00,m02,m03,m10,m12,m13,m20,m22,m23));
		const float d23=det(Matrix3x3f(m00,m01,m03,m10,m11,m13,m20,m21,m23));
		const float d33=det(Matrix3x3f(m00,m01,m02,m10,m11,m12,m20,m21,m22));
		m00=d00/d; m01=d01/d; m02=d02/d; m03=d03/d;
		m10=d10/d; m11=d11/d; m12=d12/d; m13=d13/d;
		m20=d20/d; m21=d21/d; m22=d22/d; m23=d23/d;
		m30=d30/d; m31=d31/d; m32=d02/d; m33=d03/d; 
		return *this;
	}          
   float m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,m23,m30,m31,m32,m33;
 };
 
inline float det(const Matrix4x4f m)
{
	return 	- m.m00*m.m11*m.m23*m.m32 + m.m00*m.m12*m.m23*m.m31 + m.m00*m.m11*m.m22*m.m33 - m.m00*m.m12*m.m21*m.m33 
			+ m.m00*m.m13*m.m21*m.m32 - m.m00*m.m13*m.m22*m.m31 + m.m01*m.m10*m.m23*m.m32 - m.m01*m.m12*m.m23*m.m30 
			- m.m01*m.m10*m.m22*m.m33 + m.m01*m.m12*m.m20*m.m33 - m.m01*m.m13*m.m20*m.m32 + m.m01*m.m13*m.m22*m.m30 
			- m.m02*m.m10*m.m23*m.m31 + m.m02*m.m11*m.m23*m.m30 + m.m02*m.m10*m.m21*m.m33 - m.m02*m.m11*m.m20*m.m33 
			+ m.m02*m.m13*m.m20*m.m31 - m.m02*m.m13*m.m21*m.m30 - m.m03*m.m10*m.m21*m.m32 + m.m03*m.m10*m.m22*m.m31 
			+ m.m03*m.m11*m.m20*m.m32 - m.m03*m.m11*m.m22*m.m30 - m.m03*m.m12*m.m20*m.m31 + m.m03*m.m12*m.m21*m.m30;
}

inline Matrix4x4f makeRotMatrix4x4(float rx, float ry, float rz)
{
	float sx=sin(rx); 	float cx=cos(rx);
	float sy=sin(ry);	float cy=cos(ry);
	float sz=sin(rz);	float cz=cos(rz); 	
	Matrix4x4f mx(1,0,0,0,0,cx,sx,0,0,-sx,cx,0,0,0,0,1);
	Matrix4x4f my(cy,0,-sy,0,0,1,0,0,sy,0,cy,0,0,0,0,1);
	Matrix4x4f mz(cz,sz,0,0,-sz,cz,0,0,0,0,1,0,0,0,0,1); 
	return mz*my*mx;
}

inline Matrix4x4f makeRotXMatrix4x4(float rx)
{
	float sx=sin(rx); 	float cx=cos(rx);	
	return Matrix4x4f(1,0,0,0,0,cx,sx,0,0,-sx,cx,0,0,0,0,1);
}

inline Matrix4x4f makeRotYMatrix4x4(float ry)
{
	float sy=sin(ry);	float cy=cos(ry);
	return Matrix4x4f(cy,0,-sy,0,0,1,0,0,sy,0,cy,0,0,0,0,1);
}

inline Matrix4x4f makeRotZMatrix4x4(float rz)
{
	float sz=sin(rz);	float cz=cos(rz); 
	return Matrix4x4f(cz,sz,0,0,-sz,cz,0,0,0,0,1,0,0,0,0,1); 
}
	
#endif
