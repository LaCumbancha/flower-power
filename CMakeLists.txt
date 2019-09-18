cmake_minimum_required(VERSION 3.14)
project(flower-power)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        TP1
        src/main.cpp
        src/menu/Menu.cpp
        src/menu/Menu.h
        src/menu/SettingsMenu.cpp
        src/menu/SettingsMenu.h
        src/menu/MainMenu.cpp
        src/menu/MainMenu.h
        src/core/System.cpp
        src/core/System.h
        src/core/job/producer/ProducerJob.cpp
        src/core/job/producer/ProducerJob.h
        src/core/job/seller/SellerJob.cpp
        src/core/job/seller/SellerJob.h
        src/core/config/Config.cpp
        src/core/config/Config.h
        src/core/scheduler/Scheduler.cpp
        src/core/scheduler/Scheduler.h
        src/core/job/Job.cpp
        src/core/job/Job.h
        src/core/DistributionCenter.cpp
        src/core/DistributionCenter.h
        src/utils/CSVReader.cpp
        src/utils/CSVReader.h
        src/core/config/data/FlowerBox.cpp
        src/core/config/data/FlowerBox.h
        src/utils/Definitions.h
        src/utils/Pipe.cpp
        src/utils/Pipe.h
        src/utils/Logger.cpp
        src/utils/Logger.h
        src/core/config/data/Seller.h
        src/core/config/data/Seller.cpp
        src/core/job/classifier/ClassifierJob.cpp
        src/core/job/classifier/ClassifierJob.h
        src/core/config/data/Flower.cpp
        src/core/config/data/Flower.h
        src/core/job/classifier/ClassifierJob.cpp
        src/core/job/classifier/ClassifierJob.h
        src/simulator/ClientSimulator.cpp
        src/simulator/ClientSimulator.h
        # tests/test1.cpp
        # tests/SellerTests.cpp
        # tests/DistributionCenterTests.cpp
        # tests/ClientSimulatorTests.cpp
        src/core/config/data/BouquetRequest.cpp src/core/config/data/BouquetRequest.h)

# # Download and unpack googletest at configure time
# configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
# execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
# RESULT_VARIABLE result
# WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
# message(FATAL_ERROR "CMake step for googletest failed: ${result}")
# endif()
# execute_process(COMMAND ${CMAKE_COMMAND} --build .
# RESULT_VARIABLE result
# WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
# if(result)
# message(FATAL_ERROR "Build step for googletest failed: ${result}")
# endif()
#
# # Prevent overriding the parent project's compiler/linker settings on Windows.
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#
# # Add googletest directly to our build. This defines the gtest and gtest_main targets.
# add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
# ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
# EXCLUDE_FROM_ALL)
#
# # The gtest/gtest_main targets carry header search path dependencies automatically when using CMake 2.8.11 or later. Otherwise we have to add them here ourselves.
# if (CMAKE_VERSION VERSION_LESS 2.8.11)
# include_directories("${gtest_SOURCE_DIR}/include")
# endif()
#
# # Now simply link against gtest or gtest_main as needed. Eg
# target_link_libraries(TP1 gtest_main)
# add_test(NAME example_test COMMAND example)
