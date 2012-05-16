#include <GFXEngine.h>
#include <Tile.h>
#include <Level.h>
#include <Shader.h>
#include <GL/glu.h>
#include <Matrix3x3f.h>
static Shader shader_per_pixel
(
	"varying vec3 n;\n"\
	"varying vec3 light;"\
	"varying vec4 color;\n"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"   color=gl_Color;\n"\
	"	n=gl_NormalMatrix*gl_Normal;\n"\
	"	gl_Position=ftransform();\n"\
	"	light=((gl_Vertex+vec4(0.0,0.0,1.0,1.0))*gl_ModelViewProjectionMatrix).xyz;"\
	"}"
	,
	"varying vec3 n;\n"\
	"varying vec3 light;\n"\
	"varying vec4 color;\n"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"	vec3 no=normalize(n);\n"\
	"//	if (n.z<0.0)no=-no;\n"\
	"	vec3 H=normalize(light);\n"\
	"	gl_FragColor=vec4(0.4,0.4,0.4,1.0)+max(dot(no,vec3(0.0,0.0,1.0)),0.0)*vec4(0.2,0.2,0.2,1.0)+pow(max(dot(no,H),0.0),2.0)*vec4(0.2,0.2,0.2,1.0);\n"\
	"	gl_FragColor.a=1.0f;\n"\
	"	gl_FragColor*=color;//vec4(color.r,color.g,color.b,color.a);\n"\
	"}"
);

static Shader shader_grass
(
	"varying vec3 n;\n"\
	"//varying vec3 light;"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"	n=gl_NormalMatrix*gl_Normal;\n"\
	"	gl_Position=ftransform();\n"\
	"	//light=((gl_Vertex+vec4(0.0,0.0,1.0,1.0))*gl_ModelViewProjectionMatrix).xyz;\n"\
	"}"
	,
	"varying vec3 n;\n"\
	"uniform vec3 light;\n"\
	"\n"\
	"void main()\n"\
	"{\n"\
	"	vec3 no=normalize(n);\n"\
	"//	if (n.z<0.0)no=-no;\n"\
	"	vec3 H=normalize(light);\n"\
	"	gl_FragColor=vec4(0.1,0.2,0.1,1.0)+max(dot(no,H),0.0)*vec4(0.05,0.2,0.05,1.0)+pow(max(dot(no,H),0.0),2.0)*vec4(0.0,0.01,0.0,1.0);\n"\
	"	gl_FragColor.a=1.0f;\n"\
	"}"
);

static bool inited=false;
static unsigned int _grassList=0;


static void initGrass()
{
	_grassList=glGenLists(1);
	srand(0);
	int i=0;
	const float hmin=0.1;
	const float hmax=0.2;
	//const float hmin=1.3;
	//const float hmax=2.5;
	const float f=sqrt(3.0f)/2.0f;
	const float dz=0.02f;
	const float dx=0.02f;
	//const float dz=1.25f;
	//const float dx=1.25f;
	const float nx=-dz/(dz*dz+dx*dx);
	const float nz=-dx/(dz*dz+dx*dx);
	Vector3f color(0.3f,1.0f,0.3f);
	glNewList(_grassList, GL_COMPILE);
		
		glColor3f(color.x,color.y,color.z);
		while(i<100)
		{
			const float h=0.0001f*((float)(rand()%10001))*(hmax-hmin)+hmin;
			const float x=0.0001f*((float)(rand()%10001));
			const float z=0.0001f*((float)(rand()%10001));	
			//const float wy=0.0001f*((float)(rand()%10001))*6.283185307179586476925286766559f;
			//const float wx=0.0001f*((float)(rand()%10001))*6.283185307179586476925286766559f;
			if ((x-0.0f)*f-z*0.5f <0.0f)continue;
			if ((x-1.0f)*f+z*0.5f >0.0f)continue;
			glBegin(GL_TRIANGLE_STRIP);
			/*0*/glNormal3f(nx,0,nz);
			/*0*/glVertex3f(x+dx,0.0f,z+dz);
			/*1*/glNormal3f(0,0,-1);
			/*1*/glVertex3f(x+0,0.0f,z+0);
			/*2*/glNormal3f(nx,0,nz);
			/*2*/glVertex3f(x+dx,h*0.65f,z+dz);
			/*3*/glNormal3f(0,0,-1);
			/*3*/glVertex3f(x+0,h*0.65f,z+0);
			/*4*/glVertex3f(x+0,h,z+0);
			/*5*/glNormal3f(-nx,0,nz);
			/*5*/glVertex3f(x-dx,h*0.65f,z+dz);
			/*6*/glNormal3f(0,0,-1);
			/*6*/glVertex3f(x+0,h*0.65f,z+0);
			/*8*/glNormal3f(-nx,0,nz);
			/*8*/glVertex3f(x-dx,0.0f,z+dz);
			/*7*/glNormal3f(0,0,-1);
			/*7*/glVertex3f(x+0,0.0f,z+0);
			
			glEnd();
			i++;
		}
		i=0;
		glBegin(GL_TRIANGLES);
		
		while(i<200)
		{
			const float x=0.0001f*((float)(rand()%10001));
			const float z=0.0001f*((float)(rand()%10001));	
			if ((x-0.0f)*f-z*0.5f <0.0f)continue;
			if ((x-1.0f)*f+z*0.5f >0.0f)continue;
			
			const float ox=0.000015-0.00003f*((float)(rand()%10001));
			const float oz=0.000015-0.00003f*((float)(rand()%10001));
			glNormal3f(ox,1,oz);	
			glVertex3f(x+0,0.01,0+z);
			glVertex3f(x+0.1,0.01,0+z);
			glVertex3f(x+0.05,0.01,f*0.1+z);
			i++;
		}
		glEnd();
	glEndList();	
}

static void init()
{
	if(inited)return;
	initGrass();
	inited=true;		
}

void GFXEngine::drawEmptyTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(0.5f,0.5f,0.5f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawGrasTile(const Level& level, int t, int lod)
{
	init();
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	const float f=sqrt(3.0f)/2.0f;
	
	
	const Vector3f u=(T[t].b-T[t].c);
	const float l=u.length();
	const Vector3f w=((T[t].a-T[t].c)-(T[t].b-T[t].c)*0.5f)/f;
	const Vector3f v=cross(T[t].b-T[t].a,T[t].c-T[t].a).normalize()*u.length();
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,m);
	Matrix3x3f ma(m[ 0],m[ 4],m[ 8],m[ 1],m[ 5],m[ 9],m[ 2],m[ 6],m[10]);
	Vector3f c=ma*Vector3f(0.0f,0.0f,1.0f)+Vector3f(m[12],m[13],m[14]);
	m[ 0]=u.x ; m[ 4]=v.x ; m[ 8]=w.x ; m[12]=T[t].c.x;
  	m[ 1]=u.y ; m[ 5]=v.y ; m[ 9]=w.y ; m[13]=T[t].c.y;
  	m[ 2]=u.z ; m[ 6]=v.z ; m[10]=w.z ; m[14]=T[t].c.z;
  	m[ 3]=0.0f; m[ 7]=0.0f; m[11]=0.0f; m[15]= 1.0f;
	glMultMatrixf(m);
	
	//ma.transpose();
	
	
	shader_grass.use();
	shader_grass.setUniform3f("light", c.x, c.y, c.z);
	glCallList(_grassList);
	glPopMatrix();
	shader_per_pixel.use();
	Vector3f color(0.5f,0.4f,0.3f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	Shader::unuse();
}

void GFXEngine::drawMountainTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(0.5f,0.4f,0.3f);
	shader_per_pixel.use();
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	Shader::unuse();
}

void GFXEngine::drawWaterTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(0.0f,0.2f,1.0f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawStonewallTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(0.2f,0.2f,0.2f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawWayTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(1.0f,0.5f,0.2f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawStreetTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(0.0f,0.0f,0.0f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawWoodbridgeTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(0.8f,0.3f,0.0f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawStonebridgeTile(const Level& level, int t, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(0.1f,0.1f,0.1f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawStartTile(const Level& level, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const int t=level.startTile();
	const float h = level[t].height;
	const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
	Vector3f color(1.0f,0.0f,0.0f);
	glBegin(GL_TRIANGLES);
		glColor3f(color.x,color.y,color.z);
		glNormal3f(n.x,n.y,n.z);
		glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
		glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
		glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
	glEnd();
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
}

void GFXEngine::drawPortalTile(const Level& level, int lod)
{
	const TriangleGraph &T=level.triangleGraph();
	const int tr[6]={level.portalTile(0),level.portalTile(1),level.portalTile(2),
					level.portalTile(3),level.portalTile(4),level.portalTile(5)};
	for (int i=0;i<6;i++)
	{
		const int t=tr[i];
		const float h = level[t].height;
		const Vector3f n=cross(T[t].b-T[t].a,T[t].c-T[t].a);
		Vector3f color(1.0f,0.0f,1.0f);
		glBegin(GL_TRIANGLES);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(T[t].a.x*h, T[t].a.y*h, T[t].a.z*h);
			glVertex3f(T[t].b.x*h, T[t].b.y*h, T[t].b.z*h);
			glVertex3f(T[t].c.x*h, T[t].c.y*h, T[t].c.z*h);
		glEnd();
		{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n0].height;
		if (h>H)
		{	
			const Vector3f p[4]={T[t].a*H,T[t].a*h,T[t].c*h,T[t].c*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n1].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].b*H,T[t].b*h,T[t].a*h,T[t].a*H};
			const Vector3f n=cross(p[0]-p[1],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	{
		const float H=level[T[t].n2].height;
		if (h>H)
		{
			const Vector3f p[4]={T[t].c*H,T[t].c*h,T[t].b*h,T[t].b*H};
			const Vector3f n=cross(p[1]-p[0],p[3]-p[0]).normalize(); 
			glBegin(GL_QUADS);
			glColor3f(color.x,color.y,color.z);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f(p[0].x,p[0].y,p[0].z);glVertex3f(p[1].x,p[1].y,p[1].z);
			glVertex3f(p[2].x,p[2].y,p[2].z);glVertex3f(p[3].x,p[3].y,p[3].z);
			glEnd();
		}
	}
	}
}
