#include "openglwindow.hpp"
#include <imgui.h>

#include "abcg.hpp"
#include <iostream>
#include <list>
void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_z)
      m_gameData.m_input.set(static_cast<size_t>(Input::Shoot));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_LSHIFT )
      m_gameData.m_input.set(static_cast<size_t>(Input::Focus));
    if (event.key.keysym.sym == SDLK_r )
      m_gameData.m_input.set(static_cast<size_t>(Input::Restart));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_z)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Shoot));
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_LSHIFT )
      m_gameData.m_input.reset(static_cast<size_t>(Input::Focus));
    if (event.key.keysym.sym == SDLK_r )
      m_gameData.m_input.reset(static_cast<size_t>(Input::Restart)); 
  }
  

}

void OpenGLWindow::initializeGL() {
  
  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert",
                                           getAssetsPath() + "stars.frag");

  glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_restartWaitTimer.restart();
  m_starLayers.initializeGL(m_starsProgram, 25);
  m_player.initializeGL(m_objectsProgram);

  m_enemies.clear();
  m_player.m_bullets.clear();

  m_enemies.push_back(Enemy{});
  for(auto &enemy : m_enemies){
    enemy.setBehavior(m_objectsProgram, 100, std::vector<glm::vec2>{glm::vec2{-1.0f, 1.0f },glm::vec2{-0.6f, -0.5f},glm::vec2{0.0f, 0.5f},glm::vec2{-1.0f, 1.0f }},
                      std::vector<float>{0.5f,0.8f,0.8f}, std::vector<float>{1.5f,1.5f,1.5f});
  }
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  if(m_restartWaitTimer.elapsed() > 1.0f && m_gameData.m_input[static_cast<size_t>(Input::Restart)]){
    restart();
    return;
  }
  for(auto &enemy : m_enemies){
    enemy.update(m_gameData, deltaTime);
  }

  m_player.update(m_gameData, deltaTime);

  for(auto &bullet : m_player.m_bullets){
    bullet.update(m_gameData, deltaTime);
  }
  
  m_starLayers.update(m_player, deltaTime);

  checkCollisions();

  for(auto i = m_player.m_bullets.begin(); i != m_player.m_bullets.end(); ++i){
    if(i->m_hit){
      m_player.m_bullets.erase(i);
      i--;
    }
  }
  for(auto i = m_enemies.begin(); i != m_enemies.end(); ++i){
    if(i->m_destroyed){
      m_enemies.erase(i);
      i--;
    }
  }

}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_starLayers.paintGL();
  for(auto &enemy : m_enemies){
    enemy.paintGL(m_gameData);
  }
  m_player.paintGL(m_gameData);
  for(auto &bullet : m_player.m_bullets){
    bullet.paintGL(m_gameData);
  }
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    auto size{ImVec2(300, 85)};
    auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                         (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_starsProgram);
  glDeleteProgram(m_objectsProgram);

  m_player.terminateGL();
  m_starLayers.terminateGL();
}

#include<iostream>
void OpenGLWindow::checkCollisions(){
  
  for(auto i = m_enemies.begin(); i != m_enemies.end(); ++i){
    if(!m_player.m_invincibility && (glm::distance(m_player.m_translation, i->m_translation) < (m_player.m_scale/4) + i->m_scale)){
      m_player.damage();
      std::cout << m_player.m_lives << std::endl;
    }
    for(auto j = m_player.m_bullets.begin(); j != m_player.m_bullets.end(); ++j){
      if(glm::distance(j->m_translation, i->m_translation) < (j->m_scale/2) + i->m_scale){
        i->life--;
        std::cout << i->life << std::endl;
        if(i->life == 0) i->m_destroyed = true;
        j->m_hit = true;
      }
    }
  }
}