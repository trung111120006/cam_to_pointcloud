cmake_minimum_required(VERSION 3.10)
project(cam_d415)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Đường dẫn SDK
set(REALSENSE_SDK "C:/Users/Admin/OneDrive/Documents/RealSense SDK 2.0")

# Include + lib
include_directories("${REALSENSE_SDK}/include")
link_directories("${REALSENSE_SDK}/lib/x64")

add_executable(cam_d415
    main.cpp
    camera/RealSenseCamera.cpp
    pointcloud/PointCloud.cpp
    pointcloud/PointCloudConverter.cpp
    io/PointCloudSaver.cpp
)

target_link_libraries(cam_d415
    realsense2
)

# (Khuyến nghị) Tự copy realsense2.dll vào cùng thư mục exe sau khi build
add_custom_command(TARGET cam_d415 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${REALSENSE_SDK}/bin/x64/realsense2.dll"
        $<TARGET_FILE_DIR:cam_d415>
)