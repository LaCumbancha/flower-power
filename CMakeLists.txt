cmake_minimum_required(VERSION 3.14)
project(flower-power)

set(CMAKE_CXX_STANDARD 14)

add_executable(TP1 src/main.cpp src/menu/Menu.cpp src/menu/Menu.h src/menu/SettingsMenu.cpp src/menu/SettingsMenu.h src/menu/MainMenu.cpp src/menu/MainMenu.h src/core/System.cpp src/core/System.h src/core/producer/ProducerJob.cpp src/core/producer/ProducerJob.h src/core/seller/SellerJob.cpp src/core/seller/SellerJob.h src/core/config/Config.cpp src/core/config/Config.h)