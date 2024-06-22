#pragma once

#include <sf_math.hpp>

enum SF_CAMERA_MODE
{
    SF_CAMERA_MODE_GAME = -1,
    SF_CAMERA_MODE_EDITOR = 1,
};

namespace sf {

    class SF_API Camera final {

    public:
        // Position.z = -1 is a default valid value for 2D orthographic view
        // If Position.z >= 0, 2D geometry will not be shown on screen
        float3 position = { 0, 0, -1 };
        float3 front = { 0, 0, 0 };
        float3 up = { 0, 1, 0 };
        float pitch = 0;
        float yaw = 0;
        float roll = 0;

        float top;
        float bottom;
        float left;
        float right;
        float fov;
        float aspect_ratio;
        float z_far;
        float z_near;

        float4x4 perspective;
        float4x4 ortho;
        float4x4 view;

        float move_speed = 0.1f;
        float zoom_acceleration = 10.0f;
        float pan_acceleration = 10.0f;
        float horizontal_sens = 0.01f;
        float vertical_sens = 0.01f;
        float min_fov = 1.0f;
        float max_fov = 45.0f;

        int2 frame_size;

        Camera(int2 frame_size) : frame_size(frame_size) {}

        float2 get_pan_speed() const;
        float get_zoom_speed() const;

        void zoom_out();
        void zoom_in();
        void move_forward();
        void move_backward();
        void move_left();
        void move_right();
        void pan(double2 pan);
        void look(double2 look, SF_CAMERA_MODE camera_mode);

        void on_scroll_changed(double2 scroll);
        void on_window_changed(int2 frame_size);
        void on_window_ratio_changed(float ratio);

    private:
        void update_perspective();
        void update_ortho();
        void update_view(const float3& pos);

        float3 m_focal_point;
    };

}