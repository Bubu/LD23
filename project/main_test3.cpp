#include <SDL/SDL.h>
#include <Init.h>
#include <iostream>
#include <Shader.h>
#include <math.h>
#include <World.h>
#include <Genie.h>
#include <GFXEngine.h>
#include <Player.h>
#include <IrrKlang/irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

GFXEngine gfxEngine;
World world;
Genie genie; 
Player player(genie,world);
static Shader shader_per_pixel(
"varying vec3 n;"\
"varying vec4 c;"\
"varying vec3 pos;"\
""\
"void main()"\
"{"\
"	n=gl_NormalMatrix*gl_Normal;"\
"	gl_Position=ftransform();"\
"	pos=gl_Position.xyz;"\
"	c=gl_Color;"\
"}"

,
"varying vec3 n;"\
"varying vec3 pos;"\
"varying vec4 c;"\
""\
"void main()"\
"{"\
"	vec3 no=normalize(n);"\
"	if (n.z<0.0)no=-no;"\
"	vec3 H=normalize(vec3(0.0,0.0,1.0)+pos);"\
"	gl_FragColor=vec4(0.125,0.125,0.125,1.0)+max(dot(no,vec3(0.0,0.0,1.0)),0.0)*vec4(0.8,0.8,0.8,1.0)/*+pow(max(dot(no,H),0.0),64.0)*/;"\
"	gl_FragColor*=c;"\
"}"
);

float rotx=0;
float roty=0;
float rotz=0;
const float pi=3.1415926535897932384626433832795f;
float u=0;
float v=0;
float scale=0.01;
bool frameLimit=true;
bool wireframe=false;
Uint32 time_;
Uint32 fps=60;
//Genie genie;
//TriangleGraph tg (1);

static Uint32 getDelay()
{
	Uint32 _time=SDL_GetTicks();
	Uint32 _optTime=time_+1000/fps;
	int _Delay=_optTime-_time;
	if (_Delay<0)
	{
		_Delay=0;
		_optTime=_time;
	}
	time_=_optTime;
	//std::cout<<_Delay<<"\n";
	return _Delay;
}

static void draw ()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	if(!wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	SDL_Surface *screen = SDL_GetVideoSurface();
	const int width=screen->w;
	const int height=screen->h;
	const float fovy=60.0f;
    glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluPerspective(fovy,((float)width)/((float)height), 1.0f, 1000.0f);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  	
	glClearColor(0.5, 0.75, 0.5, 1);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  	if(frameLimit)glColor3f(1,0,0);else glColor3f(1,1,1);
  	glTranslated (0, 0, -tan(fovy*0.017453292519943295769236907684886f)/scale);	
  	glRotated (rotx, 1.0f, 0.0f, 0.0f);
  	glRotated (roty, 0.0f, 1.0f, 0.0f);
  	glRotated (rotz, 0.0f, 0.0f, 1.0f);
 /* 	const float su=sin(u); const float cu=cos(u);
  	const float sv=sin(v); const float cv=cos(v);
  	const Vector3f U(cu*cv,cu*sv,-su);
  	const Vector3f V(su*cv,su*sv,cu);
  	const Vector3f W(-sv,cv,0.0f);
  	
  	Matrix3x3f r=Matrix3x3f(U.x, U.y, U.z,
   					 		V.x, V.y, V.z,
					 		W.x, W.y, W.z);
	r.transpose();
	genie.setRotation(r);
	
	genie.setTranlation(V);
	genie.setScale(1.0);
  	
  	
  	{
		glColor4f(1.0, 1.0, 1.0, 1);
		glBegin(GL_LINES);
			glVertex3f(V.x,V.y,V.z);glVertex3f(V.x+U.x,V.y+U.y,V.z+U.z);
			glVertex3f(V.x,V.y,V.z);glVertex3f(V.x+V.x,V.y+V.y,V.z+V.z);
			glVertex3f(V.x,V.y,V.z);glVertex3f(V.x+W.x,V.y+W.y,V.z+W.z);
		glEnd();
	}
	shader_per_pixel.use();
  	{
		//const Matrix3x3f& r=_rotation;
		//const Vector3f& t=V;
		//const float& s=1.0;
		glColor4f(0.0, 0.0, 1.0, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			//glTranslatef(_translate.x,_translate.y,_translate.z);
			//const Matrix3x3f& r=_rotation;
			const Vector3f& t=V;
			const float& s=1.0;
			float m[16]={	s*r.m00,s*r.m10,s*r.m20,0.0f,
							s*r.m01,s*r.m11,s*r.m21,0.0f,
							s*r.m02,s*r.m12,s*r.m22,0.0f,
							t.x    ,t.y    ,t.z    ,1.0f};
		
			glMultMatrixf(m);
			glPointSize(11.0f);
			glBegin(GL_POINTS);
				glVertex3f(0,0,0);
			glEnd();
		glPopMatrix();
		glColor4f(1.0, 0.0, 0.0, 1);
		std::cout	<<"t:("<<t.x<<","<<t.y<<","<<t.z<<")\n"
					<<"r:|"<<r.m00<<" "<<r.m01<<" "<<r.m02<<"|\n"
					<<"  |"<<r.m10<<" "<<r.m11<<" "<<r.m12<<"|\n"
					<<"  |"<<r.m20<<" "<<r.m21<<" "<<r.m22<<"|\n"
					<<"det(r)="<<det(r)<<"\n";
	}
  	
	

	genie.draw();
	Shader::unuse();*/
	gfxEngine.drawGenie(genie, player);
  	SDL_GL_SwapBuffers();
}


int main (int argc, char *argv[])
{
    
    int done=0;
    bool _fullScreenToggled=false;
	std::cout<<"init: "<<init("Test01", 0, 800, 600)<<std::endl;
	
	if(!shader_per_pixel.compileAndLink()  )std::cout<<("White-Shader-Error:"  +shader_per_pixel.getLog()  )<<"\n";
    //Sound
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
	irrklang::ISound* SoundEffect = SoundEngine->play2D("Sound_Water0001.wav",true,true,false,irrklang::ESM_AUTO_DETECT, true);
	SoundEffect->setIsPaused(true);
	//Sound_Water0001.wav
	time_=SDL_GetTicks();
	bool up_pressed=false;
    bool down_pressed=false;
	bool right_pressed=false;
	bool left_pressed=false;
	bool plus_pressed=false;
	bool minus_pressed=false;
	bool u_pressed=false;
	bool v_pressed=false;
	while (!done)
    {
		//SDL_Delay(50);
        
		if (frameLimit)SDL_Delay(getDelay());
		else time_=SDL_GetTicks();
		SDL_Event event;
        /* Check for events */
        while (SDL_PollEvent (&event))
        {
			
			
            switch (event.type)
            {
            case SDL_KEYUP:
				switch (event.key.keysym.sym) 
				{
					case SDLK_UP:      up_pressed=false;    break;
					case SDLK_DOWN:    down_pressed=false;  break;
					case SDLK_RIGHT:   right_pressed=false; break;
					case SDLK_LEFT:    left_pressed=false;  break;
					case SDLK_w:       up_pressed=false;    break;
					case SDLK_s:       down_pressed=false;  break;
					case SDLK_d:       right_pressed=false; break;
					case SDLK_a:       left_pressed=false;  break;
					case SDLK_u:       u_pressed=false;  break;
					case SDLK_v:       v_pressed=false;  break;
					case SDLK_PLUS:
					case SDLK_KP_PLUS: plus_pressed=false;  break;
					case SDLK_MINUS:
					case SDLK_KP_MINUS:minus_pressed=false; break;
					case SDLK_m: SoundEffect->setIsPaused(true);break;	
					case SDLK_ESCAPE: done=1;               break;
				
				};
                break;
            case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_UP:      up_pressed=true;    break;
					case SDLK_DOWN:    down_pressed=true;  break;
					case SDLK_RIGHT:   right_pressed=true; break;
					case SDLK_LEFT:    left_pressed=true;  break;
					case SDLK_w:       up_pressed=true;    break;
					case SDLK_s:       down_pressed=true;  break;
					case SDLK_d:       right_pressed=true; break;
					case SDLK_a:       left_pressed=true;  break;
					case SDLK_u:       u_pressed=true;  break;
					case SDLK_v:       v_pressed=true;  break;
					case SDLK_PLUS:
					case SDLK_KP_PLUS: plus_pressed=true;  break;
					case SDLK_MINUS:
					case SDLK_KP_MINUS:minus_pressed=true; break;
					case SDLK_m: SoundEffect->setIsPaused(false);break;	
					case SDLK_ESCAPE: done=1;              break;
					case SDLK_l: wireframe = ! wireframe; break;
				
				};
                break;
            case SDL_QUIT:
                done = 1;
                break;
            default:
                break;
            }
        }
        if (up_pressed   )rotx+=1.1;
        if (down_pressed )rotx-=1.1;
        if (right_pressed)roty+=1.1;
        if (left_pressed )roty-=1.1;
        if (plus_pressed ){scale*=1.05;if (scale>1.0f)scale=1.0f;}
        if (minus_pressed){scale/=1.05;if (scale<0.01f)scale=0.01f;}
        if (u_pressed){u+=0.05; /*if (u<pi)u=0.0f;*/}
        if (v_pressed){v+=0.05; /*if (v<pi*2.0f)v=0.0f;*/}
        draw ();
    }
	if (SoundEngine) SoundEngine->drop();
    return 0;
}
