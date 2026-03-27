#include "RealSenseCamera.hpp"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

using namespace std;

RealSenseCamera::RealSenseCamera(const CameraConfig& config)
    : m_config(config)
    , m_align(RS2_STREAM_DEPTH)  // Align color vào depth space
{
    // Bật depth stream
    m_rs_config.enable_stream(
        RS2_STREAM_DEPTH,
        m_config.width, m_config.height,
        RS2_FORMAT_Z16, m_config.fps
    );

    // Bật color stream — cùng resolution cho dễ map pixel
    m_rs_config.enable_stream(
        RS2_STREAM_COLOR,
        m_config.width, m_config.height,
        RS2_FORMAT_RGB8, m_config.fps
    );
}

RealSenseCamera::~RealSenseCamera() {
    if (m_running) stop();
}

void RealSenseCamera::start() {
    try {
        auto profile = m_pipeline.start(m_rs_config);

        m_depth_intrinsics = profile
            .get_stream(RS2_STREAM_DEPTH)
            .as<rs2::video_stream_profile>()
            .get_intrinsics();

        m_running = true;

        cout << "[Camera] Started "
             << m_config.width << "x" << m_config.height
             << " @ " << m_config.fps << "fps (Depth + Color)" << endl;
        cout << "[Camera] fx=" << m_depth_intrinsics.fx
             << " fy=" << m_depth_intrinsics.fy
             << " cx=" << m_depth_intrinsics.ppx
             << " cy=" << m_depth_intrinsics.ppy << endl;

        warmUp();
    }
    catch (const rs2::error& e) {
        throw runtime_error(string("[Camera] RealSense error: ") + e.what());
    }
}

void RealSenseCamera::stop() {
    if (m_running) {
        m_pipeline.stop();
        m_running = false;
        cout << "[Camera] Stopped." << endl;
    }
}

bool RealSenseCamera::isRunning() const {
    return m_running;
}

FramePair RealSenseCamera::getFrames() {
    rs2::frameset raw;
    if (!m_pipeline.poll_for_frames(&raw)) {
        // Chưa có frame — trả về empty
        return FramePair{ rs2::depth_frame(rs2::frame{}),
                          rs2::video_frame(rs2::frame{}) };
    }

    // Align color frame khớp với depth frame (cùng viewport)
    rs2::frameset aligned = m_align.process(raw);

    return FramePair{
        aligned.get_depth_frame(),
        aligned.get_color_frame()
    };
}

rs2_intrinsics RealSenseCamera::getDepthIntrinsics() const {
    return m_depth_intrinsics;
}

// --- private ---

void RealSenseCamera::warmUp() {
    cout << "[Camera] Warming up (" << m_config.warmup_frames << " frames)..." << endl;

    int success  = 0;
    int attempts = 0;
    const int max_attempts = m_config.warmup_frames * 3;

    while (success < m_config.warmup_frames && attempts < max_attempts) {
        attempts++;
        try {
            rs2::frameset fs = m_pipeline.wait_for_frames(m_config.timeout_ms);
            rs2::frameset aligned = m_align.process(fs);
            if (aligned.get_depth_frame() && aligned.get_color_frame()) {
                success++;
            }
        }
        catch (const rs2::error& e) {
            cerr << "[Camera] Warm-up drop (" << attempts << "): "
                 << e.what() << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    if (success < m_config.warmup_frames)
        cerr << "[Camera] Warning: Only " << success << "/" 
             << m_config.warmup_frames << " warm-up frames OK." << endl;
    else
        cout << "[Camera] Ready!" << endl;
}