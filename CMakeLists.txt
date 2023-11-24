cmake_minimum_required(VERSION 3.28)
project(rayTracing)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/entity/plane.cpp
    src/entity/sphere.cpp
    src/geometry/vector.cpp
    main.cpp
)

# Add an executable with the above sources
add_executable(rayTracing ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(rayTracing
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include/entity
        ${PROJECT_SOURCE_DIR}/include/geometry
)