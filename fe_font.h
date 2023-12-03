#ifndef FE_FONT_H_
#define FE_FONT_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <string>
#include <vector>

class FontFE{
 public:
  bool Load();
  void Unload();
  void UpdateTexture(SDL_Renderer* renderer, int font_size, SDL_Color color);
  void RenderTexture(SDL_Renderer* renderer, int x, int y, int w, int h);
  
  //setter
  void SetString(std::string in_str){mString = in_str;}
  void SetOneFontSize(int font_size){mFontSizes = std::vector<int> {font_size};}
  //getter
  int GetSurfH(){return mSurfH;}
  int GetSurfW(){return mSurfW;}
 private:
  std::string mString;
  std::string font_file_name = "Assets/DejaVuSans.ttf";
  std::vector<int> mFontSizes = {12, 16, 20, 24, 26, 28, 30, 34, 36, 40, 60, 80};
  std::unordered_map<int, TTF_Font*> mFontData;
  SDL_Color mFontColor {0,0,0,255};
  SDL_Texture* mTexture = nullptr;
  int mSurfW;
  int mSurfH;

};

#endif //FE_FONT_H_
