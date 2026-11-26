cmake_minimum_required(VERSION 3.18)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)

include_directories(core/include)
aux_source_directory(core/src SRC)

# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake") # 根据你的 emsdk 路径调整
add_executable(app core/main.cpp ${SRC})

# # Emscripten 编译选项：生成 WASM、导出指定函数、生成 JS 胶水文件
# target_link_options(core PRIVATE
# -s WASM=1
# -s EXPORTED_FUNCTIONS=['_malloc', '_free', '_your_function_name'] # 替换为你的 C++ 函数
# -s EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']
# --output ${CMAKE_CURRENT_BINARY_DIR}/wasm/core.js
# )

# # 编译后自动将 .wasm 和 .js 文件复制到前端目录
# add_custom_command(TARGET core POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_directory
# ${CMAKE_CURRENT_BINARY_DIR}/wasm
# ${CMAKE_CURRENT_SOURCE_DIR}/web/scripts/wasm
# COMMENT "Copying WASM artifacts to web directory"
# )