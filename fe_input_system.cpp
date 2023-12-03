#include <SDL2/SDL.h>
#include <cstring> //memset
#include "fe_input_system.h"


ButtonState MouseState::GetButtonState(int button) const{
  int mask = SDL_BUTTON(button);
  if((mask & mCurrButton) == 1){
    if((mask & mPrevButton) == 1){
      return ButtonState::HELD;
    } else {
      return ButtonState::PRESSED;
    }
  } else {
    if((mask & mPrevButton) == 1){
      return ButtonState::RELEASED;
    } else {
      return ButtonState::NONE;
    }
  }
}

ButtonState KeyboardState::GetButtonState(SDL_Scancode key_code) const{
  if(mCurrState[key_code] == 1){
    if(mPrevState[key_code] == 1){
      return ButtonState::HELD;
    } else {
      return ButtonState::PRESSED;
    }
  } else {
    if(mPrevState[key_code] == 1){
      return ButtonState::RELEASED;
    } else {
      return ButtonState::NONE;
    }    
  }
}

bool InputSystemFE::Initialize(){
  // Initialize keyboard
  mState.Keyboard.mCurrState = SDL_GetKeyboardState(nullptr);
  memset(mState.Keyboard.mPrevState,0,SDL_NUM_SCANCODES);

  // Initialize mouse
  mState.Mouse.mCurrButton = 0;
  mState.Mouse.mPrevButton = 0;

  mState.Mouse.mPrevX = 0;
  mState.Mouse.mPrevY = 0;
  mState.Mouse.mCurrX = 0;
  mState.Mouse.mCurrY = 0;
  return true;
}

void InputSystemFE::PrepareForUpdate(){
  // Copy current state to previous state
  // Keybard
  memcpy(mState.Keyboard.mPrevState,
         mState.Keyboard.mCurrState,
         SDL_NUM_SCANCODES);
  // Mouse
  mState.Mouse.mPrevButton = mState.Mouse.mCurrButton;
  mState.Mouse.mPrevX = mState.Mouse.mCurrX;
  mState.Mouse.mPrevY = mState.Mouse.mCurrY;
}

void InputSystemFE::Update(){
  // Mouse
  int x = 0, y = 0;
  mState.Mouse.mCurrButton = SDL_GetMouseState(&x, &y);
  mState.Mouse.mCurrX = x;
  mState.Mouse.mCurrY = y;
}
