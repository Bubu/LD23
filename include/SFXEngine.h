#ifndef __SFXEngine_H_INCLUDED__
#define __SFXEngine_H_INCLUDED__
#include <string>

namespace irrklang
{
	class ISound;
	class ISoundEngine;
}
class SFXEngine
{
  public:
	SFXEngine();
	void playTestSound(bool play)const;
	~SFXEngine();
	void playJumpSound(bool play)const;
	void playKeySound()const;
	void playAttackSound()const;
	void playDeathSound()const;
	void playLevelSound()const;
  private:
	irrklang::ISound* _key;
	irrklang::ISound* _attack;
	irrklang::ISound* _death;
	irrklang::ISound* _level;
	irrklang::ISound* _jump;	
	irrklang::ISound* _musik;
	irrklang::ISound* _testsound;
	irrklang::ISoundEngine* _soundEngine;
};
#endif
