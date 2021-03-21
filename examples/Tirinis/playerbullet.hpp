#ifndef PLAYERBULLET_HPP_
#define PLAYERBULLET_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cppitertools/itertools.hpp>

class PlayerBullet{
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

  glm::vec2 m_translation{0};
  glm::vec2 m_direction{0};

  bool m_hit{false};
  float m_speed{5.0f};//5.0f
  float m_scale{0.04f};
  glm::vec4 m_color{1};
 private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  
  
  // glm::vec2 m_translation{0};
};

#endif