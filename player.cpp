#include "player.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Player::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_translation = glm::vec2(spawn);
  m_lives = 5;
  //clear na lista de bullets
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  std::array<glm::vec2, 28> positions{
      // Ship body
      glm::vec2{-02.5f, 12.5f}, glm::vec2{-15.5f, 02.5f},
      glm::vec2{-15.5f, -12.5f}, glm::vec2{-09.5f, -07.5f},
      glm::vec2{-03.5f, -12.5f}, glm::vec2{03.5f, -12.5f},
      glm::vec2{09.5f, -07.5f}, glm::vec2{15.5f, -12.5f},
      glm::vec2{15.5f, 02.5f}, glm::vec2{02.5f, +12.5f},

      // Cannon left
      glm::vec2{-12.5f, +10.5f}, glm::vec2{-12.5f, +04.0f},
      glm::vec2{-09.5f, +04.0f}, glm::vec2{-09.5f, +10.5f},

      // Cannon right
      glm::vec2{+09.5f, +10.5f}, glm::vec2{+09.5f, +04.0f},
      glm::vec2{+12.5f, +04.0f}, glm::vec2{+12.5f, +10.5f},
      
      // Thruster trail (left)
      glm::vec2{-12.0f, -07.5f}, 
      glm::vec2{-09.5f, -18.0f}, 
      glm::vec2{-07.0f, -07.5f},

      // Thruster trail (right)
      glm::vec2{+07.0f, -07.5f}, 
      glm::vec2{+09.5f, -18.0f}, 
      glm::vec2{+12.0f, -07.5f},

      glm::vec2{0.0f, +07.25f}, 
      glm::vec2{+07.25f, 0.0f}, 
      glm::vec2{0.0f, -07.25f},
      glm::vec2{-07.25f, 0.0f},
      };

  // Normalize
  for (auto &position :positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array indices{24, 25, 26,
                     24, 26, 27,
                     0, 1, 3,
                     1, 2, 3,
                     0, 3, 4,
                     0, 4, 5,
                     9, 0, 5,
                     9, 5, 6,
                     9, 6, 8,
                     8, 6, 7,
                     // Cannons
                     10, 11, 12,
                     10, 12, 13,
                     14, 15, 16,
                     14, 16, 17,
                     // Thruster trails
                     18, 19, 20,
                     21, 22, 23};

  // Generate VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void Player::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;
  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform2f(m_translationLoc, m_translation.x, m_translation.y);
  glUniform1f(m_scaleLoc, m_scale);

  glUniform4f(m_colorLoc, m_color.r ,m_color.g ,m_color.b ,m_color.a);
  glDrawElements(GL_TRIANGLES, 14 * 3, GL_UNSIGNED_INT, nullptr);

  //hitbox draw
  glUniform4f(m_colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);

  glUseProgram(0);
}

void Player::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

#include<iostream>
void Player::update(const GameData &gameData, float deltaTime) {
  if(gameData.m_state != State::Playing) return;
  glm::vec2 newTranslation = glm::vec2{m_translation};
  glm::vec2 posIncrement{0};
  float focusFactor = 1.0f;

  //Invincibility
  if(m_invincibility && (m_invincibilityTimer.elapsed() > 1.0f)){
    m_color = glm::vec4{0.46f,0.46f,0.46f,1.0f};
    m_invincibility = false;
  }


  //Movement
  if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
    posIncrement.y += 1.0f;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Down)]) {
    posIncrement.y -= 1.0f;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
    posIncrement.x -= 1.0f;
  }
  if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
    posIncrement.x += 1.0f;
  }
  if(glm::distance(posIncrement, glm::vec2{0}) != 0){
    posIncrement /= glm::distance(posIncrement, glm::vec2{0});
  }

  if (gameData.m_input[static_cast<size_t>(Input::Focus)]) {
    posIncrement *= m_focus;
    focusFactor = 0.20f;
  }
  posIncrement.x *= 3.0f/4.0f;
  newTranslation = m_translation + (posIncrement*m_speed*deltaTime);


  //Movement boundaries
  if(newTranslation.x < -0.9f + m_scale){
    newTranslation.x = m_translation.x;
  }else if(newTranslation.x > 0.3f - m_scale){
    newTranslation.x = m_translation.x;
  }
  if(newTranslation.y < -1.0f+2.0/30 + m_scale){
    newTranslation.y = m_translation.y;
  }else if(newTranslation.y > 1.0f-2.0/30 - m_scale){
    newTranslation.y = m_translation.y;
  }
  m_translation = newTranslation;

  // Shoot
  if (gameData.m_input[static_cast<size_t>(Input::Shoot)] && m_bulletCoolDownTimer.elapsed() > 0.1f){ 
    for (auto i : {-0.45f,-0.32f,-0.16f, 0.0f, 0.16f,0.32f,0.45f}) {
      m_bullets.push_back(PlayerBullet{});
      m_bullets.back().m_translation = m_translation;
      m_bullets.back().m_translation.y += m_scale;
      m_bullets.back().m_direction = glm::vec2{i*focusFactor, 1.0f};
      m_bullets.back().m_color = glm::vec4{0.6f, 0.5f, 1.0f, 0.25f};
      m_bullets.back().initializeGL(m_program);
      m_bulletCoolDownTimer.restart();
    }
  }
}

void Player::damage(){
  m_lives--;
  m_invincibility = true;
  m_invincibilityTimer.restart();
  m_color = glm::vec4{0.16f,0.16f,0.16f,1.0f};
}