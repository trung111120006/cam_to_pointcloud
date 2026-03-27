#include "PointCloudConverter.hpp"
#include <iostream>

using namespace std;

PointCloudConverter::PointCloudConverter(const rs2_intrinsics& intrinsics,
                                         const ConvertConfig&  config)
    : m_intrinsics(intrinsics)
    , m_config(config)
{}

PointCloud PointCloudConverter::convert(const FramePair& frames, int frame_index) {
    PointCloud cloud;
    cloud.frame_index = frame_index;
    cloud.points.reserve(m_intrinsics.width * m_intrinsics.height);

    const rs2::depth_frame& depth = frames.depth;
    const rs2::video_frame& color = frames.color;

    // Con trỏ thô tới data RGB (mỗi pixel = 3 byte: R, G, B)
    const uint8_t* color_data = reinterpret_cast<const uint8_t*>(color.get_data());
    const int color_width     = color.get_width();

    for (int v = 0; v < m_intrinsics.height; v++) {
        for (int u = 0; u < m_intrinsics.width; u++) {

            float Z = depth.get_distance(u, v);

            if (Z < m_config.min_depth || Z > m_config.max_depth)
                continue;

            // ── Deproject pixel (u,v) → 3D point ──────────────────
            // Công thức Pinhole Camera Model:
            //   X = (u - cx) / fx * Z
            //   Y = (v - cy) / fy * Z
            // ──────────────────────────────────────────────────────
            Point3D p;
            p.x = (u - m_intrinsics.ppx) / m_intrinsics.fx * Z;
            p.y = (v - m_intrinsics.ppy) / m_intrinsics.fy * Z;
            p.z = Z;

            // ── Lấy màu RGB tại pixel (u,v) ──────────────────────
            // Vì color đã được align vào depth space nên cùng (u,v)
            // trỏ đúng màu của điểm 3D đó
            // Layout bộ nhớ: [R0,G0,B0, R1,G1,B1, ...]
            //   index = (v * width + u) * 3
            // ──────────────────────────────────────────────────────
            int idx = (v * color_width + u) * 3;
            p.r = color_data[idx + 0];  // Red
            p.g = color_data[idx + 1];  // Green
            p.b = color_data[idx + 2];  // Blue

            cloud.points.push_back(p);
        }
    }

    cout << "[Converter] Frame #" << frame_index
         << " -> " << cloud.points.size() << " points (XYZRGB)" << endl;

    return cloud;
}