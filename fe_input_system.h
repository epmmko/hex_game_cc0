#ifndef FE_INPUT_SYSTEM_H_
#define FE_INPUT_SYSTEM_H_

#include <SDL2/SDL.h>

enum class ButtonState{
  NONE, PRESSED, RELEASED, HELD
};

class MouseState{
 public:
  friend class InputSystemFE;
  ButtonState GetButtonState(int button) const;
  float GetCurrX() const {return mCurrX;}
  float GetCurrY() const {return mCurrY;}
  float GetPrevX() const {return mCurrX;}
  float GetPrevY() const {return mCurrY;}
 private:
  float mCurrX;
  float mCurrY;
  float mPrevX;
  float mPrevY;
  Uint32 mCurrButton;
  Uint32 mPrevButton;
};

class KeyboardState{
 public:
  friend class InputSystemFE;
  ButtonState GetButtonState(SDL_Scancode key_code) const;
 private:
  const Uint8* mCurrState;
  Uint8 mPrevState[SDL_NUM_SCANCODES];
  
};

struct InputState{
  KeyboardState Keyboard;
  MouseState Mouse;
};

class InputSystemFE{
 public:
  bool Initialize();
  void PrepareForUpdate();
    // Keep the previous state
  void Update();
  const InputState& GetState() const {return mState;}
 private:
  InputState mState;
};

#endif // FE_INPUT_SYSTEM_H_
