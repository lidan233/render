cmake_minimum_required(VERSION 3.14)
project(new)

set(CMAKE_CXX_STANDARD 14)

set(ASSIMP_BUILD_STATIC_LIB ON)


include_directories(
        externel/dirent
        externel/glfw/include
        externel/glm/
        externel/glad/include
        externel/glad/include/glad
        externel/assimp/include
        externel/include/
        headers
)
link_directories(
        externel/glfw/lib-mingw-w64
        externel/assimp/lib/
        externel/alllib/
        # 這個allib只是爲了引入libtiff以及其支持庫
)

add_executable(new
        externel/glad/src/glad.c

        headers/Bound.h
        headers/stb_image.h
        headers/stb_image.cpp
        headers/Model.h
        headers/Model.cpp
        headers/calcuateCurvature.h
        headers/logging.h
        headers/Mesh.h
        headers/Mesh.cpp
        headers/CheckShader.h

        newview/newView.h
        newview/newView.cpp
        newview/NewCamera.h
        newview/WriteImage.h
        newview/Rgb2Hsv.h
        newview/IteratorForLight.h
        headers/basestructure.h)
target_link_libraries(new
        glfw3.dll
        libassimp.dll
        libtiff.a
        libjpeg.a
        liblzma.a
        libgmp.a
        libzstd.a
        libz.a
        libintl.a
        libiconv.a
        libgettextpo.a
        libgettextlib.a
        libcharset.a
        libgmpxx.a
        libtiffxx.a
        libturbojpeg.a)
