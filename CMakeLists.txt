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
# подключение библиотеки libiconv.
# в отличие от Eigen, её нужно собирать в бинарник, а не ограничиваться просто исходниками
set(LIB_PATH "${CMAKE_SOURCE_DIR}/libs/libiconv-1.17")
include_directories("${LIB_PATH}/include")
if(UNIX)
	find_library(LIBICONV NAMES libiconv.so.2.6.1 "${LIB_PATH}/lib/.libs")
elseif(WIN32)
	find_library(LIBICONV NAMES libiconv.dll "${LIB_PATH}/lib/.libs")
endif()
add_executable (TestTask "TestTask.cpp" "TestTask.h" "FileParser.h" "FileParser.cpp")
#message(${LIBICONV})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TestTask PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(TestTask ${LIBICONV})
