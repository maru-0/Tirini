#include "enemy.hpp"

#include <iostream>

void Enemy::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_translation = glm::vec2(spawn);
  
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

void Enemy::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;
  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform2f(m_translationLoc, m_translation.x, m_translation.y);
  glUniform1f(m_scaleLoc, m_scale);
  glUniform4f(m_colorLoc, m_color.r ,m_color.g ,m_color.b ,m_color.a);
  glDrawElements(GL_TRIANGLES, 14 * 3, GL_UNSIGNED_INT, nullptr);


  glBindVertexArray(0);

  glUseProgram(0);
}

void Enemy::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Enemy::update( glm::vec2 playerPos,const GameData &gameData, float deltaTime, std::vector<EnemyBullet> &eBullets){
  if(gameData.m_state != State::Playing) return;
  playerPosition = playerPos;
  executeShootPattern(eBullets);


  switch (m_state){
  case Waiting:
    if(m_waitTime.size() == 0){
      m_destroyed = true;
      break;
    } 
    if(waitTimer.elapsed() > m_waitTime[0]){
      m_waitTime.erase(m_waitTime.begin());
      m_state = Moving;
    }

    break;

  case Moving:
    glm::vec2 increment{m_positions[0] - m_translation};
    increment /= glm::distance(increment, glm::vec2{0});
    m_translation += (increment*deltaTime*m_movementSpeed[0]);
    if(glm::distance(m_positions[0], m_translation) < 0.005f){
      m_translation = m_positions[0];
      m_positions.erase(m_positions.begin());
      m_movementSpeed.erase(m_movementSpeed.begin());
      waitTimer.restart();
      m_state = Waiting;
    }
    break;

  }
}

void Enemy::setBehavior(GLuint program, float hp, std::vector<glm::vec2> positions, std::vector<float> waitTime, std::vector<float>movementSpeed){
  life = hp;
  m_positions = positions;
  m_waitTime = waitTime;
  m_movementSpeed = movementSpeed;
  m_state = Waiting;

  spawn = m_positions[0];
  m_positions.erase(m_positions.begin());

  initializeGL(program);
  
  waitTimer.restart();
}


void Enemy::executeShootPattern(std::vector<EnemyBullet> &eBullets){
  if(m_currentPattern.ended){
    if(m_patternIndexes.size() == 0){
      return;
    }
    m_currentPattern = m_shootPatterns[m_patternIndexes[0]];
    m_patternIndexes.erase(m_patternIndexes.begin());
    m_patternTimer.restart();
  }else if( m_currentPattern.cooldown > m_cooldownTimer.elapsed() ) return;
  if(m_patternTimer.elapsed() > m_currentPattern.activeTime){
    m_currentPattern.ended = true;
    return;
  }
  
  if(m_currentPattern.bullet.m_aimed) {
    m_currentPattern.bullet.m_direction = playerPosition - m_translation;
    m_currentPattern.bullet.m_direction.x *= 4.0f/3;
  }
  glm::vec2 baseDir{m_currentPattern.bullet.m_direction};
  baseDir /= glm::distance(baseDir, glm::vec2{0});
  
  

  eBullets.push_back(EnemyBullet{ m_currentPattern.bullet});
  eBullets.back().initializeGL(m_bulletProgram);
  eBullets.back().m_direction = baseDir;
  eBullets.back().m_translation = m_translation + m_scale*baseDir;
  // float angle = glm::wrapAngle(m_currentPattern.angle);
  float angle = m_currentPattern.angle;
  for(int i = 0; i<m_currentPattern.nSpread-1; i++){
    eBullets.push_back(EnemyBullet{ m_currentPattern.bullet});
    eBullets.back().initializeGL(m_bulletProgram);
    eBullets.back().m_direction = glm::rotate(baseDir, ((float)((2*(i%2)*M_PI) + pow(-1.0, double(i))*(angle/(m_currentPattern.nSpread-1))*(1.0 + floor(i/2)))));
    eBullets.back().m_translation = m_translation + m_scale*baseDir;
    
    
  }  
  m_cooldownTimer.restart();  


}
#include<iostream>
void Enemy::createEnemy(GLuint program, float hp, std::vector<glm::vec2> positions, std::vector<float> waitTime, std::vector<float>movementSpeed,GLuint bulletProg, std::vector<shootPattern> shootPatterns, std::vector<uint16_t> patternIndexes){
  setBehavior(program, hp, positions, waitTime,movementSpeed);
  m_shootPatterns = shootPatterns;
  m_patternIndexes = patternIndexes;
  m_bulletProgram = bulletProg;
  

  if(m_patternIndexes.size() == 0) return;
  m_currentPattern = m_shootPatterns[m_patternIndexes[0]];
  m_patternIndexes.erase(m_patternIndexes.begin());
  m_patternTimer.restart();
}