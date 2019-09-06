cmake_minimum_required(VERSION 3.14)
project(flower-power)

set(CMAKE_CXX_STANDARD 14)

add_executable(TP1 src/main.cpp src/menu/Menu.cpp src/menu/Menu.h src/menu/SettingsMenu.cpp src/menu/SettingsMenu.h src/menu/MainMenu.cpp src/menu/MainMenu.h src/utils/StringUtils.cpp src/utils/StringUtils.h)