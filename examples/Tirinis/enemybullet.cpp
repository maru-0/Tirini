#include "enemybullet.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cppitertools/itertools.hpp>

#include<iostream>

void EnemyBullet::initializeGL(GLuint program){
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
//   m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  std::array<glm::vec2, 4> positions{
    glm::vec2{-1.0f, 1.0f},glm::vec2{1.0f, 1.0f},
    glm::vec2{-1.0f, -1.0f},glm::vec2{1.0f, -1.0f}, 
    };


  std::array indices{0,1,3,
                     0,2,3};

//   std::vector<glm::vec2> positions(0);
//   positions.emplace_back(0, 0);
//   auto step{M_PI * 2 / sides};
//   for (auto angle : iter::range(0.0, M_PI * 2, step)) {
//     positions.emplace_back(std::cos(angle), std::sin(angle));
//   }
//   positions.push_back(positions.at(1));

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
void EnemyBullet::paintGL(const GameData &gameData){
  if (gameData.m_state != State::Playing) return;
  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform2f(m_translationLoc, m_translation.x, m_translation.y);
  glUniform1f(m_scaleLoc, m_scale);
  glUniform4f(m_colorLoc,  m_color.r ,m_color.g ,m_color.b ,m_color.a);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  glDisable(GL_BLEND);
  glBindVertexArray(0);

  glUseProgram(0);
}
void EnemyBullet::terminateGL(){
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void EnemyBullet::update(const GameData &gameData, float deltaTime){
  if(gameData.m_state != State::Playing) return;

  glm::vec2 increment = (m_direction/glm::distance(glm::vec2{0}, m_direction))*(deltaTime*m_speed);
  increment.x *= 3.0f/4.0f;
  
  m_translation += increment;

  if(m_translation.x > 0.3f + m_scale || m_translation.x < -0.95f - m_scale || m_translation.y > 0.95f + m_scale || m_translation.y < -0.95f - m_scale ){
    m_hit = true;
  }
}

void EnemyBullet::setParams(GLuint program, glm::vec2 translation, glm::vec2 direction, float scale, float speed, glm::vec4 color){
  m_translation = translation;
  m_direction = direction;
  m_scale = scale;
  m_speed = speed;
  m_color = color;
  m_program = program;

  initializeGL(m_program);
}