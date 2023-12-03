#include <iostream>
#include <cstdint>
#include "fe_game.h"
#include <string> //std::stoi

int main(int argc, char* argv[]){
  
  uint8_t board_size;
  if(argc == 1){
    board_size = 7;
  } else {
    board_size = std::stoi(argv[1]);
  }
 
  std::cout << "sizeof(int) = " << sizeof(int) << '\n';
  std::cout << "sizeof(Game) = " << sizeof(Game) << '\n';
  Game game(board_size);
  
  bool success = game.Initialize();
  if(success){
    game.RunLoop();
  }
  game.Shutdown();
  return 0;
} 
