cmake_minimum_required(VERSION 3.14)
project(flower-power)

set(CMAKE_CXX_STANDARD 14)

add_executable(TP1 src/main.cpp src/menu/Menu.cpp src/menu/Menu.h src/menu/SettingsMenu.cpp src/menu/SettingsMenu.h src/menu/MainMenu.cpp src/menu/MainMenu.h src/core/System.cpp src/core/System.h src/core/job/producer/ProducerJob.cpp src/core/job/producer/ProducerJob.h src/core/job/seller/SellerJob.cpp src/core/job/seller/SellerJob.h src/core/config/Config.cpp src/core/config/Config.h src/core/scheduler/Scheduler.cpp src/core/scheduler/Scheduler.h src/core/job/Job.cpp src/core/job/Job.h src/core/DistributionCenter.cpp src/core/DistributionCenter.h src/utils/CSVReader.cpp src/utils/CSVReader.h src/core/config/data/ProducerFlowers.h src/utils/Definitions.h src/utils/Pipe.cpp src/utils/Pipe.h src/core/config/data/ProducerFlowers.cpp src/utils/Logger.cpp src/utils/Logger.h src/core/config/data/Seller.cpp src/core/config/data/Seller.h)