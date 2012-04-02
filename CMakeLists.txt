PROJECT (Renderer)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

SET(RENDERER_SRCS Renderer.cpp Importer/OBJImporter.cpp GLSLProgram/GLSLProgram.cpp
    SceneGraph/SceneGraph.cpp SceneGraph/SceneCamera.cpp SceneGraph/SceneNode.cpp
    SceneGraph/SceneNodeTriangleMesh.cpp)
SET(RENDERER_MOC_HDRS Renderer.h Importer/OBJImporter.h GLSLProgram/GLSLProgram.hpp
    SceneGraph/SceneGraph.h SceneGraph/SceneCamera.h SceneGraph/SceneNode.h
    SceneGraph/SceneNodeTriangleMesh.h)
SET(RENDERER_UIS )
SET(RENDERER_RCS )

ADD_DEFINITIONS(-Wall)

SET(CMAKE_CXX_FLAGS "-DGL_GLEXT_PROTOTYPES")

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_ADD_RESOURCES(RENDERER_RC_SRCS ${RENDERER_RCS})
QT4_WRAP_UI(RENDERER_UI_HDRS ${RENDERER_UIS})
QT4_WRAP_CPP(RENDERER_MOC_SRCS ${RENDERER_MOC_HDRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${QT_QT_INCLUDE_DIR}
                    ${QT_QTOPENGL_INCLUDE_DIR}
                    ${QT_INCLUDE_DIR})

ADD_EXECUTABLE (Renderer ${RENDERER_SRCS} ${RENDERER_MOC_SRCS} ${RENDERER_RC_SRCS}
    ${RENDERER_UI_HDRS})
TARGET_LINK_LIBRARIES (Renderer ${QT_LIBRARIES} ${QT_QTOPENGL_LIBRARY} GLEW)
