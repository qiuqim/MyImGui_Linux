cmake_minimum_required(VERSION 3.10)
project(my_imgui_project)

set(CMAKE_CXX_STANDARD 17)

# 添加宏
 add_definitions(-DIMGUI_DISABLE_DEBUG_TOOLS)

# gdb调试
# add_definitions(-g)


# 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)



# 添加ImGui源文件
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    # imgui/misc/freetype/imgui_freetype.cpp
    misc/stb_img/stb_image.cpp
)

set(MY_IMGUI_SOURCES
    src/custom_widgets.cpp
    src/theme.cpp
)

# 查找必要的包
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
# find_package(Freetype REQUIRED)  # 添加 FreeType 包的查找


# 添加可执行文件
add_executable(${PROJECT_NAME} 
    src/main.cpp
    ${IMGUI_SOURCES}
    ${MY_IMGUI_SOURCES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/freetype
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/freetype/include
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/stb_img
    # ${CMAKE_CURRENT_SOURCE_DIR}/misc/freetype/include/freetype
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    dl
    pthread
    # Freetype::Freetype
)