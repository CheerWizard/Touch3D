#pragma once

#include <sf_math.hpp>

enum SF_CAMERA_MODE
{
    SF_CAMERA_MODE_GAME = -1,
    SF_CAMERA_MODE_EDITOR = 1,
};

namespace sf {

    struct SF_API camera_t final {
        // Position.z = -1 is a default valid value for 2D orthographic view
        // If Position.z >= 0, 2D geometry will not be shown on screen
        float3_t position = { 0, 0, -1 };
        float3_t front = { 0, 0, 0 };
        float3_t up = { 0, 1, 0 };
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

        float4x4_t perspective;
        float4x4_t ortho;
        float4x4_t view;

        float move_speed = 0.1f;
        float zoom_acceleration = 10.0f;
        float pan_acceleration = 10.0f;
        float horizontal_sens = 0.01f;
        float vertical_sens = 0.01f;
        float min_fov = 1.0f;
        float max_fov = 45.0f;

        int2_t frame_size;

        float3_t focal_point;
    };

    SF_API camera_t camera_init(int2_t frame_size);
    SF_API float2_t camera_get_pan_speed(const camera_t& camera);
    SF_API float camera_get_zoom_speed(const camera_t& camera);
    SF_API void camera_zoom_out(camera_t& camera);
    SF_API void camera_zoom_in(camera_t& camera);
    SF_API void camera_move_forward(camera_t& camera);
    SF_API void camera_move_backward(camera_t& camera);
    SF_API void camera_move_left(camera_t& camera);
    SF_API void camera_move_right(camera_t& camera);
    SF_API void camera_pan(camera_t& camera, double2_t pan);
    SF_API void camera_look(camera_t& camera, double2_t look, SF_CAMERA_MODE camera_mode);
    SF_API void camera_on_scroll_changed(camera_t& camera, double2_t scroll);
    SF_API void camera_on_window_frame_resized(camera_t& camera, int2_t frame_size);
    SF_API void camera_on_window_ratio_changed(camera_t& camera, float ratio);
    SF_API void camera_update_perspective(camera_t& camera);
    SF_API void camera_update_ortho(camera_t& camera);
    SF_API void camera_update_view(camera_t& camera, const float3_t& pos);

}
