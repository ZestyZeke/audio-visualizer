add_executable(analyzable
        main.cpp Visualizer.cpp Analyzer.cpp
        Engine.cpp Extrema.cpp utils.cpp Config.cpp)

target_compile_features(analyzable PUBLIC cxx_std_17)

target_link_libraries(analyzable ${CONAN_LIBS})
target_link_libraries(analyzable Aquila)
target_link_libraries(analyzable Threads::Threads)
target_link_libraries(analyzable stdc++fs)

file(GLOB MY_RESOURCE_FILES
        ${PROJECT_SOURCE_DIR}/res/*.wav)
file(COPY ${MY_RESOURCE_FILES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MY_CONFIG_FILES
        ${PROJECT_SOURCE_DIR}/src/*.xml)
file(COPY ${MY_CONFIG_FILES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
