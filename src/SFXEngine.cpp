#include <SFXEngine.h>
#include <IrrKlang/irrKlang.h>

#pragma comment(lib, "irrKlang.lib")
SFXEngine::SFXEngine()
{
	_soundEngine = irrklang::createIrrKlangDevice();	
	_testsound = _soundEngine->play2D("Sound_Water0001.wav",true,true,false,irrklang::ESM_AUTO_DETECT, true);
	if(_testsound)_testsound->setIsPaused(true);
}

void SFXEngine::playTestSound(bool play)const
{
	if(_testsound)_testsound->setIsPaused(!play);
}

SFXEngine::~SFXEngine()
{
	if(_testsound) _testsound->drop();
	if (_soundEngine) _soundEngine->drop();
}
