#include <Genie.h>
#include <Shader.h>
#include <GL/glu.h>
/*	float _scale;
	float _swirlHeight;
	float _shoulderHeight;
	float _shoulderWidth;
	float _upperArmLength;
	float _forearmLength;
	float _headHeight;
	
	float _armAngle[2][3];
	float _neckAngle;*/
void Genie::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		//glTranslatef(_translate.x,_translate.y,_translate.z);
		const Matrix3x3f& r=_rotation;
		const Vector3f& t=_translate;
		const float& s=_scale;
		float m[16]={	s*r.m00,s*r.m10,s*r.m20,0.0f,
						s*r.m01,s*r.m11,s*r.m21,0.0f,
						s*r.m02,s*r.m12,s*r.m22,0.0f,
						t.x    ,t.y    ,t.z    ,1.0f};
		glMultMatrixf(m);
		Vector3f points[10]; getPositions(points);
		
		glPointSize(11.0f);
		glBegin(GL_POINTS);
			for (int i=0;i<10;i++)glVertex3f(points[i].x,points[i].y,points[i].z);
		glEnd();
		glPointSize(1.0f);
		glBegin(GL_LINES);
			glVertex3f(points[0].x,points[0].y,points[0].z); glVertex3f(points[1].x,points[1].y,points[1].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[1].x,points[1].y,points[1].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[3].x,points[3].y,points[3].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[4].x,points[4].y,points[4].z);
			glVertex3f(points[2].x,points[2].y,points[2].z); glVertex3f(points[7].x,points[7].y,points[7].z);
			glVertex3f(points[4].x,points[4].y,points[4].z); glVertex3f(points[5].x,points[5].y,points[5].z);
			glVertex3f(points[6].x,points[6].y,points[6].z); glVertex3f(points[5].x,points[5].y,points[5].z);
			glVertex3f(points[7].x,points[7].y,points[7].z); glVertex3f(points[8].x,points[8].y,points[8].z);
			glVertex3f(points[9].x,points[9].y,points[9].z); glVertex3f(points[8].x,points[8].y,points[8].z);		
		glEnd();
	glPopMatrix();
}
