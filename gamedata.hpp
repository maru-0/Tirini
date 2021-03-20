#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down, Up, Shoot, Focus, Restart };
enum class State { Playing};

struct GameData {
  State m_state{State::Playing};
  std::bitset<7> m_input;  // [fire, up, down, left, right]
};

#endif