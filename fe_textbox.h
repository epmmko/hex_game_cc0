#ifndef FE_TEXTBOX_H_
#define FE_TEXTBOX_H_

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_ttf.h>

enum class EAlign{
  ELeft, ECenter, ERight
};

class TextBoxFE{
 public:
  void Initialize(SDL_Renderer *renderer, 
		int x_top_left, int y_top_left,
		int width, int height, std::string text,
		SDL_Color color = {0,0,0,255},
		EAlign align = EAlign::ELeft);
  void Unload();
  void Draw(SDL_Renderer *renderer) const;
 private:
  // Variables set upon initialization
  int mXT; //x to top left font
  int mYT; //y to top left font
  int mW; //width of the entire text
  int mH; //height of the entire text
  std::string mText;
  SDL_Color mColor = SDL_Color {0,0,0,255};
  EAlign mAlign = EAlign::ELeft;

	// Internal result
  int mFontSize;
  int mTextureW;
  int mTextureH;

	// Method
  int CalculateFontSize(int w, int h, std::string text) const;  
  void Load();


  // Internal variable
  std::string mFontFileName = "Assets/DejaVuSans.ttf";
  TTF_Font* mFont = nullptr;
  SDL_Texture* mTexture = nullptr;
  
  
};

#endif // FE_TEXTBOX_H_
