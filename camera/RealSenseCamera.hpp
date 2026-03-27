#pragma once
#include <librealsense2/rs.hpp>

struct CameraConfig {
    int width         = 848;
    int height        = 480;
    int fps           = 60;
    int warmup_frames = 30;
    unsigned int timeout_ms = 5000;
};

// Gom cả depth lẫn color vào 1 struct để trả về cùng lúc
struct FramePair {
    rs2::depth_frame depth;
    rs2::video_frame color;

    // Kiểm tra cả 2 frame đều hợp lệ
    bool valid() const { return depth && color; }
};

class RealSenseCamera {
public:
    explicit RealSenseCamera(const CameraConfig& config);
    ~RealSenseCamera();

    void           start();
    void           stop();
    bool           isRunning() const;

    // Trả về cặp (depth + color) đã được align với nhau
    FramePair      getFrames();

    rs2_intrinsics getDepthIntrinsics() const;

private:
    void warmUp();

    CameraConfig    m_config;
    rs2::pipeline   m_pipeline;
    rs2::config     m_rs_config;
    rs2::align      m_align;        // Align color → depth space
    rs2_intrinsics  m_depth_intrinsics;
    bool            m_running = false;
};