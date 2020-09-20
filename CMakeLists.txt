cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(PongByCan)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} 
		    ${SDL2_IMAGE_INCLUDE_DIRS} 
		    ${SDL2_TTF_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} 
		FP/main.cpp 
		FP/Game.cpp 
		FP/CollisionDetection.cpp 
		FP/GameObjects.cpp 
		FP/Timer.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} 
				${SDL2_IMAGE_LIBRARIES} 
				${SDL2_TTF_LIBRARIES})
