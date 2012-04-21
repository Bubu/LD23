//#include <SDL/SDL.h>
#include <Init.h>
#include <iostream>
#include <ProgamManager.h>

int main (int argc, char *argv[])
{
    
    int done=0;
    bool _fullScreenToggled=false;
	std::cout<<"init: "<<init("Test01", 0, 800, 600)<<std::endl;
		
   
	ProgramManager programManager;
	programManager.run();
	
    return 0;
}
