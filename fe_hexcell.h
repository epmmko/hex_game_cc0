#ifndef FE_HEXCELL_H_
#define FE_HEXCELL_H_

#include <SDL2/SDL.h> //Sint16
#include <cstdint>
#include <vector>
#include "fe_mouse_state.h"
#include "be_game_state.h"

class HexBoardFE;
class InputState;
class KeyboardCursorFE;
enum class EPlayMode;

class HexCellFE{
 public:
  HexCellFE(HexBoardFE* hex_board, int hex_row, int hex_col,
    bool apply_xy_offset = false, int x_offset = 0, int y_offset = 0);
  void Draw(SDL_Renderer* renderer);
  void DrawBlink(SDL_Renderer* renderer, EDrawBlink brightness);
  void UpdateCellState(int x_abs, int y_abs, 
    bool left_is_down, PlayerTurn player_turn,
    const InputState& input_state,
    KeyboardCursorFE* p_keyboard_cursor);

    //Top left is 0,0
  float GetEdgeLength() const {return mL;}

  // Call from parent
  void GetXYOuterArray(std::vector<int> &x_vex_to_be_filled,
    std::vector<int> &y_vex_to_be_filled);
    //input x and y must be size 6
  void MakeAiMove(PlayerTurn player_turn);
  // For shadow drawing
  void SetDefaultOuterColor(SDL_Color color){mDefaultOuterColor = color;}
  void SetDrawOnlyOuterCell(bool val){mDrawOnlyOuterCell = val;}
 private:

  HexBoardFE* pHexBoard;
  int mHexRow; //0 to mBoardSize - 1
  int mHexCol; //0 to mBoardSize - 1
  uint8_t mBoardSize;
  EPlayMode mPlayMode;

  Sint16 mXarr_inner[6]; //store screen location of hex-cell vertices
  Sint16 mYarr_inner[6]; //store screen location of hex-cell vertices
  Sint16 mXarr_outer[6]; //store screen location of hex-cell vertices
  Sint16 mYarr_outer[6]; //store screen location of hex-cell vertices

  float mCx; //center-x on screen coordinate
  float mCy; //center-y on screen coordinate
  float mL; //edge length
  
  FocusState mFocusState = FocusState::NOT_IN_FOCUS;
  ClickState mClickState = ClickState::LEFT_NONE;
  PreviousClickState mPreviousClickState = PreviousClickState::LEFT_UP;
  CellState mCellState = CellState::AVAILABLE;
  CellFocus mCellFocus = CellFocus::NONE;

  CellOwner mCellOwner = CellOwner::NONE;
  CellPreOwner mCellPreOwner = CellPreOwner::NONE;
  //private method
  void DrawFilledHexagon(SDL_Renderer* renderer, Sint16* xarr, Sint16* yarr, SDL_Color color);

  void GetXarrYarr(Sint16 *xarr_out, Sint16 *yarr_out, float L) const;
  bool ContainMouse(int x_abs, int y_abs) const;

  void UpdateCellFocusState(int x_abs, int y_abs, 
    bool left_is_down);
  void UpdateMouseClickState(int x_abs, int y_abs, 
    bool left_is_down);

  // For shadow drawing
  SDL_Color mDefaultOuterColor = {0,0,255,255};
  bool mDrawOnlyOuterCell = false;
  bool mApplyDrawOffSet = false;
  int mCellXOffSet;
  int mCellYOffSet;
};



#endif // FE_HEXCELL_H_
