#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <string>
#include "fe_font.h"

bool FontFE::Load(){

  for(auto& size: mFontSizes){
    TTF_Font* font = TTF_OpenFont(font_file_name.c_str(), size);
    //std::cout << "font size: " << size << " font: " << font << std::endl;
    if(font == nullptr){
      SDL_Log("Fail to load font: %s of size = %d", font_file_name.c_str(), size);
      return false;
    }
    mFontData.emplace(size, font);
  }

  //std::cout << "in FontFE::Load\n";
//  for(auto &font_data: mFontData){
//    std::cout<<font_data.first << ":" << font_data.second << '\n';
//  }

  return true; 
}

void FontFE::Unload(){
  for(auto& font_i: mFontData){
    TTF_CloseFont(font_i.second);
  }
  if(mTexture != nullptr){
    SDL_DestroyTexture(mTexture);
  }
}

void FontFE::UpdateTexture(SDL_Renderer* renderer, int font_size, SDL_Color color){
  //https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
  //https://github.com/gameprogcpp/code/blob/master/Chapter11/Font.cpp
  auto iter = mFontData.find(font_size);
  if(iter != mFontData.end()){
    TTF_Font* font = iter->second;
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, mString.c_str(), color);
    //SDL_Surface *surf = TTF_RenderText_Solid(font, mString.c_str(), color);
    //SDL_Surface *surf = TTF_RenderUTF8_Shaded(font, mString.c_str(), color, {180,242,255,255});
    mSurfW = surf->w;
    mSurfH = surf->h;
    if(surf != nullptr){
      mTexture = SDL_CreateTextureFromSurface(renderer, surf);
      SDL_FreeSurface(surf);
      if(mTexture == nullptr){
        SDL_Log("Fail to create font texture from surface: %s", SDL_GetError());
      }
    } else {
      SDL_Log("Fail to create font surface of text: %s: %s", 
        mString.c_str(), SDL_GetError());
    }
  } else {
    SDL_Log("Font size %d is not supported", font_size);
  }
  //std::cout << "mTexture:" << mTexture << '\n';
}

void FontFE::RenderTexture(SDL_Renderer* renderer, int x, int y, int w, int h){
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.h = h;
  rect.w = w;
  SDL_RenderCopy(renderer, mTexture, NULL, &rect);
}

