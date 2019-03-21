cmake_minimum_required(VERSION 3.12)

project(PairwiseStitching)

# Find ITK.
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(PairwiseStitching main.cpp Stitching.cpp PairwiseStitching.cpp ImageRegistrationMethodTranslation.cxx  )

target_link_libraries(PairwiseStitching ${ITK_LIBRARIES})

file(GLOB DATA_FILES "*.png")
file( COPY ${DATA_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()

add_test(NAME PairwiseStitchingTest COMMAND PairwiseStitching WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})