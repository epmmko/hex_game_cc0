#ifndef FE_MENU_H_
#define FE_MENU_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>
#include <memory>
#include <SDL2/SDL.h>
#include <iostream>

class Game;
class SDL_Renderer;
class InputState;
class FontFE;
class TextBoxFE;
class MenuFE;

enum class EMenuButtonState{
  NONE, FOCUS, RELEASED, CLICKED
};

enum class EPlayMode {E2Player, EAIFirst, EAISecond, ENone};

std::ostream& operator<<(std::ostream& os, EPlayMode val);


class MenuButtonFE{
 public:
  void Initialize(
    SDL_Renderer *renderer,
    SDL_Color bg_color, std::string title,
    int x_top_left, int y_top_left,
    int width, int height,
    std::function<void()> on_click,
    bool is_enable = true);
  void LoadText(SDL_Renderer *renderer);
  void Draw(SDL_Renderer *renderer);
  void ProcessInput(const InputState& input_state);

  void SetFontSize(int font_size){mFontSize = font_size;}
  void SetFontColor(std::string name, SDL_Color color)
    {mFontColorMap[name] = color;}
  void SetToIsNotSelected(){mIsSelected = false;}
  void SetEnableStatus(bool status){mIsEnable = status;}

  void Shutdown();

 private:
  SDL_Color mBGColor;
  MenuFE* pMenu;
  EMenuButtonState mMenuButtonState;
  std::function<void()> mOnClick;
  std::string mTitle;
  std::unordered_map<std::string, std::shared_ptr<FontFE>> pFontMap;

  //Box
  int mXTopLeft;
  int mYTopLeft;
  int mWidth;
  int mHeight;
  int mShadowPad;

  //Text
  int mXT; //x to top left font
  int mYT; //y to top left font
  int mWT; //width of font
  int mHT; //height of font
  int mFontSize;
  std::unordered_map<std::string,SDL_Color> mFontColorMap{
    {"None",SDL_Color {0,0,0,255}},
    {"Focus",SDL_Color {0,0,0,255}},
    {"Pressed",SDL_Color {0,0,0,255}},
    {"Held",SDL_Color {0,0,0,255}},
    {"Released",SDL_Color {0,0,0,255}}
  };
  std::unordered_set<std::string> mMouseStateSet =
    {"None", "Focus", "Pressed", "Held", "Released"};
  
  //Status
  bool mIsSelected = false;
  bool mIsEnable = true;

  //Method
  bool ContainCursor(const InputState& input_state);
  void TextAutoFit(
    int full_text_w,
    int full_text_h,
    int box_w,
    int box_h,
    int& ans_fit_w,
    int& ans_fit_h,
    float fit_ratio = 1.0f) const;
  void GetTextXY(int& ans_x, int& ans_y,
    int text_w, int text_h,
    int box_x, int box_y,
    int box_w, int box_h) const;
};



class MenuFE{
 public:
  void Initialize(Game* game, SDL_Renderer *renderer, uint8_t board_size);
  void Draw(SDL_Renderer *renderer);
  void ProcessInput(const InputState& input_state);

  void Set2Player(){mPlayMode = EPlayMode::E2Player;}
  void SetAIPlayFirst(){mPlayMode = EPlayMode::EAIFirst;}
  void SetAIPlaySecond(){mPlayMode = EPlayMode::EAISecond;}
  void SetStartGame();
  void SetExitGame();

  EPlayMode GetPlayMode() const {return mPlayMode;}
  void Shutdown();
 private:
  Game *pGame;
  SDL_Renderer *pRenderer;
  int mScreenW;
  int mScreenH;
  int mMenuW;
  int mMenuH;
  int mXTopLeft;
  int mYTopLeft;
  int mShadowPad;
  std::unordered_map<std::string, std::shared_ptr<MenuButtonFE>> 
    pButtonMap;  

  std::unordered_map<int,std::string> mLineToButtonTextMap = {
    {2,"2 Players Game"}, {3,"AI = First Player"}, 
    {4,"AI = Second Player"},
    {6,"Start Game"},
    {7,"Exit Game"}
  };

  std::unordered_map<int, std::string> mLineToTextBoxMap = {
    {1,"Hex Game Mode"},
    {5,"Board Size = 7"}
  };

  std::unordered_map<int,std::function<void()>> mLineToCallBack;

  EPlayMode mPlayMode = EPlayMode::E2Player;
  uint8_t mBoardSize = 7;
  bool mExitGame = false;

  void DrawCredits(SDL_Renderer *renderer);
  std::shared_ptr<TextBoxFE> mTextBoxCredits;
};


#endif // FE_MENU_H_
