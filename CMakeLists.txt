cmake_minimum_required(VERSION 2.8)

project(gbemu)
add_executable(${PROJECT_NAME}
	"main.cpp"
	"romfile.cpp"
	"cpu.cpp"
	"register.cpp"
)

set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -Wpedantic -Wno-c99-extensions")
