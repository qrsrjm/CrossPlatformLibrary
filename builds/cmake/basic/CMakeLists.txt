# 最低版本声明
cmake_minimum_required(VERSION 2.8)

# 库文件编译输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../bin)

# 头文件搜索路径
#include_directories(${PROJECT_SOURCE_DIR}/../../include )

# 依赖的库文件搜索路径
#link_directories("${PROJECT_SOURCE_DIR}/../../bin")

# basic为库文件的名称，后面跟源代码文件，以空格分隔
add_library(basic SHARED ${PROJECT_SOURCE_DIR}/../../src/basic/err.cpp)

# 可以重新设置输出名
#set_target_properties(MathFunctions PROPERTIES OUTPUT_NAME "xxx")
