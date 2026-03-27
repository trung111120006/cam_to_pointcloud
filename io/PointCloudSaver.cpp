#include "PointCloudSaver.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cstring>

using namespace std;

PointCloudSaver::PointCloudSaver(const string& output_dir, SaveFormat format)
    : m_output_dir(output_dir)
    , m_format(format)
{
    cout << "[Saver] Output dir: " << m_output_dir << endl;
}

bool PointCloudSaver::save(const PointCloud& cloud) {
    m_saved_count++;
    string filepath = buildFilePath(m_saved_count);
    if (m_format == SaveFormat::PLY) return savePLY(cloud, filepath);
    else                             return savePCD(cloud, filepath);
}

// --- private ---

string PointCloudSaver::buildFilePath(int index) const {
    ostringstream oss;
    oss << m_output_dir << "\\"
        << "pointcloud_"
        << setw(4) << setfill('0') << index
        << (m_format == SaveFormat::PLY ? ".ply" : ".pcd");
    return oss.str();
}

bool PointCloudSaver::savePLY(const PointCloud& cloud, const string& filepath) {
    ofstream file(filepath);
    if (!file.is_open()) {
        cerr << "[Saver] Cannot open: " << filepath << endl;
        return false;
    }

    file << "ply\n"
         << "format ascii 1.0\n"
         << "element vertex " << cloud.points.size() << "\n"
         << "property float x\n"
         << "property float y\n"
         << "property float z\n"
         << "property uchar red\n"
         << "property uchar green\n"
         << "property uchar blue\n"
         << "end_header\n";

    file << fixed << setprecision(6);
    for (const auto& p : cloud.points) {
        file << p.x << " " << p.y << " " << p.z << " "
             << static_cast<int>(p.r) << " "
             << static_cast<int>(p.g) << " "
             << static_cast<int>(p.b) << "\n";
    }

    file.close();
    cout << "[Saver] Saved: " << filepath
         << " (" << cloud.points.size() << " pts)" << endl;
    return true;
}

bool PointCloudSaver::savePCD(const PointCloud& cloud, const string& filepath) {
    ofstream file(filepath);
    if (!file.is_open()) {
        cerr << "[Saver] Cannot open: " << filepath << endl;
        return false;
    }

    file << "# .PCD v0.7\n"
         << "VERSION 0.7\n"
         << "FIELDS x y z rgb\n"
         << "SIZE 4 4 4 4\n"
         << "TYPE F F F F\n"
         << "COUNT 1 1 1 1\n"
         << "WIDTH "    << cloud.points.size() << "\n"
         << "HEIGHT 1\n"
         << "VIEWPOINT 0 0 0 1 0 0 0\n"
         << "POINTS "   << cloud.points.size() << "\n"
         << "DATA ascii\n";

    file << fixed << setprecision(6);
    for (const auto& p : cloud.points) {
        uint32_t rgb_packed = ((uint32_t)p.r << 16)
                            | ((uint32_t)p.g <<  8)
                            | ((uint32_t)p.b);
        float rgb_float;
        memcpy(&rgb_float, &rgb_packed, sizeof(float));

        file << p.x << " " << p.y << " " << p.z << " "
             << rgb_float << "\n";
    }

    file.close();
    cout << "[Saver] Saved: " << filepath
         << " (" << cloud.points.size() << " pts)" << endl;
    return true;
}