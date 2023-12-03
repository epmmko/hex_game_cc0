#include <SDL2/SDL.h>
#include <string>
#include "fe_textbox.h"

void TextBoxFE::Initialize(SDL_Renderer *renderer, 
		int x_top_left, int y_top_left,
		int width, int height, std::string text,
		SDL_Color color, EAlign align){
  mXT = x_top_left;
  mYT = y_top_left;
  mW = width;
  mH = height;
  mText = text;
  mColor = color;
  mAlign = align;

  mFontSize = CalculateFontSize(mW, mH, mText);
  Load();

  //create texture
  SDL_Surface *surf = TTF_RenderUTF8_Blended(mFont, mText.c_str(), mColor);
  mTextureW = surf->w;
  mTextureH = surf->h;

  if(surf != nullptr){
    mTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if(mTexture == nullptr){
      SDL_Log("Fail to create texture from surface in TextBoxFE::Initialize:%s",
        SDL_GetError());
    }
  } else {
    SDL_Log("Fail to create font surface of text: %s: %s",
      mText.c_str(), SDL_GetError());
  }
}

int TextBoxFE::CalculateFontSize(int w, int h, std::string text) const{
  //first try = 0.75 * h
  int font_size = static_cast<int>(0.75f * h);
  int ans_w; // result from TTF_SizeText
  int ans_h; // result from TTF_SizeText
  int get_size_status;

  TTF_Font* font = TTF_OpenFont(mFontFileName.c_str(), font_size);
  get_size_status = TTF_SizeText(font, text.c_str(), &ans_w, &ans_h);
  if(get_size_status == -1){
    SDL_Log("Fail to get font size in CalculateFontSize: %s",
			SDL_GetError());
  }
  TTF_CloseFont(font);

  if(ans_w <= w){
    return font_size;
  } else {
    return static_cast<int>(0.75f * ans_h / ans_w * w);
  }
}

void TextBoxFE::Load(){
  mFont = TTF_OpenFont(mFontFileName.c_str(), mFontSize);
  if(mFont == nullptr){
    SDL_Log("Fail to get font size in TextBoxFE::Load(): %s",
			SDL_GetError());
  }
}

void TextBoxFE::Draw(SDL_Renderer *renderer) const{
  SDL_Rect rect;
  switch(mAlign){
    case(EAlign::ELeft):{
      rect.x = mXT;
      rect.y = mYT;
      rect.h = mTextureH;
      rect.w = mTextureW;
      break;
    }
    case(EAlign::ERight):{
      rect.x = mXT + mW - mTextureW;
      rect.y = mYT;
      rect.h = mTextureH;
      rect.w = mTextureW;
      break;
    }
    case(EAlign::ECenter):{
      rect.x = mXT + (mW - mTextureW)/2.0f;
      rect.y = mYT;
      rect.h = mTextureH;
      rect.w = mTextureW;
      break;
    }
  }
  SDL_RenderCopy(renderer, mTexture, NULL, &rect);
}

void TextBoxFE::Unload(){
  if(mFont != nullptr){
    TTF_CloseFont(mFont);
  }
  if(mTexture != nullptr){
    SDL_DestroyTexture(mTexture);
  }
  
}
