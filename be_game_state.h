#ifndef BE_GAME_STATE_H_
#define BE_GAME_STATE_H_

enum class CellOwner{
  RED, GREEN, NONE
};

enum class CellPreOwner{
  RED, GREEN, NONE
};

enum class CellFocus{
  RED, GREEN, NONE
};

enum class PlayerTurn{
  RED, GREEN
};

enum class EDrawBlink{
  BRIGHT, DARK
};

#endif
