# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abelov <abelov@student.42london.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/11 20:06:17 by abelov            #+#    #+#              #
#    Updated: 2025/01/11 20:06:18 by abelov           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cmake_minimum_required(VERSION 3.8)
#set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER gcc)

project(ft_printf C)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 99)

include_directories(include)

set(FT_CFLAGS
        -Wall -Wextra -Werror
        -Wwrite-strings -Wimplicit
        -Wno-error=unused-parameter
        -Wno-error=unused-function
        -Wno-error=unused-variable
        -O0         -g3 -gdwarf-3
        -masm=att -MMD
        -fasynchronous-unwind-tables
        -fdata-sections
        -fsanitize=address
        -fsanitize=undefined
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow
        -fno-sanitize-recover=all)

set(COMMON_LDFLAGS
        -fsanitize=address
        -fsanitize=undefined
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow
        -fno-sanitize-recover=all)

#==============================================================================#
# FT lib
#==============================================================================#

set(FT_LIB_NAME ft)
set(FT_LIB_FILENAME libft.a)
set(FT_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/libft")

file(GLOB_RECURSE FT_LIB_HEADERS "${FT_LIB_DIR}/*.h")
file(GLOB_RECURSE FT_LIB_SOURCES "${FT_LIB_DIR}/ft_*.c")

add_custom_target(${FT_LIB_NAME}_target ALL
        COMMAND make -C ${FT_LIB_DIR}# BUILD_WITH_ASAN=1
        BYPRODUCTS "${FT_LIB_DIR}/${FT_LIB_FILENAME}"
        SOURCES "${FT_LIB_HEADERS};${FT_LIB_SOURCES}"
        COMMENT "Building FT_LIB")

add_library(${FT_LIB_NAME} STATIC IMPORTED)

add_dependencies(${FT_LIB_NAME} "${FT_LIB_NAME}_target")
set_target_properties(${FT_LIB_NAME} PROPERTIES
        IMPORTED_LOCATION ${FT_LIB_DIR}/${FT_LIB_FILENAME}
        INTERFACE_COMPILE_DEFINITIONS ""
        INTERFACE_LINK_OPTIONS ""
        INTERFACE_SOURCES "${FT_LIB_HEADERS}"
        INTERFACE_INCLUDE_DIRECTORIES "${FT_LIB_DIR};${FT_LIB_DIR}/include;${FT_LIB_DIR}/include/ft")

#==============================================================================#


#==============================================================================#
# FT printf lib
#==============================================================================#
set(FTPRINTF_TARGET_NAME libprintf.a)

file(GLOB FTPRINTF_HEADERS "src/ft_*.h")
file(GLOB FTPRINTF_SOURCES "src/ft_*.c")

#Generate the library from the sources
add_library(${FTPRINTF_TARGET_NAME} STATIC ${FTPRINTF_HEADERS} ${FTPRINTF_SOURCES})
target_include_directories(${FTPRINTF_TARGET_NAME} PRIVATE include)
target_compile_options(${FTPRINTF_TARGET_NAME} PRIVATE ${FT_CFLAGS})
target_link_libraries(${FTPRINTF_TARGET_NAME} PRIVATE ${FT_LIB_NAME})
set_target_properties(${FTPRINTF_TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_NAME "${PROJECT_NAME}")

#==============================================================================#

set(TARGET_NAME main)

file(GLOB TARGET_HEADERS "tests/*.h")
file(GLOB TARGET_SOURCES "tests/*.c")

add_executable(${TARGET_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES})
target_compile_options(${TARGET_NAME} PRIVATE ${TARGET_CFLAGS})
target_include_directories(${TARGET_NAME} PRIVATE include src)
target_link_options(${TARGET_NAME} PRIVATE ${COMMON_LDFLAGS})
target_link_libraries(${TARGET_NAME} ${FTPRINTF_TARGET_NAME})
set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/tests"
        OUTPUT_NAME "ft_printf_test")

#==============================================================================#
# Custom targets
#==============================================================================#
add_custom_target(fclean
        COMMAND make -C ${FT_LIB_DIR} fclean)

add_custom_target(re
        COMMAND make -C ${FT_LIB_DIR} re
        COMMAND make -C ${PROJECT_SOURCE_DIR} re)

add_custom_target(norm
        COMMAND make -C ${FT_LIB_DIR} norm
        COMMAND make -C ${PROJECT_SOURCE_DIR} norm)
