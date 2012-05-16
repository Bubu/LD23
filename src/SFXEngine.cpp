#include <SFXEngine.h>
#include <IrrKlang/irrKlang.h>

#pragma comment(lib, "irrKlang.lib")
SFXEngine::SFXEngine()
{
	_soundEngine = irrklang::createIrrKlangDevice();	
	_testsound = _soundEngine->play2D("Sound_Water0001.wav",true,true,false,irrklang::ESM_AUTO_DETECT, true);
	_key = _soundEngine->play2D("item.wav",false,true,false,irrklang::ESM_AUTO_DETECT, true);
	_attack = _soundEngine->play2D("attack.wav",false,true,false,irrklang::ESM_AUTO_DETECT, true);
	_death = _soundEngine->play2D("death.wav",false,true,false,irrklang::ESM_AUTO_DETECT, true);
	_level = _soundEngine->play2D("level.wav",false,true,false,irrklang::ESM_AUTO_DETECT, true);
	_jump = _soundEngine->play2D("jump.wav",true,true,false,irrklang::ESM_AUTO_DETECT, true);
	_musik = _soundEngine->play2D("music2.mp3",true,true,false,irrklang::ESM_AUTO_DETECT, true);
	if(_testsound)_testsound->setIsPaused(true);
}

void SFXEngine::playTestSound(bool play)const
{
	if(_testsound)_testsound->setIsPaused(!play);
}

void SFXEngine::playJumpSound(bool play)const
{
	if(_jump)_jump->setIsPaused(!play);
}
void SFXEngine::playKeySound()const
{
	if(_key)_key->setIsPaused(false);
}
void SFXEngine::playAttackSound()const
{
	if(_attack)_attack->setIsPaused(false);
}
void SFXEngine::playDeathSound()const
{
	if(_death)_death->setIsPaused(false);
}
void SFXEngine::playLevelSound()const
{
	if(_level)_level->setIsPaused(false);
}

SFXEngine::~SFXEngine()
{
	if(_testsound) _testsound->drop();
	if (_soundEngine) _soundEngine->drop();
}
