cmake_minimum_required(VERSION 3.10.2)
project(cpu_temperature_diagnostics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_library(REQUIRED libsensors)
find_package(catkin REQUIRED COMPONENTS diagnostic_updater roscpp)

catkin_package(INCLUDE_DIRS include CATKIN_DEPENDS roscpp)

# ##############################################################################
# Build ##
# ##############################################################################

add_executable(
  ${PROJECT_NAME}_node
  src/main.cc src/sensor_chip.cc src/temperature_feature.cc
  src/cpu_temperature_diagnostic.cc)

target_include_directories(${PROJECT_NAME}_node PRIVATE include)

target_include_directories(${PROJECT_NAME}_node SYSTEM
                           PRIVATE ${catkin_INCLUDE_DIRS})

target_compile_options(${PROJECT_NAME}_node PRIVATE -Wall -Wextra -pedantic
                                                    -Werror)

target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} sensors)

# ##############################################################################
# Install ##
# ##############################################################################
install(TARGETS ${PROJECT_NAME}_node
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
