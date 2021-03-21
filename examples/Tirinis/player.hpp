#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <vector>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "playerbullet.hpp"

class Player {
  friend PlayerBullet;
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

  void damage();

  std::vector<PlayerBullet> m_bullets{};

  int16_t m_lives{3};

  glm::vec2 m_translation{glm::vec2(0)};
  float m_scale{0.05f};

  bool m_invincibility{false};
  abcg::ElapsedTimer m_invincibilityTimer;
 private:

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{0.46f,0.46f,0.46f,1.0f};
  
  float m_speed{1.5f};
  float m_focus{0.5f};
  
  glm::vec2 spawn{-0.3f, -0.85f};

  abcg::ElapsedTimer m_trailBlinkTimer;
  abcg::ElapsedTimer m_bulletCoolDownTimer;

  
};

#endif