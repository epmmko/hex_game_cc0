#ifndef BE_HEX_AI_H_
#define BE_HEX_AI_H_

#include <random>
#include <unordered_set>
#include <memory>
#include <utility>				// std::pair
namespace HexBE{
class HexBoardBE;

class MonteCarloAI{
 public:
  void Initialize(int board_size, int trial_times = 1000);
  int GetAiMove(const std::unordered_set<int> &p1_moves,
    const std::unordered_set<int> &p2_moves);
  void Fill(
    std::unordered_set<int> &p1_moves_to_be_filled,
    std::unordered_set<int> &p2_moves_to_be_filled,
    const std::unordered_set<int> &available_moves);
 private:
  int mBoardSize;
  int mTrialTimes;
  //Monte Carlo Calculation

  unsigned mSeed;
  std::default_random_engine mRandEngine; //pseudo random number gnerator
};

class MaxComeFirst{
 public:
  bool operator()(const std::pair<int,int>& a, const std::pair<int,int>& b){
    if(a.second > b.second){
      return true;
    } else {
      return false;
    }
  }
};

} // namespace HexBE
#endif // BE_HEX_AI_H_
