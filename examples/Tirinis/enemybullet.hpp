#ifndef ENEMYBULLET_HPP_
#define ENEMYBULLET_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

#include <vector>


class EnemyBullet{
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

  void setParams( GLuint program,glm::vec2 translation, glm::vec2 direction, float scale, float speed, glm::vec4 color);


  bool m_hit{false};
  float m_speed{0.9f};//5.0f
  float m_scale{0.025f};
  glm::vec4 m_color{1.0f,0.0f,0.0f,1.0f};

  glm::vec2 m_translation{-0.3f,0.0f};
  glm::vec2 m_direction{0.0f,-1.0f};

  bool m_aimed{true};

 private:
  GLuint m_vbo{};
  GLuint m_vao{};
  GLuint m_ebo{};

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

};





#endif