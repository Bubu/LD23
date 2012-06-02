#ifndef VEKTOR4F_H
#define VEKTOR4F_H 

#include <math.h>

class Vector4f
 {
  public:  
   inline Vector4f(float x, float y, float z, float w):x(x),y(y),z(z),w(w){}
   inline Vector4f():x(0.0f),y(0.0f),z(0.0f),w(0.0f){}         
   //Komponentenweise Addition  
   inline Vector4f operator+(const Vector4f& other)const{return Vector4f(other.x+this->x,other.y+this->y,other.z+this->z,other.w+this->w);}
   inline Vector4f& operator+=(const Vector4f& other){x+=other.x;y+=other.y;z+=other.z;w+=other.w;return *this;}
   //Komponentenweise Subtraktion
   inline Vector4f operator-(const Vector4f& other) const{return Vector4f(this->x-other.x,this->y-other.y,this->z-other.z,this->w-other.w);}
   inline Vector4f& operator-=(const Vector4f& other){x-=other.x;y-=other.y;z-=other.z;w-=other.w;return *this;}
   //Multiplikation mit einem Skalaren
   inline Vector4f operator*(const float& other)const{return Vector4f(other*this->x,other*this->y,other*this->z,other*this->w);}
   inline Vector4f& operator*=(const float& other){x*=other;y*=other;z*=other;w*=other;return *this;}
   //Komponentenweise Multipliokation
   inline Vector4f operator*(const Vector4f& other) const{return Vector4f(other.x*this->x,other.y*this->y,other.z*this->z,other.w*this->w);}
   inline Vector4f& operator*=(const Vector4f& other){x*=other.x;y*=other.y;z*=other.z;w*=other.w;return *this;}
   //Division mit einem Skalaren
   inline Vector4f operator/(const float& other) const{return Vector4f(this->x/other,this->y/other,this->z/other,this->w/other);}
   inline Vector4f& operator/=(const float& other){x/=other;y/=other;z/=other;w/=other;return *this;}
   //Komponentenweise Division
   inline Vector4f operator/(const Vector4f& other) const{return Vector4f(this->x/other.x,this->y/other.y,this->z/other.z,this->w/other.w);}
   inline Vector4f& operator/=(const Vector4f& other){x/=other.x;y/=other.y;z/=other.z;w/=other.w;return *this;}
   //Weiter Funktionen 
   inline float length()const {return sqrt(x*x+y*y+z*z+w*w);}
   inline float squareLength()const {return x*x+y*y+z*z+w*w;}
   inline Vector4f normalize(){float l=length();if (l!=0){x/=l;y/=l;z/=l;w/=l;};return *this;}
   inline Vector4f norm()const{float l=length();if (l==0)return Vector4f();return Vector4f(x/l,y/l,z/l,w/l);} 
   inline bool operator==(const Vector4f& other)const{return (this->x==other.x&&this->y==other.y&&this->z==other.z&&this->w==other.w);} 
   inline bool operator!=(const Vector4f& other)const{return (this->x!=other.x||this->y!=other.y||this->z!=other.z||this->w!=other.w);}
   //Komponenten                
   float x;
   float y;
   float z;
   float w;
 };
//Skalarprodukt
inline float dot(const Vector4f& A, const Vector4f& B){return A.x*B.x+A.y*B.y+A.z*B.z+A.w*B.w;} 
//inline Vector3f cross(const Vector3f& A, const Vector3f& B)
// {
//  return Vector3f(A.y*B.z - A.z*B.y,
//                  A.z*B.x - A.x*B.z,
//                  A.x*B.y - A.y*B.x);
// }
inline bool same(const Vector4f& A, const Vector4f& B,const float& e)
{
	return (A.x-e<B.x && B.x<A.x+e &&
	        A.y-e<B.y && B.y<A.y+e && 
			A.z-e<B.z && B.z<A.z+e && 
			A.w-e<B.w && B.w<A.w+e);
}
//inline Vector3f findNormalVector(const Vector3f v)
//{
//	//float f[3];
//	int i=0;
//	float f=v.x*v.x;
//	if (f>v.y*v.y){f=v.y;i=1;}
//	if (f>v.z*v.z){f=v.z;i=2;}
//	switch (i)
//	{
//		case 0: return Vector3f(0.0f, v.z , -v.y);
//		case 1: return Vector3f(v.z , 0.0f, -v.x);
//		case 2: return Vector3f(v.y , -v.x, 0.0f);
//	};	
//	return Vector3f();
//}
#endif
