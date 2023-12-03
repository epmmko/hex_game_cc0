#ifndef BE_HEXBOARD_H_
#define BE_HEXBOARD_H_

#include <unordered_set>
#include <ostream>

class GraphBE;

namespace HexBE{

enum class EPlayerTurn{
  EPlayer1, EPlayer2
};

enum class EWinner{
  EPlayer1Win, EPlayer2Win, ENone
};

EWinner FindWinner(const std::unordered_set<int> &p1_move,
    const std::unordered_set<int> &p2_move, int board_size,
    std::unordered_set<int> &winner_moves);
  // Can be used to find the winner during either normal game play
  // or for the board that is filled by Monte Carlo

EWinner FindWinnerOfFilledBoard(const std::unordered_set<int> &p1_move,int board_size);
  // Can be used to find the winner during either normal game play
  // or for the board that is filled by Monte Carlo

class HexBoardBE{
// HexBoard is used during a normal game play
// AI take HexBoard Object to find the next move (based on the Monte Carlo result)
 public:
  void Initialize(int board_size){mBoardSize = board_size;}
  void AddP1Move(int move);
  void AddP2Move(int move);
  EPlayerTurn GetCurrentPlayerTurn(){return mCurrentPlayerTurn;}
  const std::unordered_set<int>& GetP1Moves(){return mP1Moves;}
  const std::unordered_set<int>& GetP2Moves(){return mP2Moves;}
    // Return the pointer directly, yet mP1Moves & mP2Moves should not
    // be modified externally
  int GetPly(){return mPly;}
  EWinner GetWinner(std::unordered_set<int>& winner_move){
    return FindWinner(mP1Moves, mP2Moves, mBoardSize, winner_move);}
 private:
  int mBoardSize;
  std::unordered_set<int> mP1Moves;
  std::unordered_set<int> mP2Moves;
  int mPly; //starting from 0 to mBoardSize * mBoardSize
    // at 0, no one make move yet
  EPlayerTurn mCurrentPlayerTurn;
};



std::ostream& operator<<(std::ostream& out, EWinner winner);

} // namespace HexBE
#endif // BE_HEXBOARD_H_
