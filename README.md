# can_to_pointcloud

## Description

This project captures point cloud data directly from a camera using the Intel RealSense SDK. It is implemented in C++ and uses CMake as the build system. The application connects to a RealSense camera, retrieves depth data, and converts it into a point cloud output file.

## Requirements

* C++ compiler (supporting C++11 or later)
* CMake (version 3.10 or higher)
* Intel RealSense SDK (librealsense)

## Important Note

Before building the project, make sure the RealSense SDK is installed on your system and its path can be found by CMake. If the SDK is not automatically detected, you must manually specify the path to `librealsense`.

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

If CMake cannot find the RealSense SDK, specify it manually:

```bash
cmake .. -Drealsense2_DIR=/path/to/librealsense
```

## Output

After building and running the program, the application will generate a point cloud file (for example: `.ply`) from the camera data. Ensure that:

* The output file path is correctly created
* The application has permission to write to the directory

## Usage

1. Connect your Intel RealSense camera
2. Run the executable:

```bash
./can_to_pointcloud
```

## Notes

* Ensure the camera is properly connected before running
* Verify RealSense SDK installation if build fails
* Output file format can be modified in the source code if needed
* Works on Linux and Windows with proper SDK setup

## License

This project is open-source and can be used for learning and development purposes.
