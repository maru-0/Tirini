#ifndef STAGE1_HPP_
#define STAGE1_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

#include <imgui.h>

#include <random>
#include <vector>

#include "abcg.hpp"

#include "enemy.hpp"
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cppitertools/itertools.hpp>

#include<vector>


enum waves {Wave01,Wave02,Wave03,Wave04,Wave05,Wave06};
class Stage1{
    public:

    void update(std::vector<Enemy> &enemies);

    Stage1(){
        // m_objectsProgram = po;
        // m_bulletsProgram = pb;
        waveTimer.restart();
    
    }

    float currentWait = 10.0f;
    waves currentState = Wave01;
    bool waveDone = false;
    abcg::ElapsedTimer waveTimer;

    GLuint m_objectsProgram;
    GLuint m_bulletsProgram;

    bool stageDone{false};
    float speedfactor{1.25f};
    int iterations=1;

    void wave01(std::vector<Enemy> &enemies);
    void wave02(std::vector<Enemy> &enemies);
    void wave03(std::vector<Enemy> &enemies);
    void wave04(std::vector<Enemy> &enemies);
    void wave05(std::vector<Enemy> &enemies);
    void wave06(std::vector<Enemy> &enemies);
    void wave07(std::vector<Enemy> &enemies);
};

#endif