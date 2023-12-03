#include <cstdint>
#include <cmath>
#include <SDL2/SDL.h>
#include <memory>
#include "fe_hexboard.h"
#include "fe_game.h"
#include "fe_hexcell.h"
#include "fe_input_system.h"
#include "fe_keyboard_cursor.h"

#include <iostream>

HexBoardFE::~HexBoardFE(){
  for(auto& ptr: mAllHexCells){
    ptr.reset();
  }
}

void HexBoardFE::MakeAiMove(int move, PlayerTurn player_turn){
  mAllHexCells[move]->MakeAiMove(player_turn);
}

void HexBoardFE::Initialize(Game* game, uint8_t board_size){
  mBoardSize = board_size;
  mGame = game;
  mPlayMode = mGame->GetPlayMode();

  mScreenW = game->GetScreenWidth();
  mScreenH = game->GetScreenHeight();
  float cell_height = mScreenH / (board_size + 4.0f);
  mEdgeLength = cell_height / sqrtf(3);

  float board_width_on_screen = mEdgeLength * (3 * board_size -1 );
  mLeftPad = (mScreenW - board_width_on_screen) * 0.5f;
  mTopPad = cell_height * 2.0;

  for(uint8_t i = 0; i < board_size; ++i){
    for(uint8_t j = 0; j < board_size; ++j){
      mAllHexCells.push_back(std::move(std::shared_ptr<HexCellFE>(new HexCellFE(this,i,j))));
    }
  }

  for(int i = 0; i < board_size; ++i){
    mTopLeftShadowCells.push_back(std::move(std::shared_ptr<HexCellFE>(
      new HexCellFE(this,0,i,true, -cell_height*0.1, -cell_height*0.1)
    )));
    mTopLeftShadowCells[i]->SetDrawOnlyOuterCell(true);
    mTopLeftShadowCells[i]->SetDefaultOuterColor({255, 220, 220,255});

    mBottomRightShadowCells.push_back(std::move(std::shared_ptr<HexCellFE>(
      new HexCellFE(this,mBoardSize - 1,i,true, cell_height*0.1, cell_height*0.1)
    )));
    mBottomRightShadowCells[i]->SetDrawOnlyOuterCell(true);
    mBottomRightShadowCells[i]->SetDefaultOuterColor({255, 220, 220,255});

    mTopRightShadowCells.push_back(std::move(std::shared_ptr<HexCellFE>(
      new HexCellFE(this,i,board_size-1,true, cell_height*0.1, -cell_height*0.1)
    )));
    mTopRightShadowCells[i]->SetDrawOnlyOuterCell(true);
    mTopRightShadowCells[i]->SetDefaultOuterColor({220, 255, 220,255});

    mBottomLeftShadowCells.push_back(std::move(std::shared_ptr<HexCellFE>(
      new HexCellFE(this,i,0,true, -cell_height*0.1, cell_height*0.1)
    )));
    mBottomLeftShadowCells[i]->SetDrawOnlyOuterCell(true);
    mBottomLeftShadowCells[i]->SetDefaultOuterColor({220, 255, 220,255});

  }
}

void HexBoardFE::Draw(SDL_Renderer* renderer){
  for(auto& hex_cell: mTopLeftShadowCells){
    hex_cell->Draw(renderer);
  }
  for(auto& hex_cell: mTopRightShadowCells){
    hex_cell->Draw(renderer);
  }

  for(auto& hex_cell: mBottomRightShadowCells){
    hex_cell->Draw(renderer);
  }
  for(auto& hex_cell: mBottomLeftShadowCells){
    hex_cell->Draw(renderer);
  }
  for(auto& hex_cell: mAllHexCells){
    hex_cell->Draw(renderer);
  }
}

void HexBoardFE::DrawWinner(SDL_Renderer* renderer, const std::unordered_set<int> &winner_mst, 
    EDrawBlink brightness){
  for(auto i: winner_mst){
    mAllHexCells[i]->DrawBlink(renderer, brightness);
  }
}

void HexBoardFE::UpdateAllCellsState(
    int mouse_x, int mouse_y,
    bool left_is_down, PlayerTurn player_turn,
    const InputState& input_state,
    KeyboardCursorFE *p_keyboard_cursor){
  for(auto& hex_cell: mAllHexCells){
    hex_cell->UpdateCellState(mouse_x, mouse_y,
      left_is_down, player_turn, input_state, p_keyboard_cursor);
  }
}

void HexBoardFE::IncrementPly(){
  mGame->IncrementPly();
}

void HexBoardFE::AddP1Move(int move){
  mGame->AddP1Move(move);
}
void HexBoardFE::AddP2Move(int move){
  mGame->AddP2Move(move);
}

void HexBoardFE::GetScreenIndex(uint8_t hex_row, uint8_t hex_col, 
    uint8_t &out_screen_row, uint8_t &out_screen_col) const{
  out_screen_row = mBoardSize + hex_row - hex_col - 1;
  out_screen_col = hex_row + hex_col;
}

void HexBoardFE::GetCxCy(uint8_t screen_row, uint8_t screen_col, float L, 
    float left_pad, float top_pad, float &out_cx, float &out_cy) const {
  out_cx = left_pad + L + screen_col * 1.5f * L;
  out_cy = top_pad + 0.5f * L * sqrtf(3) * (1 + screen_row);
}

void HexBoardFE::UpdateWinner(){
  mGame->UpdateWinner();
}
