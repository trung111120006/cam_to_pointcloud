#pragma once
#include <librealsense2/rs.hpp>
#include "PointCloud.hpp"
#include "../camera/RealSenseCamera.hpp"  // dùng FramePair

struct ConvertConfig {
    float min_depth = 0.1f;
    float max_depth = 10.0f;
};

class PointCloudConverter {
public:
    explicit PointCloudConverter(const rs2_intrinsics& intrinsics,
                                 const ConvertConfig&  config = ConvertConfig{});

    // Nhận FramePair (depth + color đã align), trả về PointCloud có RGB
    PointCloud convert(const FramePair& frames, int frame_index = 0);

private:
    rs2_intrinsics m_intrinsics;
    ConvertConfig  m_config;
};