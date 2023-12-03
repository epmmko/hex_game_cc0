#ifndef FE_MOUSE_STATE_H_
#define FE_MOUSE_STATE_H_

enum class FocusState{
  NOT_IN_FOCUS, IN_FOCUS
};

enum class ClickState{
  LEFT_NONE, LEFT_PRESSED, LEFT_RELEASED, LEFT_HELD
//0, 0     , 0, 1      , 1, 0        , 1,1
};

enum class PreviousClickState{
  LEFT_DOWN, LEFT_UP
};

enum class CellState{
  PRE_OWNED, OWNED, AVAILABLE, FOCUS
};

#endif //FE_MOUSE_STATE_H_
