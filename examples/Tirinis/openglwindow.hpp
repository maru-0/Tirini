#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>
#include <vector>

#include "abcg.hpp"


#include "player.hpp"
#include "starlayers.hpp"
#include "enemy.hpp"
#include "enemybullet.hpp"
#include "stage1.hpp"


class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

  void checkCollisions();


 private:
  GLuint m_objectsProgram{};
  GLuint m_starsProgram{};
  GLuint m_bulletsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};
  uint16_t defeated = 0;
  GameData m_gameData;

  Player m_player;
  StarLayers m_starLayers;

  abcg::ElapsedTimer m_restartWaitTimer;

  std::default_random_engine m_randomEngine;

  std::vector<Enemy> m_enemies{};
  
  std::vector<EnemyBullet> m_eBullets{};
  glm::vec2 playerPosition;

  Stage1 stage;

  void restart();
  void update();
};

#endif