CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(todomgr VERSION 0.1)
SET(BUILD_SHARED_LIBS ON)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/)
SET(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

AUX_SOURCE_DIRECTORY(src/ DIR_SRC)
ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRC})

# -------------------FTXUI--------------------- #
INCLUDE(FetchContent)

FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
IF(NOT ftxui_POPULATED)
    FetchContent_MakeAvailable(ftxui)
    ADD_SUBDIRECTORY(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component # Not needed for this example.
)
# --------------------------------------------- #

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
    PRIVATE src/      
    PRIVATE ${ftxui_SOURCE_DIR}/include/
)

FILE(GLOB DEPENDENCY_DLLS "${ftxui_BINARY_DIR}/*.dll")
FOREACH(DLL ${DEPENDENCY_DLLS})
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
ENDFOREACH()
