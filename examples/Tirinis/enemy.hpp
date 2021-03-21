#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"
#include "enemybullet.hpp"

#include <vector>

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

enum behaviorState {Waiting, Moving};

struct shootPattern{
  EnemyBullet bullet{};

  float angle = M_PI;
  uint16_t nSpread = 9;
  float cooldown = 0.6f;
  float activeTime = 9.0f;

  bool ended = false;
};


class Enemy{

 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void createEnemy(GLuint program, float hp, std::vector<glm::vec2> positions, std::vector<float> waitTime, std::vector<float>movementSpeed,GLuint bulletProg, std::vector<shootPattern> shootPatterns, std::vector<uint16_t> patternIndexes); // recebe tudo do setBehavior, shootPatterns e patternIndexes

  void setBehavior(GLuint program, float hp, std::vector<glm::vec2> positions, std::vector<float> waitTime, std::vector<float>movementSpeed);
  void update(glm::vec2 playerPos, const GameData &gameData, float deltaTime, std::vector<EnemyBullet> &eBullets);

  void executeShootPattern(std::vector<EnemyBullet> &eBullets);

  GLuint m_bulletProgram{};

  float m_speed{1.0f};
  glm::vec2 spawn{-0.3f, 0.75f};
  float m_scale{0.05f};

  uint16_t life{100};

  glm::vec2 m_translation{glm::vec2(0)};

  glm::vec2 playerPosition{glm::vec2(0)};

  bool m_destroyed{false};

  glm::vec4 m_color{1.0f,0.46f,0.46f,1.0f};

  std::vector<glm::vec2> m_positions;
  std::vector<float> m_waitTime;
  std::vector<float> m_movementSpeed;
  abcg::ElapsedTimer waitTimer;
  behaviorState m_state{Waiting};

  std::vector<shootPattern> m_shootPatterns{};
  std::vector<uint16_t> m_patternIndexes{};
  shootPattern m_currentPattern{};

  abcg::ElapsedTimer m_patternTimer;
  abcg::ElapsedTimer m_cooldownTimer;

 private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  
};


#endif