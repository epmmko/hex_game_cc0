#include <iostream>
#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>
#include <memory> //std::unique_ptr
#include <SDL2/SDL_ttf.h>
#include "fe_game.h"
#include "fe_hexboard.h"
#include "fe_hexcell.h"
#include "be_kernel.h"
#include "be_game_state.h"
#include "fe_font.h"
#include "fe_input_system.h"
#include "fe_keyboard_cursor.h"
#include "fe_menu.h"
#include "fe_textbox.h"

bool Game::Initialize(){
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cout << "SDL_Init error\n" << SDL_GetError();
    return false;
  }
  mWindow = SDL_CreateWindow("Hex Game", 
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    0,
    0,
    SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE);
  if(!mWindow){
    std::cout << "error in create window\n" << SDL_GetError();
    return false;
  }
  mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!mRenderer){
    SDL_Log("SDL_CreateRenderer Fails: %s", SDL_GetError());
  }

  SDL_GetRendererOutputSize(mRenderer, &mScreenW, &mScreenH);

  // Initialize text
  if(TTF_Init() != 0){
    SDL_Log("TTF_Init() fail: %s", SDL_GetError());
    return false;
  }

  for(int i = 0; i < 4; ++i){
    mTextInstructionVec.push_back(std::shared_ptr<FontFE>(new FontFE()));
  }

  for(int i = 0; i < 4; ++i){
    mTextInstructionVec[i]->Load();
  }

  std::vector<std::string> instruction_vec = {"Instruction: 2-player mode",
    "Red (mouse: player 1): Connect top-left and bottom-right to win.",
    "Green (keyboard): Connect top-right and bottom-left to win.",
    "    w = up, s = down, a = left, d = right, e = enter"};

  std::vector<int> text_size = {34, 24, 24, 24};
  for(int i = 0; i < mTextInstructionVec.size(); ++i){
    mTextInstructionVec[i]->SetString(instruction_vec[i]);
    mTextInstructionVec[i]->UpdateTexture(mRenderer, text_size[i], {0, 120, 120, 255});
  }

  // Initialize Input System
  mInputSystem = std::shared_ptr<InputSystemFE>(new InputSystemFE());
  
  bool initialize_input_system_success = mInputSystem->Initialize();
  if(!initialize_input_system_success){
    std::cout << "Fail to initialize the input system";
    return false;
  }


    //get pointer out of shared_ptr

  mTextBoxCredits = std::move(
    std::shared_ptr<TextBoxFE>(
      new TextBoxFE()
    )
  );
  mTextBoxCredits->Initialize(mRenderer, mScreenW * 0.7f,
    mScreenH * 0.97f, mScreenW * 0.3f, mScreenH * 0.03f,
    "CC0 No Rights Reserved: By Ekarit Panacharoensawad ",
    {0,0,0,255}, EAlign::ERight);

  mMenu = std::shared_ptr<MenuFE>(new MenuFE());
  mMenu->Initialize(this, mRenderer, mBoardSize);
  return true;
}

void Game::InitializeByMenuOutput(int board_size){
  // Called from MenuFE::SetStartGame()
  // that is about the exit menu
  mBoardSize = board_size;
  mPlayMode = mMenu->GetPlayMode();

  mHexBoard = std::shared_ptr<HexBoardFE>(new HexBoardFE());
  mHexBoard->Initialize(this, mBoardSize);

  mKernelBE = std::shared_ptr<KernelBE>(new KernelBE());
  mKernelBE->Initialize(this, board_size, mPlayMode);

  // Initialize Keyboard Cursor
  mKeyboardCursor = std::shared_ptr<KeyboardCursorFE>(new KeyboardCursorFE());
  mKeyboardCursor->Initialize(mHexBoard.get());
}

void Game::ProcessInput(){

  mInputSystem->PrepareForUpdate();

  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT){
      mIsRunning = false;
    }
  }

  mInputSystem->Update();
  const InputState &state = mInputSystem->GetState();

  switch(mGameScreen){
    case(EGameScreen::GAME_PLAY):{
      //std::cout << "number of player = " << mMenu->GetPlayMode() << '\n';

      mKeyboardCursor->Update(state);
      if(state.Keyboard.GetButtonState(SDL_SCANCODE_ESCAPE)
        == ButtonState::RELEASED){
        mIsRunning = false;
      }
      int mouse_x, mouse_y;
      Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
      bool left_is_down = (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) == 1;
      //get player turn
      PlayerTurn player_turn = mKernelBE->GetPlayerTurn();
      mHexBoard->UpdateAllCellsState(mouse_x, mouse_y,
        left_is_down, player_turn, state, mKeyboardCursor.get());
      break;
    }
    case(EGameScreen::MENU):{
      mMenu->ProcessInput(state);
      break;
    }
  }

}

void Game::RunLoop(){
  while(mIsRunning){
    ProcessInput();
    UpdateNonScreen();
    UpdateScreen();
  }
}

void Game::UpdateScreen(){
  SDL_SetRenderDrawColor(mRenderer, 180, 242, 255, 255);
  SDL_RenderClear(mRenderer);

  switch(mGameScreen){
    case(EGameScreen::GAME_PLAY):{
      mHexBoard->Draw(mRenderer);
      if((mPlayMode == EPlayMode::E2Player) ||
          (mPlayMode == EPlayMode::EAIFirst)){
        mKeyboardCursor->Draw(mRenderer);
      }
      std::vector<int> text_y = {50, 100, 140, 180};
      std::vector<int> text_x = {50, 80, 80, 100};
      for(int i = 0; i < mTextInstructionVec.size(); ++i){
        mTextInstructionVec[i]->RenderTexture(mRenderer, text_x[i],text_y[i],
        mTextInstructionVec[i]->GetSurfW(), mTextInstructionVec[i]->GetSurfH());
      }
      mTextBoxCredits->Draw(mRenderer);
      break;
    }
    case(EGameScreen::MENU):{
      mMenu->Draw(mRenderer);
      break;
    }
    case(EGameScreen::SHOW_WINNER):{
		  int stop_blink_time = mTicksCount + 5000;
		  int temp_blink_time = mTicksCount;
		  int blink_count = 0;

		  for(auto i: mWinnerMoves){
		    std::cout << i << '\n';
		  }

		  while(SDL_GetTicks() < stop_blink_time){
		    while(!SDL_TICKS_PASSED(SDL_GetTicks(), temp_blink_time + 100));
		    temp_blink_time = SDL_GetTicks();

        //regular gameplay draw
				mHexBoard->Draw(mRenderer);
		    if((mPlayMode == EPlayMode::E2Player) ||
		        (mPlayMode == EPlayMode::EAIFirst)){
		      mKeyboardCursor->Draw(mRenderer);
		    }
		    std::vector<int> text_y = {50, 100, 140, 180};
		    std::vector<int> text_x = {50, 80, 80, 100};
		    for(int i = 0; i < mTextInstructionVec.size(); ++i){
		      mTextInstructionVec[i]->RenderTexture(mRenderer, text_x[i],text_y[i],
		      mTextInstructionVec[i]->GetSurfW(), mTextInstructionVec[i]->GetSurfH());
		    }
		    mTextBoxCredits->Draw(mRenderer);

        //draw blinking
		    if(blink_count % 2 == 0){
          mHexBoard->DrawWinner(mRenderer, mWinnerMoves, EDrawBlink::BRIGHT);
		    } else {
          mHexBoard->DrawWinner(mRenderer, mWinnerMoves, EDrawBlink::DARK);
		    }
        SDL_RenderPresent(mRenderer);
		    std::cout << "Player " << mWinner << "win\n";
		    ++blink_count;
		  }
		  mGameScreen = EGameScreen::MENU;
  	}
  }

  SDL_RenderPresent(mRenderer);
} 

void Game::UpdateNonScreen(){
  while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
  mTicksCount = SDL_GetTicks();

  if(mGameScreen == EGameScreen::GAME_PLAY){
    if(mPlayMode != EPlayMode::E2Player){
      int ai_move = -1;
      PlayerTurn player_turn = mKernelBE->GetPlayerTurn();
      if(player_turn == PlayerTurn::RED){
        if(mPlayMode == EPlayMode::EAIFirst){
          ai_move = mKernelBE->GetAiMove();
          mHexBoard->MakeAiMove(ai_move, player_turn);
        }
      } else { // player2 turn (green turn)
        if(mPlayMode == EPlayMode::EAISecond){
          ai_move = mKernelBE->GetAiMove();        
          mHexBoard->MakeAiMove(ai_move, player_turn);
        }
      }
    }
  }

  return;
}

void Game::Shutdown(){
  mMenu->Shutdown();
  for(auto& ptr: mTextInstructionVec){
    ptr->Unload();
    ptr.reset();
  }

  mTextBoxCredits->Unload();

  TTF_Quit();
  mHexBoard.reset();
  mKernelBE.reset();
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::IncrementPly(){
  mKernelBE->IncrementPly();
}

void Game::AddP1Move(int move){
  mKernelBE->AddP1Move(move);
}

void Game::AddP2Move(int move){
  mKernelBE->AddP2Move(move);
}

void Game::UpdateWinner(){
  mWinner = mKernelBE->FindWinner(mWinnerMoves);
  if(mWinner != 0){
    mGameScreen = EGameScreen::SHOW_WINNER;
  }
}
