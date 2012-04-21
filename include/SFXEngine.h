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
	
  private:
	irrklang::ISound* _testsound;
	irrklang::ISoundEngine* _soundEngine;
};
#endif
