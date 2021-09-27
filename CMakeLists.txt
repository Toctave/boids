cmake_minimum_required(VERSION 3.10)

project(boids)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
  src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE
  SFML-2.5.1/include)
target_link_directories(${PROJECT_NAME} PRIVATE
  SFML-2.5.1/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE
  sfml-graphics
  sfml-window
  sfml-system)
