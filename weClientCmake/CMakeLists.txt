# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("weClientCmake")

# 包含子项目。
# add_subdirectory ("lib")

# 将源代码添加到此项目的可执行文件。
add_executable (${PROJECT_NAME} "./src/weClientCmake.cpp" "src/FirstFLTK.cpp")

# 寻找链接库
find_library(WS2_LIBRARY ws2_32)

# 配置工程内部链接库
set(FLTKD_LIBRARY "${PROJECT_SOURCE_DIR}/lib/fltkd.lib")
set(FLTKIMAGED_LIBRARY "${PROJECT_SOURCE_DIR}/lib/fltkimagesd.lib")
set(FLTKGLD_LIBRARY "${PROJECT_SOURCE_DIR}/lib/fltkgld.lib")
set(FLTKFORMD_LIBRARY "${PROJECT_SOURCE_DIR}/lib/fltkformsd.lib")
set(FLTKJPEGD_LIBRARY "${PROJECT_SOURCE_DIR}/lib/fltkjpegd.lib")
set(FLTKPNGD_LIBRARY "${PROJECT_SOURCE_DIR}/lib/fltkpngd.lib")
set(FLTKZLIBD_LIBRARY "${PROJECT_SOURCE_DIR}/lib/fltkzlibd.lib")

# 配置链接库
target_link_libraries( ${PROJECT_NAME}  
	PRIVATE 
		${WS2_LIBRARY} 
		${FLTKD_LIBRARY}
		${FLTKIMAGED_LIBRARY}
		${FLTKGLD_LIBRARY}
		${FLTKFORMD_LIBRARY}
		${FLTKJPEGD_LIBRARY}
		${FLTKPNGD_LIBRARY}
		${FLTKZLIBD_LIBRARY}
)
target_include_directories( ${PROJECT_NAME}  
	PRIVATE 
		${PROJECT_SOURCE_DIR}/include
)
