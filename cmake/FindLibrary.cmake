
# PkgConfig
include(FindPkgConfig)

# pixman
pkg_check_modules(PIXMAN pixman-1)
if (NOT PIXMAN_FOUND)
    message(FATAL_ERROR "pixman not found!")
else ()
    message(STATUS "pixman library status:")
    message(STATUS "    version: ${PIXMAN_VERSION}")
    message(STATUS "    include path: ${PIXMAN_INCLUDE_DIRS}")
    message(STATUS "    lib path: ${PIXMAN_LIBDIR}")
    message(STATUS "    libraries: ${PIXMAN_LIBRARIES}")
endif ()

# cairo
pkg_check_modules(CAIRO cairo)
if (NOT CAIRO_FOUND)
    message(FATAL_ERROR "Cairo not found!")
else ()
    message(STATUS "Cairo library status:")
    message(STATUS "    version: ${CAIRO_VERSION}")
    message(STATUS "    include path: ${CAIRO_INCLUDE_DIRS}")
    message(STATUS "    lib path: ${CAIRO_LIBDIR}")
    message(STATUS "    libraries: ${CAIRO_LIBRARIES}")
endif ()

# pango
if (UsePango)
    pkg_check_modules(PANGO pangocairo)
    if (NOT PANGO_FOUND)
        message(FATAL_ERROR "Pango not found!")
    else ()
        message(STATUS "Pango library status:")
        message(STATUS "    version: ${PANGO_VERSION}")
        message(STATUS "    include path: ${PANGO_INCLUDE_DIRS}")
        message(STATUS "    lib path: ${PANGO_LIBDIR}")
        message(STATUS "    libs path: ${PANGO_LIBRARY_DIRS}")
        message(STATUS "    libraries: ${PANGO_LIBRARIES}")
    endif ()
endif ()

# rsvg
if (UseRsvg)
    pkg_check_modules(RSVG librsvg-2.0)
    if (NOT RSVG_FOUND)
        message(FATAL_ERROR "rsvg not found!")
    else ()
        message(STATUS "rsvg library status:")
        message(STATUS "    version: ${RSVG_VERSION}")
        message(STATUS "    include path: ${RSVG_INCLUDE_DIRS}")
        message(STATUS "    lib path: ${RSVG_LIBDIR}")
        message(STATUS "    libs path: ${RSVG_LIBRARY_DIRS}")
        message(STATUS "    libraries: ${RSVG_LIBRARIES}")
    endif ()
endif ()

# libwebp
if (UseWebP)
    pkg_check_modules(WEBP libwebp)
    if (NOT WEBP_FOUND)
        message(FATAL_ERROR "libwebp not found!")
    else ()
        message(STATUS "WebP library status:")
        message(STATUS "    version: ${WEBP_VERSION}")
        message(STATUS "    include path: ${WEBP_INCLUDE_DIRS}")
        message(STATUS "    lib path: ${WEBP_LIBDIR}")
        message(STATUS "    libraries: ${WEBP_LIBRARIES}")
    endif ()
endif ()


# Jpeg
if (UseJpeg)
    pkg_check_modules(JPEG libturbojpeg)
    if (NOT JPEG_FOUND)
        pkg_check_modules(JPEG libjpeg)
    endif ()
    if (NOT JPEG_FOUND)
        message(FATAL_ERROR "JPEG not found!")
    else ()
        message(STATUS "Jpeg library status:")
        message(STATUS "    version: ${JPEG_VERSION}")
        message(STATUS "    include path: ${JPEG_INCLUDE_DIRS}")
        message(STATUS "    libraries: ${JPEG_LIBDIR}")
        message(STATUS "    libraries: ${JPEG_LIBRARIES}")
    endif ()
endif ()

# OpenCV
if (UseOpenCV)
    find_package(OpenCV REQUIRED)
    if (NOT OpenCV_FOUND)
        message(FATAL_ERROR "OpenCV not found!")
    else ()
        message(STATUS "OpenCV library status:")
        message(STATUS "    version: ${OpenCV_VERSION}")
        message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
        message(STATUS "    libraries: ${OpenCV_LIBS}")
        if (CMAKE_SYSTEM_NAME MATCHES "Emscripten")
            set(OpenCV_WITHOUT_IMAPI TRUE)
        endif ()
    endif ()
endif ()
