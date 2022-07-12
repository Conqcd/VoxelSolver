include(${PROJECT_SOURCE_DIR}/third_party/fmt.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/spdlog.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/glfw.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/glm.cmake)
include(${PROJECT_SOURCE_DIR}/third_party/yaml_cpp.cmake)
# include(${PROJECT_SOURCE_DIR}/third_party/itk.cmake)

if (WIN32)
add_definitions(-DRE_WINDOWS)
endif()
# add_definitions(-DRE_ENABLE_ASSERT)
add_definitions(-DRE_BUILD_DLL)

find_package(ITK REQUIRED)

file(GLOB_RECURSE RENGINE_SRC *.cpp *.hpp)

# message(${ITK_INCLUDE_DIRS})

include_directories(../TinyOBJLoader)
include_directories(../Glad/include)
include_directories(../Imgui)
include_directories(../ImGuizmo)
include_directories(../rengine-src)
include_directories(../Stb)
include_directories(../Entt)
include_directories(${PROJECT_SOURCE_DIR}/Eigen)
# include_directories(${ITK_USE_FILE})
include_directories(${ITK_INCLUDE_DIRS})

# add_library(rengine SHARED ${RENGINE_SRC})
add_library(rengine STATIC ${RENGINE_SRC})
target_link_libraries(rengine
    PRIVATE
    spdlog
    opengl32
    glfw
    glad
    tinyobjloader
    imgui
    imguizmo
    glm
    yaml-cpp
    eigen
    ${ITK_LIBRARIES}
    )