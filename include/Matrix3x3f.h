#ifndef __Matrix_h_included__
#define __Matrix_h_included__

class Matrix3x3f
 {
	//m00 m01 m02
	//m10 m11 m12
	//m20 m21 m22
  public:  
   inline Matrix3x3f(float m00, float m01, float m02,
   					 float m10, float m11, float m12,
					 float m20, float m21, float m22)
					 :m00(m00),m01(m01),m02(m02),
					  m10(m10),m11(m11),m12(m12),
					  m20(m20),m21(m21),m22(m22){}
   inline Matrix3x3f():m00(1.0f),m01(0.0f),m02(0.0f),
					   m10(0.0f),m11(1.0f),m12(0.0f),
					   m20(0.0f),m21(0.0f),m22(1.0f){}        
   inline Matrix3x3f operator*(const Matrix3x3f& other)const
   {
		return Matrix3x3f(this->m00*other.m00 + this->m01*other.m10 + this->m02*other.m20,
						  this->m00*other.m01 + this->m01*other.m11 + this->m02*other.m21, 
						  this->m00*other.m02 + this->m01*other.m12 + this->m02*other.m22,
						  this->m10*other.m00 + this->m11*other.m10 + this->m12*other.m20, 
						  this->m10*other.m01 + this->m11*other.m11 + this->m12*other.m21, 
						  this->m10*other.m02 + this->m11*other.m12 + this->m12*other.m22,
						  this->m20*other.m00 + this->m21*other.m10 + this->m22*other.m20, 
						  this->m20*other.m01 + this->m21*other.m11 + this->m22*other.m21, 
						  this->m20*other.m02 + this->m21*other.m12 + this->m22*other.m22);
   }
   
   	inline Vector3f operator*(const Vector3f& other)const
   	{
		return Vector3f(other.x*this->m00 + other.y*this->m01 + other.z*this->m02,
						other.x*this->m10 + other.y*this->m11 + other.z*this->m12,
						other.x*this->m20 + other.y*this->m21 + other.z*this->m22);								
   	}
   	
   	inline void transpose()
	{
		float t;
		t=m10;m10=m01;m01=t;
		t=m20;m20=m02;m02=t;
		t=m21;m21=m12;m12=t;	
	}  
	             
   float m00,m01,m02,m10,m11,m12,m20,m21,m22;
 };
 
inline float det(const Matrix3x3f m)
{
	return m.m00*m.m11*m.m22+m.m01*m.m12*m.m20+m.m02*m.m10*m.m21-m.m02*m.m11*m.m20-m.m01*m.m10*m.m22-m.m00*m.m12*m.m21;
}


inline Matrix3x3f makeRotMatrix3x3(float rx, float ry, float rz)
{
	float sx=sin(rx); 	float cx=cos(rx);
	float sy=sin(ry);	float cy=cos(ry);
	float sz=sin(rz);	float cz=cos(rz); 	
	Matrix3x3f mx(1,0,0,0,cx,sx,0,-sx,cx);
	Matrix3x3f my(cy,0,-sy,0,1,0,sy,0,cy);
	Matrix3x3f mz(cz,sz,0,-sz,cz,0,0,0,1); 
	return mz*my*mx;
}

inline Matrix3x3f makeRotXMatrix3x3(float rx)
{
	float sx=sin(rx); 	float cx=cos(rx);	
	return Matrix3x3f(1,0,0,0,cx,sx,0,-sx,cx);
}

inline Matrix3x3f makeRotYMatrix3x3(float ry)
{
	float sy=sin(ry);	float cy=cos(ry);
	return Matrix3x3f(cy,0,-sy,0,1,0,sy,0,cy);
}

inline Matrix3x3f makeRotZMatrix3x3(float rz)
{
	float sz=sin(rz);	float cz=cos(rz); 
	return Matrix3x3f(cz,sz,0,-sz,cz,0,0,0,1); 
}
	
#endif
