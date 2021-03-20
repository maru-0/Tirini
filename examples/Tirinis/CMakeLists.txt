project(tirinis)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp player.cpp starlayers.cpp playerbullet.cpp enemy.cpp)
enable_abcg(${PROJECT_NAME})