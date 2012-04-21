#ifndef VEKTOR3F_H
#define VEKTOR3F_H 

#include <math.h>

class Vector3f
 {
  public:  
   inline Vector3f(float x, float y, float z):x(x),y(y),z(z){}
   inline Vector3f():x(0.0f),y(0.0f),z(0.0f){}         
   //Komponentenweise Addition  
   inline Vector3f operator+(const Vector3f& other)const{return Vector3f(other.x+this->x,other.y+this->y,other.z+this->z);}
   inline Vector3f& operator+=(const Vector3f& other){x+=other.x;y+=other.y;z+=other.z;return *this;}
   //Komponentenweise Subtraktion
   inline Vector3f operator-(const Vector3f& other) const{return Vector3f(this->x-other.x,this->y-other.y,this->z-other.z);}
   inline Vector3f& operator-=(const Vector3f& other){x-=other.x;y-=other.y;z-=other.z;return *this;}
   //Multiplikation mit einem Skalaren
   inline Vector3f operator*(const float& other)const{return Vector3f(other*this->x,other*this->y,other*this->z);}
   inline Vector3f& operator*=(const float& other){x*=other;y*=other;z*=other;return *this;}
   //Komponentenweise Multipliokation
   inline Vector3f operator*(const Vector3f& other) const{return Vector3f(other.x*this->x,other.y*this->y,other.z*this->z);}
   inline Vector3f& operator*=(const Vector3f& other){x*=other.x;y*=other.y;return *this;}
   //Division mit einem Skalaren
   inline Vector3f operator/(const float& other) const{return Vector3f(this->x/other,this->y/other,this->z/other);}
   inline Vector3f& operator/=(const float& other){x/=other;y/=other;z/=other;return *this;}
   //Komponentenweise Division
   inline Vector3f operator/(const Vector3f& other) const{return Vector3f(this->x/other.x,this->y/other.y,this->z/other.z);}
   inline Vector3f& operator/=(const Vector3f& other){x/=other.x;y/=other.y;z/=other.z;return *this;}
   //Weiter Funktionen 
   inline float length()const {return sqrt(x*x+y*y+z*z);}
   inline float squareLength()const {return x*x+y*y+z*z;}
   inline Vector3f normalize(){float l=length();if (l!=0){x/=l;y/=l;z/=l;};return *this;}
   inline Vector3f norm()const{float l=length();if (l==0)return Vector3f();return Vector3f(x/l,y/l,z/l);} 
   inline bool operator==(const Vector3f& other)const{return (this->x==other.x&&this->y==other.y&&this->z==other.z);} 
   inline bool operator!=(const Vector3f& other)const{return (this->x!=other.x||this->y!=other.y||this->z!=other.z);}
   //Komponenten                
   float x;
   float y;
   float z;
 };
//Skalarprodukt
inline float dot(const Vector3f& A, const Vector3f& B){return A.x*B.x+A.y*B.y+A.z*B.z;} 
inline Vector3f cross(const Vector3f& A, const Vector3f& B)
 {
  return Vector3f(A.y*B.z - A.z*B.y,
                  A.z*B.x - A.x*B.z,
                  A.x*B.y - A.y*B.x);
 }
inline bool same(const Vector3f& A, const Vector3f& B,const float& e)
{
	return (A.x-e<B.x && B.x<A.x+e &&
	        A.y-e<B.y && B.y<A.y+e && 
			A.z-e<B.z && B.z<A.z+e);
}
inline Vector3f findNormalVector(const Vector3f v)
{
	//float f[3];
	int i=0;
	float f=v.x*v.x;
	if (f>v.y*v.y){f=v.y;i=1;}
	if (f>v.z*v.z){f=v.z;i=2;}
	switch (i)
	{
		case 0: return Vector3f(0.0f, v.z , -v.y);
		case 1: return Vector3f(v.z , 0.0f, -v.x);
		case 2: return Vector3f(v.y , -v.x, 0.0f);
	};	
	return Vector3f();
}
#endif
