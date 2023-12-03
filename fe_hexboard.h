#ifndef FE_HEXBOARD_H_
#define FE_HEXBOARD_H_

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>
#include "be_game_state.h"
#include <unordered_set>
//#include "fe_hexcell.h" //unique_ptr need access to sizeof
class Game;
class HexCellFE;
class InputState;
class KeyboardCursorFE;
class HexBoardBE;
enum class EPlayMode;


class HexBoardFE{
 public:
  HexBoardFE(){};
  ~HexBoardFE();
//  HexBoardFE(const HexBoardFE&) = delete;
    //cannot have copy because of unique_ptr
//  HexBoardFE& operator=(const HexBoardFE&) = delete;
    //cannot have copy assignment because of unique_ptr
  void Initialize(Game* game, uint8_t board_size);
  //getter & setter
  uint8_t GetBoardSize() const {return mBoardSize;}
  void SetBoardSize(uint8_t b) {mBoardSize = b;}
  float GetEdgeLength () const {return mEdgeLength;}
  float GetLeftPad(){return mLeftPad;}
  float GetTopPad(){return mTopPad;}

  void Draw(SDL_Renderer* renderer);

  void UpdateAllCellsState(int mouse_x, int mouse_y,
    bool left_is_down, PlayerTurn player_turn,
    const InputState& input_state,
    KeyboardCursorFE* p_keyboard_cursor);

  //back called from subclass
  void IncrementPly();

  void GetScreenIndex(uint8_t hex_row, uint8_t hex_col, 
    uint8_t &out_screen_row, uint8_t &out_screen_col) const;
  void GetCxCy(uint8_t screen_row, uint8_t screen_col, float edge_length, 
    float left_pad, float top_pad, float &out_cx, float &out_cy) const;

  void AddP1Move(int move);
  void AddP2Move(int move);
  void UpdateWinner();
  EPlayMode GetPlayMode() const {return mPlayMode;}

  // Called from parent
  void MakeAiMove(int move, PlayerTurn player_turn);
  void DrawWinner(SDL_Renderer* renderer, const std::unordered_set<int> &winner_mst, 
    EDrawBlink brightness);
 private:
  Game* mGame;
  uint8_t mBoardSize;
  float mEdgeLength;
  float mLeftPad; //empty space on the left
  float mTopPad; //empty space on the right
  int mScreenW;
  int mScreenH;
  std::vector<std::shared_ptr<HexCellFE>> mAllHexCells;
    //Use shared pointer to use incomplete definition of HexCellFE
    //Instead of directly include fe_hexcell.h here
    //https://stackoverflow.com/questions/21943569/
    //https://stackoverflow.com/questions/6012157/

  std::vector<std::shared_ptr<HexCellFE>> mTopLeftShadowCells;
  std::vector<std::shared_ptr<HexCellFE>> mBottomRightShadowCells;
  std::vector<std::shared_ptr<HexCellFE>> mTopRightShadowCells;
  std::vector<std::shared_ptr<HexCellFE>> mBottomLeftShadowCells;

  EPlayMode mPlayMode;    
};

#endif // FE_HEXBOARD_H_
