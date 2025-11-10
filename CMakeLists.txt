cmake_minimum_required(VERSION 3.10)
project(Gentlix_Bank_C C)

set(CMAKE_C_STANDARD 11)

# Set MSYS2 UCRT64 paths (if not set by toolchain)
if(WIN32 AND EXISTS "C:/msys64/ucrt64/bin/gcc.exe")
    set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe" CACHE FILEPATH "C compiler")
    set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe" CACHE FILEPATH "C++ compiler")
    # Add MSYS2 to PATH for pkg-config
    set(ENV{PATH} "C:/msys64/ucrt64/bin;C:/msys64/usr/bin;$ENV{PATH}")
    set(ENV{PKG_CONFIG_PATH} "C:/msys64/ucrt64/lib/pkgconfig")
endif()

# Include directories
include_directories(domain)
include_directories(gui)
include_directories(repository)
include_directories(services)

# Find GTK3 using pkg-config (with fallback)
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(GTK3 gtk+-3.0)
    if(GTK3_FOUND)
        add_definitions(${GTK3_CFLAGS_OTHER})
    else()
        # Fallback: try to find GTK3 manually
        message(WARNING "pkg-config not found or GTK3 not found via pkg-config. Trying manual detection...")
        find_path(GTK3_INCLUDE_DIRS
            NAMES gtk/gtk.h
            PATHS
            C:/msys64/ucrt64/include
            C:/msys64/mingw64/include
            /usr/include
        )
        find_library(GTK3_LIBRARIES
            NAMES gtk-3
            PATHS
            C:/msys64/ucrt64/lib
            C:/msys64/mingw64/lib
            /usr/lib
        )
        if(GTK3_INCLUDE_DIRS AND GTK3_LIBRARIES)
            message(STATUS "GTK3 found manually: ${GTK3_INCLUDE_DIRS}")
        else()
            message(FATAL_ERROR "GTK3 not found! Please install GTK3 via MSYS2: pacman -S mingw-w64-ucrt-x86_64-gtk3")
        endif()
    endif()
else()
    message(FATAL_ERROR "pkg-config not found! Please install: pacman -S mingw-w64-ucrt-x86_64-pkg-config")
endif()

# Add executable with all source files
add_executable(Gentlix_Bank_C
        domain/account.c
        domain/affiliate.c
        domain/date.c
        domain/domain.h
        domain/transaction.c
        domain/userAccount.c
        gui/gui.c
        gui/gui.h
        repository/repository.c
        repository/repository.h
        services/services.c
        services/services.h
        main.c)

# Link GTK3 libraries
target_link_libraries(Gentlix_Bank_C ${GTK3_LIBRARIES})
target_include_directories(Gentlix_Bank_C PRIVATE ${GTK3_INCLUDE_DIRS})

# Copy images folder to build directory (runs after each build)
add_custom_command(TARGET Gentlix_Bank_C POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/images
    ${CMAKE_BINARY_DIR}/images
    COMMENT "Copying images folder to build directory"
)
