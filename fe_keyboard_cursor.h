#ifndef FE_KEYBOARD_CURSOR_H_
#define FE_KEYBOARD_CURSOR_H_

#include <cstdint>
#include <SDL2/SDL.h>

class InputState;
class HexBoardFE;

class KeyboardCursorFE{
 public:
  void Initialize(HexBoardFE* hex_board);
  void Draw(SDL_Renderer* renderer);
  void Update(const InputState& input_state);
  int8_t GetCursorHexRow(){return mHexRow;}
  int8_t GetCursorHexCol(){return mHexCol;}
 private:
  HexBoardFE* pHexBoard;
  int8_t mHexRow; //0 to mBoardSize - 1
  int8_t mHexCol; //0 to mBoardSize - 1
  float mCx; //center-x on screen coordinate
  float mCy; //center-y on screen coordinate
  float mL; //edge length
  float mRi; //inner radius
  float mTopPad;
  float mLeftPad;
  uint8_t mBoardSize;
};

#endif // FE_KEYBOARD_CURSOR_H_
