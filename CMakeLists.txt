cmake_minimum_required(VERSION 3.10)

# set the project name
project(naive_calculator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -O2")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

FIND_PACKAGE(Boost REQUIRED COMPONENTS filesystem program_options system)

file(GLOB SOURCES "src/*.cpp")

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(include)

    add_executable(${PROJECT_NAME} ${SOURCES})

    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

# cppcheck --enable=all  $< --language=c++ -I $(INC_DIR) --suppress=missingIncludeSystem --check-config
# clang-tidy -checks=*  src/main.cpp -- -Iinclude