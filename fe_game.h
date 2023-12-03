#ifndef FE_GAME_H_
#define FE_GAME_H_

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_set>

//#include "fe_hexboard.h"
class HexBoardFE;
class KernelBE;
class FontFE;
class InputSystemFE;
class KeyboardCursorFE;
class MenuFE;
class TextBoxFE;

enum class EPlayMode;

enum class EGameScreen{
  MENU, GAME_PLAY, SHOW_WINNER
};

class Game{
 public:
  Game(uint8_t board_size): mBoardSize(board_size){}
  bool Initialize();
  void RunLoop();
  void ProcessInput();
  void UpdateScreen();
  void UpdateNonScreen();
  void Shutdown();
  
  //getter
  int GetScreenWidth() const {return mScreenW;}
  int GetScreenHeight() const {return mScreenH;}
  
  void SetGameScreen(EGameScreen screen)
    {mGameScreen = screen;}
  void ExitGame(){mIsRunning = false;}

  //called from subclass
  void IncrementPly();
  void AddP1Move(int move);
  void AddP2Move(int move);  
  void UpdateWinner();
  EPlayMode GetPlayMode() const {return mPlayMode;}
  void InitializeByMenuOutput(int board_size);
    //called from fe_menu.cpp

 private:
  SDL_Event event;
  SDL_Window *mWindow = nullptr;
  SDL_Renderer* mRenderer = nullptr;
  uint32_t mTicksCount = 0;
  bool mIsRunning = true;
  EGameScreen mGameScreen = EGameScreen::MENU;
  int mWinner = 0;
    //0 = none
    //1 = p1 win
    //2 = p2 win
    // updated by mKernelBE
    //   everytime that AddP1Move or AddP2Move is called
  std::unordered_set<int> mWinnerMoves;

  int mScreenW;
  int mScreenH;
  uint8_t mBoardSize;
  //Front End component
  std::shared_ptr<HexBoardFE> mHexBoard;
  std::vector<std::shared_ptr<FontFE>> mTextInstructionVec;
  std::shared_ptr<InputSystemFE> mInputSystem;
  std::shared_ptr<KeyboardCursorFE> mKeyboardCursor;
  std::shared_ptr<MenuFE> mMenu;
  std::shared_ptr<TextBoxFE> mTextBoxCredits;

  //Back End component
  std::shared_ptr<KernelBE> mKernelBE;

  //Result after calling menu screen
  EPlayMode mPlayMode;

}; 

#endif // FE_GAME
// comment 2
