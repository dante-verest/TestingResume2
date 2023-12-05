# CMakeList.txt: проект CMake для TestTask; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TestTask")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (TestTask "TestTask.cpp" "TestTask.h" "FileParser.h" "FileParser.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestTask PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

include_directories("${CMAKE_SOURCE_DIR}/libs/libiconv-1.17/include")
find_library(LIBICONV libiconv "${CMAKE_SOURCE_DIR}/libs/libiconv-1.17/lib/.libs")
target_link_libraries(TestTask &{LIBICONV})

if(!MSVC)
add_compile_options(-std=gnu++20)
add_compile_options(-Wall -Wextra -W -ansi -pedantic -g -ggdb -c -o)
endif()
