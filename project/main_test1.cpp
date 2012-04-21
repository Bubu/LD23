#include <SDL/SDL.h>
#include <Init.h>
#include <iostream>
#include <Shader.h>
#include <math.h>
#include <TriangleGraph.h>

#include <IrrKlang/irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

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
float scale=0.01;
bool frameLimit=true;
bool wireframe=false;
Uint32 time_;
Uint32 fps=60;

TriangleGraph tg (1);

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
  	//shader_per_pixel.use();
  	/*glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f,  0.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f( 0.0f,  0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f( 0.0f,  1.0f,  0.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f( 0.0f, -1.0f,  0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f( 1.0f,  0.0f,  0.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f,  0.0f,  0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();*/
	
	glBegin(GL_TRIANGLES);
		for(int i=0;i<tg.size();i++)
		{
			glVertex3f(tg[i].a.x, tg[i].a.y, tg[i].a.z);
			glVertex3f(tg[i].b.x, tg[i].b.y, tg[i].b.z);
			glVertex3f(tg[i].c.x, tg[i].c.y, tg[i].c.z);
		}
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0,0,1);

	for(int i=0;i<tg.size();i++)
		{
			glVertex3f(tg[i].centerPoint().x, tg[i].centerPoint().y, tg[i].centerPoint().z);
			glVertex3f(tg[tg[i].n0].centerPoint().x, tg[tg[i].n0].centerPoint().y, tg[tg[i].n0].centerPoint().z);
			glVertex3f(tg[i].centerPoint().x, tg[i].centerPoint().y, tg[i].centerPoint().z);
			glVertex3f(tg[tg[i].n1].centerPoint().x, tg[tg[i].n1].centerPoint().y, tg[tg[i].n1].centerPoint().z);
			glVertex3f(tg[i].centerPoint().x, tg[i].centerPoint().y, tg[i].centerPoint().z);
			glVertex3f(tg[tg[i].n2].centerPoint().x, tg[tg[i].n2].centerPoint().y, tg[tg[i].n2].centerPoint().z);
		}
	glEnd();
	Shader::unuse();
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
        draw ();
    }
	if (SoundEngine) SoundEngine->drop();
    return 0;
}