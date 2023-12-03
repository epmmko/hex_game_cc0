#include <iostream>
#include <functional>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <SDL2/SDL.h>
#include "fe_menu.h"
#include "fe_game.h"
#include "fe_input_system.h"
#include "fe_font.h"
#include "fe_textbox.h"
/*
    Hex Game Mode
2 >> 2 Players Game
3 >> AI = First Player
4 >> AI = Second Player
5 >> Board Size
6 >> Start!
7 >> Exit Game
*/


std::ostream& operator<<(std::ostream& os, EPlayMode val){
  switch(val){
    case EPlayMode::E2Player:
      return os << "E2Player";
      break;
    case EPlayMode::EAIFirst:
      return os << "EAIFirst";
      break;
    case EPlayMode::EAISecond:
      return os << "EAISecond";
      break;
    default:
      return os << "ENone";
  }
}

void MenuFE::SetStartGame(){
  pGame->SetGameScreen(EGameScreen::GAME_PLAY);
  pGame->InitializeByMenuOutput(mBoardSize);

}

void MenuFE::SetExitGame(){
  pGame->ExitGame();
}

void MenuFE::Initialize(Game* game, SDL_Renderer *renderer,
    uint8_t board_size){
  mBoardSize = board_size;
  mLineToCallBack = std::unordered_map<int,std::function<void()>>{
    {2,[this](){Set2Player();
      pButtonMap["AI = First Player"]->SetToIsNotSelected();
      pButtonMap["AI = Second Player"]->SetToIsNotSelected();
      pButtonMap["Start Game"]->SetEnableStatus(true);
    }},
      //[2] is updated again at the end of the initialization
    {3,[this](){SetAIPlayFirst();
      pButtonMap["2 Players Game"]->SetToIsNotSelected();
      pButtonMap["AI = Second Player"]->SetToIsNotSelected();
      pButtonMap["Start Game"]->SetEnableStatus(true);
    }},
      //[3] is updated again at the end of the initialization
    {4,[this](){SetAIPlaySecond();
      pButtonMap["2 Players Game"]->SetToIsNotSelected();
      pButtonMap["AI = First Player"]->SetToIsNotSelected();
      pButtonMap["Start Game"]->SetEnableStatus(true);
    }},
      //[4] is updated again at the end of the initialization
    {6,[this](){SetStartGame();}},
    {7,[this](){SetExitGame();}},
  };

  pGame = game;
  pRenderer = renderer;
  mScreenW = game->GetScreenWidth();
  mScreenH = game->GetScreenHeight();
  mMenuW = 0.4f * mScreenW;
  mMenuH = 0.5f * mScreenH;
  mXTopLeft = 0.5 * mScreenW - 0.5 * mMenuW;
  mYTopLeft = 0.5 * mScreenH - 0.5 * mMenuH;

  int pad_menu = 0.1 * mMenuW;
  
  int x_top_left;
  int y_top_left;
  int width;
  int height;
  
  float total_h_no_pad = mMenuH - 2 * pad_menu;
  float total_space_no_pad = 0.3f * total_h_no_pad;
  float total_text_h_no_pad = 0.7f * total_h_no_pad;

  int space_h = static_cast<int>(total_space_no_pad / 6.0);
  int text_h = static_cast<int>(total_text_h_no_pad / 7.0);  

  for(auto& item: mLineToButtonTextMap){
    int line_number = item.first; //start from 1
    std::string line_text = item.second;
    x_top_left = mXTopLeft + pad_menu;
    y_top_left = (line_number - 1) * (text_h +  space_h) + mYTopLeft + pad_menu;
    width = mMenuW - 2 * pad_menu;
    height = text_h;

    pButtonMap.insert({line_text,
      std::move(
        std::shared_ptr<MenuButtonFE>(
          new MenuButtonFE()
        )
      )
    });

    pButtonMap[line_text]->Initialize(
      pRenderer,
      {220,255,220,255},
      line_text,
      x_top_left,
      y_top_left,
      width,
      height,
      mLineToCallBack[line_number]
    );
  }

  pButtonMap["Start Game"]->SetEnableStatus(false);

  mShadowPad = (mMenuW*0.03f) > (mMenuH*0.03f) ?
    static_cast<int>(mMenuH*0.03f) : static_cast<int>(mMenuW*0.03f);

  mTextBoxCredits = std::move(
    std::shared_ptr<TextBoxFE>(
      new TextBoxFE()
    )
  );
  mTextBoxCredits->Initialize(renderer, mScreenW * 0.7f,
    mScreenH * 0.97f, mScreenW * 0.3f, mScreenH * 0.03f,
    "CC0 No Rights Reserved: By Ekarit Panacharoensawad ",
    {0,0,0,255}, EAlign::ERight);
}

void MenuFE::Draw(SDL_Renderer *renderer){

  SDL_SetRenderDrawColor(renderer, 255, 180, 180, 255);
  SDL_Rect rect = {
    static_cast<int>(mXTopLeft + mShadowPad),
    static_cast<int>(mYTopLeft + mShadowPad), 
    mMenuW,
    mMenuH
  };
  if(SDL_RenderFillRect(renderer, &rect) != 0){
    SDL_Log("Menu Rectangle Error: %s", SDL_GetError());
  }

  SDL_SetRenderDrawColor(renderer, 255, 220, 220, 255);
  rect.x = mXTopLeft;
  rect.y = mYTopLeft;
  rect.w = mMenuW;
  rect.h = mMenuH;
  if(SDL_RenderFillRect(renderer, &rect) != 0){
    SDL_Log("Menu Rectangle Error: %s", SDL_GetError());
  }

  for(auto& ptr: pButtonMap){
    ptr.second->Draw(renderer);
  }
  DrawCredits(renderer);
}

void MenuFE::ProcessInput(const InputState& input_state){
  if(input_state.Keyboard.GetButtonState(SDL_SCANCODE_ESCAPE)
      == ButtonState::RELEASED){
    SetStartGame();
  }

  for(auto& ptr: pButtonMap){
    ptr.second->ProcessInput(input_state);
  }
}

void MenuFE::DrawCredits(SDL_Renderer *renderer){
  mTextBoxCredits->Draw(renderer);
}

void MenuButtonFE::Initialize(SDL_Renderer *renderer,
    SDL_Color color, std::string title,
    int x_top_left, int y_top_left,
    int width, int height,
    std::function<void()> on_click,
    bool is_enable){
  mBGColor = color;
  mTitle = title;
  mXTopLeft = x_top_left;
  mYTopLeft = y_top_left;
  mWidth = width;
  mHeight = height;
  mOnClick = on_click;
  mFontSize = static_cast<int>(mHeight * 0.75f);
  LoadText(renderer);

  mShadowPad = (mWidth*0.01f) > (mHeight*0.01f) ?
    static_cast<int>(mWidth*0.01f) : static_cast<int>(mHeight*0.01f);

  mIsEnable = is_enable;
  
}

void MenuButtonFE::LoadText(SDL_Renderer *renderer){
  for(auto& name: mMouseStateSet){
    pFontMap.insert(std::pair<std::string, std::shared_ptr<FontFE>>
      (name, std::move(std::shared_ptr<FontFE>(new FontFE()))));
    pFontMap[name]->SetOneFontSize(mFontSize);
    pFontMap[name]->Load();
    pFontMap[name]->SetString(mTitle);
    pFontMap[name]->UpdateTexture(renderer, mFontSize,
      mFontColorMap[name]);
  }
  std::cout << "Button Load Text\n";
  std::cout << "Button Load Text\n";
  std::cout << "Button Load Text\n";
  for(auto& name: mMouseStateSet){
    std::cout << name << "H: " << pFontMap[name]->GetSurfH() << '\n';
    std::cout << name << "W: " << pFontMap[name]->GetSurfW() << '\n';
  }
}

bool MenuButtonFE::ContainCursor(const InputState& input_state){
  int x = input_state.Mouse.GetCurrX();
  int y = input_state.Mouse.GetCurrY();
  if((x >= mXTopLeft) && (x <= mXTopLeft + mWidth) &&
     (y >= mYTopLeft) && (y <= mYTopLeft + mHeight)){
    return true;
  }
  return false;
}

void MenuButtonFE::ProcessInput(const InputState& input_state){
  if(!mIsEnable){
    mMenuButtonState = EMenuButtonState::NONE;
    return;
  }

  if(ContainCursor(input_state)){
    switch(input_state.Mouse.GetButtonState(SDL_BUTTON_LEFT)){
      case(ButtonState::NONE):{
        mMenuButtonState = EMenuButtonState::FOCUS;
        break;
      }
      case(ButtonState::PRESSED):{
        mMenuButtonState = EMenuButtonState::CLICKED;
        break;
      }
      case(ButtonState::RELEASED):{
        mMenuButtonState = EMenuButtonState::RELEASED;
        mOnClick();
        if(!mIsSelected){
          mIsSelected = true;
        } else {
          mIsSelected = false;
        }
        break;
      }
      case(ButtonState::HELD):{
        mMenuButtonState = EMenuButtonState::CLICKED;
        break;
      }
    }
  } else {
    mMenuButtonState = EMenuButtonState::NONE;
  }
}

void MenuButtonFE::Draw(SDL_Renderer *renderer){
  //draw shadow first
  if(mIsEnable){
    if(mIsSelected){
      SDL_SetRenderDrawColor(renderer, 20, 20, 255, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 150, 220, 150, 255);
    }
  } else {
    SDL_SetRenderDrawColor(renderer, 112, 112, 112, 255);
  }

  SDL_Rect outer_rect = {
    mXTopLeft + mShadowPad,
    mYTopLeft + mShadowPad,
    mWidth,
    mHeight
  };

  SDL_RenderFillRect(renderer, &outer_rect);

  std::string state_name;
  switch(mMenuButtonState){
    case(EMenuButtonState::NONE):{
      state_name = "None";
      if(mIsEnable){
        SDL_SetRenderDrawColor(renderer, 
          mBGColor.r, mBGColor.g, mBGColor.b, mBGColor.a);
      } else {
        SDL_SetRenderDrawColor(renderer, 
          200, 200, 200, 255);
      }
      break;
    }
    case(EMenuButtonState::FOCUS):{
      state_name = "Focus";
      SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);
      break;
    }
    case(EMenuButtonState::RELEASED):{
      state_name = "Released";
      SDL_SetRenderDrawColor(renderer, 
        mBGColor.r, mBGColor.g, mBGColor.b, mBGColor.a);
      break;
    }
    case(EMenuButtonState::CLICKED):{
      state_name = "Pressed";
      SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
      break;
    }
  }

  SDL_Rect rect;
  rect.x = mXTopLeft;
  rect.y = mYTopLeft;
  rect.w = mWidth;
  rect.h = mHeight;

  SDL_RenderFillRect(renderer, &rect);

  //Render text
  SDL_Color font_color = mFontColorMap[state_name];
  int full_text_w = pFontMap[state_name]->GetSurfW();
  int full_text_h = pFontMap[state_name]->GetSurfH();
  int adjust_text_w = full_text_w;
  int adjust_text_h = full_text_h;
  int adjust_text_x;
  int adjust_text_y;
//  TextAutoFit(full_text_w, full_text_h,
//    mWidth, mHeight, adjust_text_w, adjust_text_h);
  GetTextXY(adjust_text_x, adjust_text_y,
            adjust_text_w, adjust_text_h,
            mXTopLeft, mYTopLeft,
            mWidth, mHeight);
  pFontMap[state_name]->RenderTexture(renderer,
    adjust_text_x, adjust_text_y, adjust_text_w, adjust_text_h);
}

void MenuButtonFE::TextAutoFit(
    int full_text_w,
    int full_text_h,
    int box_w,
    int box_h,
    int& ans_fit_w,
    int& ans_fit_h,
    float fit_ratio) const{

  ans_fit_h = fit_ratio * box_h;
  ans_fit_w = static_cast<float>(full_text_w) / full_text_h * ans_fit_h;
  if(ans_fit_w > box_w * fit_ratio){
    ans_fit_w = box_w * fit_ratio;
    ans_fit_h = static_cast<float>(full_text_h) / full_text_w * ans_fit_w;
  }
}

void MenuButtonFE::GetTextXY(int& ans_x, int& ans_y,
    int text_w, int text_h,
    int box_x, int box_y,
    int box_w, int box_h) const{
  ans_x = box_x + box_w / 2.0f - text_w / 2.0f;
  ans_y = box_y + box_h / 2.0f - text_h / 2.0f;
}

void MenuButtonFE::Shutdown(){
  for(auto& name: mMouseStateSet){
    pFontMap[name]->Unload();
  }
}

void MenuFE::Shutdown(){
  for(auto& item: pButtonMap){
    item.second->Shutdown();
  }
  mTextBoxCredits->Unload();
}
