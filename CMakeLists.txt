cmake_minimum_required(VERSION 3.24)
project(DConsole)

set(CMAKE_CXX_STANDARD 17)

add_executable(app/main.cpp src/datecslib/datecslib.cpp include/datecslib/datecslib.h src/helpFunc/helpFunc.cpp include/helpFunc/helpFunc.h src/datecsException/datecsException.cpp include/datecsException/datecsException.h include/userFunc/userFunc.h src/userFunc/userFunc.cpp)
