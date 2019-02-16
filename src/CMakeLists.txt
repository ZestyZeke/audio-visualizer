add_executable(analyzable
        main.cpp Visualizer.cpp Visualizer.h Analyzer.cpp Analyzer.h
        Engine.cpp Engine.h Extrema.cpp Extrema.h utils.cpp utils.h)

target_link_libraries(analyzable ${SFML_LIBRARIES} ${SFML_DEPENDENCIES}
        Aquila fftw3)
target_link_libraries(analyzable Threads::Threads)

file(GLOB MY_RESOURCE_FILES
        ${PROJECT_SOURCE_DIR}/res/*.wav)
file(COPY ${MY_RESOURCE_FILES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
