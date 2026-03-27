#pragma once
#include "../pointcloud/PointCloud.hpp"
#include <string>

using namespace std;

enum class SaveFormat { PLY, PCD };

class PointCloudSaver {
public:
    explicit PointCloudSaver(
        const string& output_dir = "D:\\FILlab_code\\cam_d415\\output",
        SaveFormat format = SaveFormat::PLY
    );

    bool save(const PointCloud& cloud);
    int  getSavedCount() const { return m_saved_count; }

private:
    bool   savePLY(const PointCloud& cloud, const string& filepath);
    bool   savePCD(const PointCloud& cloud, const string& filepath);
    string buildFilePath(int index) const;

    string     m_output_dir;
    SaveFormat m_format;
    int        m_saved_count = 0;
};