#pragma once
#include <vector>
#include <string>
#include <cstdint> 

using namespace std;
struct Point3D {
    float x, y, z;
    uint8_t r, g, b;   // Màu RGB từ color camera
};

struct PointCloud {
    vector<Point3D> points;
    int    frame_index = 0; // Số thứ tự frame (từ camera)
    string timestamp;
};