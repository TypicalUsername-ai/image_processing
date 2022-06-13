cmake_minimum_required(VERSION 3.13)
set(PROJECT_ID image_processor)

project(${PROJECT_ID})

#set(CMAKE_C_STANDARD 11)
# Ustaw standard języka C++.
set(CMAKE_CXX_STANDARD 17) ## MANUAL

include_directories(include) ## MANUAL - allow for all header files inclusion


#in windows the add -Wno-multichar
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

if (WIN32)
    add_compile_options(-Wno-multichar)
endif (WIN32)

set(EXEC_MAIN ${PROJECT_ID}__main)
set(SOURCE_FILES src/improc.cpp src/bitmap.c)

add_executable(${EXEC_MAIN} ${SOURCE_FILES} main.cpp)


set(SOURCE_FILES_TESTS_AUX test/own/test_matrix.cpp test/own/test_Image.cpp)
set(SOURCES_FILES_TESTS_MAIN test/main_gtest.cpp test/test_io.cpp test/test_aux.cpp test/test_improc.cpp)

##MANUAL -> pasted cmake code for inclusions of GTEST library
if(EXISTS ${PROJECT_SOURCE_DIR}/test)
    set(EXEC_TEST ${PROJECT_ID}__test)
    add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS_MAIN} ${SOURCE_FILES_TESTS_AUX})

    # == Uwzględnij pliki frameworku Google Testing Framework (GTF) ==

    # Przyjmij, że główny katalog z plikami frameworku GTF znajduje się
    # dwa katalogi wyżej względem katalogu projektu.
    set(GTEST_ROOT ../googletest-master)

    # Dodaj katalogi z plikami nagłówkowymi GTF.
    target_include_directories(${EXEC_TEST} PUBLIC
            ${GTEST_ROOT}/googlemock/include
            ${GTEST_ROOT}/googletest/include
            )

    add_subdirectory(${GTEST_ROOT} googletest-master)

    # Dołącz bibliotekę Google Mock.
    target_link_libraries(${EXEC_TEST} gmock)
endif()