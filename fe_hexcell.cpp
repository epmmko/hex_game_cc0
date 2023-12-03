#include <iostream>
#include <cstdint>
#include <cmath> //sqrtf
#include <SDL2/SDL.h>
#include <vector>
#include <SDL2/SDL2_gfxPrimitives.h> //filledPolygonRGBA
#include "fe_hexcell.h"
#include "fe_hexboard.h"
#include "fe_mouse_state.h" //MouseState
#include "be_game_state.h" //CellOwner
#include "fe_input_system.h"
#include "fe_keyboard_cursor.h"
/*
Visual representation (cells)


   0   1   2   3   4   5   6   7   8   9   10  11  12  << orthogonal screen index
                                                                V
                         6 ___ 0                                V
                     5 ___/0,6\___ 1                            0
                 4 ___/0,5\___/1,6\___ 2                        1
             3 ___/0,4\___/1,5\___/2,6\___ 3                    2
         2 ___/0,3\___/1,4\___/2,5\___/3,6\___ 4                3
     1 ___/0,2\___/1,3\___/2,4\___/3,5\___/4,6\___ 5            4
 0 ___/0,1\___/1,2\___/2,3\___/3,4\___/4,5\___/5,6\___ 6        5
  /0,0\___/1,1\___/2,2\___/3,3\___/4,4\___/5,5\___/6,6\         6
  \___/1,0\___/2,1\___/3,2\___/4,3\___/5,4\___/6,5\___/  Line 8 7
      \___/2,0\___/3,1\___/4,2\___/5,3\___/6,4\___/  Line 9     8
          \___/3,0\___/4,1\___/5,2\___/6,3\___/  Line 10        9
              \___/4,0\___/5,1\___/6,2\___/  Line 11            10
                  \___/5,0\___/6,1\___/  Line 12                11
                      \___/6,0\___/  Line 13                    12
                          \___/  Line 14                        

*/
void HexCellFE::UpdateCellFocusState(int x_abs, int y_abs,
    bool left_is_down){
  if(ContainMouse(x_abs, y_abs)){
    mFocusState = FocusState::IN_FOCUS;
  } else {
    mFocusState = FocusState::NOT_IN_FOCUS;
  }
}

void HexCellFE::UpdateMouseClickState(int x_abs, int y_abs,
    bool left_is_down){
  if(!ContainMouse(x_abs, y_abs)){
    mPreviousClickState = PreviousClickState::LEFT_UP;
    mClickState = ClickState::LEFT_NONE;
  } else { // has mouse
    if(mPreviousClickState == PreviousClickState::LEFT_UP){
      if(left_is_down){
        mPreviousClickState = PreviousClickState::LEFT_DOWN;
        mClickState = ClickState::LEFT_PRESSED;
      } else { //left is up
        mPreviousClickState = PreviousClickState::LEFT_UP;        
        mClickState = ClickState::LEFT_NONE;
      }
    } else { // left was down previously
      if(left_is_down){
        mPreviousClickState = PreviousClickState::LEFT_DOWN;
        mClickState = ClickState::LEFT_HELD;
      } else {
        mPreviousClickState = PreviousClickState::LEFT_UP;        
        mClickState = ClickState::LEFT_RELEASED;
      }
    }
  }
}

void HexCellFE::UpdateCellState(int x_abs, int y_abs, 
    bool left_is_down, PlayerTurn player_turn,
    const InputState& input_state,
    KeyboardCursorFE* p_keyboard_cursor){

  if(player_turn == PlayerTurn::GREEN){
    if((p_keyboard_cursor->GetCursorHexRow() == mHexRow) &&
       (p_keyboard_cursor->GetCursorHexCol() == mHexCol)){
      // HexRow & HexCol are matched to the keyboard cursor location
      if(mCellState != CellState::OWNED){
        mCellState = CellState::FOCUS;
        mCellFocus = CellFocus::GREEN;
      }

      switch(input_state.Keyboard.GetButtonState(SDL_SCANCODE_E)){
        case(ButtonState::RELEASED):{
          if(mCellState != CellState::OWNED){
            mCellState = CellState::OWNED;
            mCellOwner = CellOwner::GREEN;
            pHexBoard->AddP2Move(mHexRow * mBoardSize + mHexCol);
            pHexBoard->IncrementPly();
            pHexBoard->UpdateWinner();
          }
          break;
        }
        case(ButtonState::NONE):{
          if(mCellState == CellState::PRE_OWNED){
            mCellState = CellState::AVAILABLE;
          }
          break;
        }
        case(ButtonState::PRESSED):{
          if(mCellState != CellState::OWNED){
            mCellState = CellState::PRE_OWNED;
            mCellPreOwner = CellPreOwner::GREEN;
          }
          break;
        }
        case(ButtonState::HELD):{
          if(mCellState != CellState::OWNED){
            mCellState = CellState::PRE_OWNED;
            mCellPreOwner = CellPreOwner::GREEN;
          }
          break;
        }
      }
    } else { // not in focus
      if(mCellState != CellState::OWNED){
        mCellState = CellState::AVAILABLE;
        mCellFocus = CellFocus::NONE;
      }
    }
    return;
  }

  //do this update if it is RED (mouse) turn
  UpdateCellFocusState(x_abs, y_abs, left_is_down);
  UpdateMouseClickState(x_abs, y_abs, left_is_down);

  //update if focus
  if(mCellState != CellState::OWNED){
    if(mFocusState == FocusState::IN_FOCUS){
      mCellState = CellState::FOCUS;
      mCellFocus = CellFocus::RED;
    } else {
      mCellState = CellState::AVAILABLE;
      mCellFocus = CellFocus::NONE;
    }
  }

  //if focus, more updates may need
  switch(mClickState){
    case(ClickState::LEFT_NONE):{
      if(mCellState == CellState::PRE_OWNED){
        mCellState = CellState::AVAILABLE;
      }
      break;
    }
    case(ClickState::LEFT_PRESSED):{
      if(mCellState != CellState::OWNED){
        mCellState = CellState::PRE_OWNED;
        mCellPreOwner = CellPreOwner::RED;
      }
      break;
    }
    case(ClickState::LEFT_HELD):{
      if(mCellState != CellState::OWNED){
        mCellState = CellState::PRE_OWNED;
        mCellPreOwner = CellPreOwner::RED;
      }
      break;
    }
    case(ClickState::LEFT_RELEASED):{
      if(mCellState != CellState::OWNED){
        mCellState = CellState::OWNED;
        mCellOwner = CellOwner::RED;
        pHexBoard->AddP1Move(mHexRow * mBoardSize + mHexCol);
        pHexBoard->IncrementPly();
        pHexBoard->UpdateWinner();
      }
      break;
    }
  }
}

void HexCellFE::MakeAiMove(PlayerTurn player_turn){
  if(player_turn == PlayerTurn::RED){
    mCellState = CellState::OWNED;
    mCellOwner = CellOwner::RED;
    pHexBoard->AddP1Move(mHexRow * mBoardSize + mHexCol);
    pHexBoard->IncrementPly();
    pHexBoard->UpdateWinner();
  } else {
    mCellState = CellState::OWNED;
    mCellOwner = CellOwner::GREEN;
    pHexBoard->AddP2Move(mHexRow * mBoardSize + mHexCol);
    pHexBoard->IncrementPly();
    pHexBoard->UpdateWinner();
  }
}

void HexCellFE::GetXYOuterArray(std::vector<int> &x_vex_to_be_filled,
    std::vector<int> &y_vex_to_be_filled){
  for(int i = 0; i < 6; ++i){
    x_vex_to_be_filled[i] = static_cast<int>(mXarr_outer[i]);
    y_vex_to_be_filled[i] = static_cast<int>(mYarr_outer[i]);
  }
}

bool HexCellFE::ContainMouse(int x_abs, int y_abs) const{
  float x = x_abs - mCx;
  float y = y_abs - mCy;
  if( (y > -mL * sqrtf(3) * 0.5) && (y < mL * sqrtf(3) * 0.5) ){
    if(y >=0){
      if((x >        (y - mL * sqrtf(3))/sqrtf(3)) && 
         (x < -1.0 * (y - mL * sqrtf(3))/sqrtf(3))){
        return true;
      }
    } else {
      if((x > -1.0 * (y + mL * sqrtf(3))/sqrtf(3)) && 
         (x <        (y + mL * sqrtf(3))/sqrtf(3))){
        return true;
      }
    }
  }
  return false;
}



HexCellFE::HexCellFE(HexBoardFE* hex_board, int hex_row, int hex_col,
    bool apply_xy_offset, int x_offset, int y_offset):
    pHexBoard(hex_board), mHexRow(hex_row), mHexCol(hex_col){
  uint8_t screen_row;
  uint8_t screen_col;
  float top_pad;
  float left_pad;

  mBoardSize = pHexBoard->GetBoardSize();
  mPlayMode = pHexBoard->GetPlayMode();
 
  mL = pHexBoard->GetEdgeLength();
  top_pad = pHexBoard->GetTopPad();
  left_pad = pHexBoard->GetLeftPad();
  pHexBoard->GetScreenIndex(mHexRow, mHexCol, screen_row, screen_col);
  pHexBoard->GetCxCy(screen_row, screen_col, mL, left_pad, top_pad, mCx, mCy);

  if(apply_xy_offset){
    mCx = mCx + x_offset;
    mCy = mCy + y_offset;
  }

  GetXarrYarr(mXarr_outer, mYarr_outer, mL*1.05);
  GetXarrYarr(mXarr_inner, mYarr_inner, mL*0.95);
}

void HexCellFE::GetXarrYarr(Sint16 *xarr_out, Sint16 *yarr_out, float L) const{
  xarr_out[0] = static_cast<Sint16>(mCx - L);
  yarr_out[0] = static_cast<Sint16>(mCy);
  xarr_out[1] = static_cast<Sint16>(mCx-L/2.0f);
  yarr_out[1] = static_cast<Sint16>(mCy-L*0.5f*sqrtf(3));
  xarr_out[2] = static_cast<Sint16>(mCx+L/2.0f);
  yarr_out[2] = static_cast<Sint16>(mCy-L*0.5f*sqrtf(3));
  xarr_out[3] = static_cast<Sint16>(mCx+L);
  yarr_out[3] = static_cast<Sint16>(mCy);
  xarr_out[4] = static_cast<Sint16>(mCx+L/2.0f);
  yarr_out[4] = static_cast<Sint16>(mCy+L*0.5f*sqrtf(3));
  xarr_out[5] = static_cast<Sint16>(mCx-L/2.0f);
  yarr_out[5] = static_cast<Sint16>(mCy+L*0.5f*sqrtf(3));
}

void HexCellFE::Draw(SDL_Renderer* renderer){
  SDL_Color inner_color {180, 242, 255,255};
  SDL_Color outer_color = mDefaultOuterColor;
  switch(mCellState){
    case(CellState::AVAILABLE):{
      break;
    }
    case(CellState::FOCUS):{
      if(mCellFocus == CellFocus::RED){
        inner_color = {255, 220, 220,255};
      } else {
        inner_color = {220, 255, 220,255};
      }
      break;
    }
    case(CellState::OWNED):{
      if(mCellOwner == CellOwner::RED){
        inner_color = {255, 180, 180,255};
      } else {
        inner_color = {180, 255, 180,255};
      }
      break;
    }
    case(CellState::PRE_OWNED):{
      if(mCellPreOwner == CellPreOwner::RED){
        inner_color = {200, 100, 100,255};
      } else {
        inner_color = {100, 200, 100,255};
      }
    }
  }

  DrawFilledHexagon(renderer,
    mXarr_outer, mYarr_outer, outer_color);
  if(!mDrawOnlyOuterCell){
		DrawFilledHexagon(renderer,
		  mXarr_inner, mYarr_inner, inner_color);  
  }
}

void HexCellFE::DrawBlink(SDL_Renderer* renderer, EDrawBlink brightness){
  SDL_Color inner_color {180, 242, 255,255};
  SDL_Color outer_color = mDefaultOuterColor;

  switch(brightness){
    case(EDrawBlink::BRIGHT):{
			switch(mCellState){
				case(CellState::OWNED):{
				  if(mCellOwner == CellOwner::RED){
				    inner_color = {255, 220, 220,255};
						outer_color = {180, 100, 100,255};
				  } else {
				    inner_color = {220, 255, 220,255};
						outer_color = {100, 180, 100,255};
				  }
				  break;
				}
			}
      break;
    }
    case(EDrawBlink::DARK):{
			switch(mCellState){
				case(CellState::OWNED):{
				  if(mCellOwner == CellOwner::RED){
				    inner_color = {200, 100, 100,255};
				  } else {
				    inner_color = {100, 200, 100,255};
				  }
				  break;
				}
			}

      break;
    }
  }


  DrawFilledHexagon(renderer,
    mXarr_outer, mYarr_outer, outer_color);
  if(!mDrawOnlyOuterCell){
		DrawFilledHexagon(renderer,
		  mXarr_inner, mYarr_inner, inner_color);  
  }
}

void HexCellFE::DrawFilledHexagon(SDL_Renderer* renderer, Sint16* xarr, Sint16* yarr, SDL_Color color){
  filledPolygonRGBA(renderer, xarr, yarr, 6, color.r, color.g, color.b, color.a);
}

