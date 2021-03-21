#include "stage1.hpp"

#include "enemybullet.hpp"



void Stage1::update(std::vector<Enemy> &enemies){
    switch (currentState)
    {
    case Wave01:
        if(waveTimer.elapsed() > currentWait) {
            currentState = Wave01;
            waveDone = false;
            waveTimer.restart();
            break;
        }
        if(waveDone) break;
        waveTimer.restart();
        waveDone = true;  

        wave01(enemies);
        currentWait = 9.8f;
        break;
    }
}

void Stage1::wave01(std::vector<Enemy> &enemies){
    enemies.push_back(Enemy{});

    std::vector<shootPattern> shoots{};
    std::vector<uint16_t> indexes{};

    indexes.push_back(0);

    shoots.push_back(shootPattern{});
    enemies.back().m_currentPattern.bullet.m_aimed = true;
    enemies.back().createEnemy(m_objectsProgram, 60,  //hp
                            std::vector<glm::vec2>{glm::vec2{-1.0f, 1.0f }, 
                                                    glm::vec2{-0.6f, 0.6f}, 
                                                    glm::vec2{-1.0f, 1.0f}},//positions
                                         std::vector<float>{0.1f,6.0f}, //wait timer
                                         std::vector<float>{1.5f,1.5f}, //move speed
                                         m_bulletsProgram , shoots, indexes);


    enemies.push_back(Enemy{});


    // enemies.back().m_currentPattern.bullet.m_aimed = true;
    enemies.back().createEnemy(m_objectsProgram, 60,  //hp
                            std::vector<glm::vec2>{glm::vec2{0.40f, 1.0f }, 
                                                    glm::vec2{0.f, 0.6f}, 
                                                    glm::vec2{0.35f, 1.0f}},//positions
                                         std::vector<float>{0.1f,6.0f}, //wait timer
                                         std::vector<float>{1.5f,1.5f}, //move speed
                                         m_bulletsProgram , shoots, indexes);

    
}
