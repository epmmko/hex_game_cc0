#include <iostream>

#include <SDL2/SDL2_gfxPrimitives.h> // circleRGBA
#include <SDL2/SDL.h>
#include "fe_input_system.h"
#include "fe_keyboard_cursor.h"
#include "fe_hexboard.h"

void KeyboardCursorFE::Draw(SDL_Renderer* renderer){
  SDL_Color color = {0, 200, 0,255};
  aacircleRGBA(renderer, mCx, mCy, mL/2, color.r, color.g, color.b, color.a);
  aacircleRGBA(renderer, mCx, mCy, mL/2+1, color.r, color.g, color.b, color.a);
  aacircleRGBA(renderer, mCx, mCy, mL/2+2, color.r, color.g, color.b, color.a);
  aacircleRGBA(renderer, mCx, mCy, mL/2+3, color.r, color.g, color.b, color.a);
}

void KeyboardCursorFE::Initialize(HexBoardFE* hex_board){
  mHexRow = 0;
  mHexCol = 0;
  pHexBoard = hex_board;

  mL = pHexBoard->GetEdgeLength();
  mTopPad = pHexBoard->GetTopPad();
  mLeftPad = pHexBoard->GetLeftPad();
  mBoardSize = pHexBoard->GetBoardSize();
}

void KeyboardCursorFE::Update(const InputState& input_state){
  uint8_t screen_row;
  uint8_t screen_col;

  if(input_state.Keyboard.GetButtonState(SDL_SCANCODE_W)
      == ButtonState::RELEASED){
    --mHexRow;
    ++mHexCol;
  }

  if(input_state.Keyboard.GetButtonState(SDL_SCANCODE_S)
      == ButtonState::RELEASED){
    ++mHexRow;
    --mHexCol;
  }

  if(input_state.Keyboard.GetButtonState(SDL_SCANCODE_A)
      == ButtonState::RELEASED){
    --mHexRow;
  }

  if(input_state.Keyboard.GetButtonState(SDL_SCANCODE_D)
      == ButtonState::RELEASED){
    ++mHexCol;
  }

  if(mHexRow < 0) mHexRow = 0;
  if(mHexCol < 0) mHexCol = 0;
  if(mHexRow > mBoardSize -1) mHexRow =mBoardSize - 1;
  if(mHexCol > mBoardSize -1) mHexCol =mBoardSize - 1;

  pHexBoard->GetScreenIndex(mHexRow, mHexCol, screen_row, screen_col);
  pHexBoard->GetCxCy(screen_row, screen_col, mL, mLeftPad, mTopPad, mCx, mCy);
}
