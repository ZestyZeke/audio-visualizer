
add_executable(analyzable
    Analyzable.cpp
    Analyzable.hpp
    Main.cpp Visualizer.cpp Visualizer.h)

target_link_libraries(analyzable ${SFML_LIBRARIES} ${SFML_DEPENDENCIES}
        Aquila)
target_link_libraries(analyzable fftw3)
#target_link_libraries(analyzable Aquila)

file(COPY ${PROJECT_SOURCE_DIR}/res/Kingpin.wav
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/res/444.wav
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
