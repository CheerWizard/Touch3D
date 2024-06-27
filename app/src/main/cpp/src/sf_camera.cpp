#include <sf_camera.hpp>

namespace sf {

    camera_t camera_init(int2_t frame_size) {
        camera_t camera;
        camera.frame_size = frame_size;
        return camera;
    }

    float2_t camera_get_pan_speed(const camera_t& camera) {
        float x = std::min(static_cast<float>(camera.frame_size.x) / 1000.0f, 2.4f);
        float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(static_cast<float>(camera.frame_size.y) / 1000.0f, 2.4f);
        float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { x_factor * camera.pan_acceleration, y_factor * camera.pan_acceleration };
    }

    float camera_get_zoom_speed(const camera_t& camera) {
        float zoom_speed = camera.move_speed * camera.zoom_acceleration;
        zoom_speed = std::max(zoom_speed, 0.0f);
        return zoom_speed * zoom_speed;
    }

    void camera_pan(camera_t& camera, double2_t pan) {
        float2_t pan_speed = camera_get_pan_speed(camera);
        quat_t rotation = { -camera.pitch, -camera.yaw, camera.roll, 0 };
        float3_t right = rotate(rotation, { 1.0f, 0.0f, 0.0f }).xyz();
        float3_t up = rotate(rotation, { 0.0f, 1.0f, 0.0f }).xyz();
        camera.focal_point = camera.focal_point + -right * static_cast<float>(pan.x) * pan_speed.x * camera.move_speed;
        camera.focal_point = camera.focal_point + up * static_cast<float>(pan.y) * pan_speed.y * camera.move_speed;
        camera_update_view(camera, camera.focal_point);
    }

    void camera_move_forward(camera_t& camera) {
        camera.position = camera.position - camera.front * camera.move_speed;
        camera_update_view(camera, camera.position);
    }

    void camera_move_backward(camera_t& camera) {
        camera.position = camera.position + camera.front * camera.move_speed;
        camera_update_view(camera, camera.position);
    }

    void camera_move_left(camera_t& camera) {
        camera.position = camera.position - cross(camera.front, camera.up) * camera.move_speed;
        camera_update_view(camera, camera.position);
    }

    void camera_move_right(camera_t& camera) {
        camera.position = camera.position + cross(camera.front, camera.up) * camera.move_speed;
        camera_update_view(camera, camera.position);
    }

    void camera_zoom_in(camera_t& camera) {
        camera.fov -= camera_get_zoom_speed(camera);
        camera.fov = clamp(camera.min_fov, camera.max_fov, camera.fov);
        camera_update_perspective(camera);
    }

    void camera_zoom_out(camera_t& camera) {
        camera.fov += camera_get_zoom_speed(camera);
        camera.fov = clamp(camera.min_fov, camera.max_fov, camera.fov);
        camera_update_perspective(camera);
    }

    void camera_on_scroll_changed(camera_t& camera, double2_t scroll) {
        camera.fov -= static_cast<float>(scroll.y) * camera_get_zoom_speed(camera);
        camera.fov = clamp(camera.min_fov, camera.max_fov, camera.fov);
        camera_update_perspective(camera);
    }

    void camera_look(camera_t& camera, double2_t look, SF_CAMERA_MODE camera_mode) {
        quat_t rotation = { -camera.pitch, -camera.yaw, camera.roll, 0 };
        float3_t up = rotate(rotation, { 0.0f, 1.0f, 0.0f }).xyz();
        float look_sign = static_cast<float>(camera_mode);
        float yaw_sign = up.y < 0 ? -1.0f : 1.0f;
        camera.yaw += static_cast<float>(look.x) * yaw_sign * camera.horizontal_sens * look_sign;
        camera.pitch += static_cast<float>(look.y) * camera.vertical_sens * look_sign;
        camera_update_view(camera, camera.position);
    }

    void camera_on_window_frame_resized(camera_t& camera, int2_t frame_size) {
        camera.frame_size = frame_size;
        camera_on_window_ratio_changed(camera, static_cast<float>(frame_size.x) / static_cast<float>(frame_size.y));
    }

    void camera_on_window_ratio_changed(camera_t& camera, float ratio) {
        camera.aspect_ratio = ratio;
        camera_update_perspective(camera);
    }

    void camera_update_perspective(camera_t& camera) {
        camera.perspective = mat4_perspective(camera.aspect_ratio, degree_t(camera.fov), camera.z_near, camera.z_far);
    }

    void camera_update_ortho(camera_t& camera) {
        camera.ortho = mat4_ortho(camera.left, camera.right, camera.bottom, camera.top, camera.z_near, camera.z_far);
    }

    void camera_update_view(camera_t& camera, const float3_t& pos) {
        // yaw = pitch = 0.0f; // lock the camera's rotation
        quat_t rotation = quat_t(-camera.pitch, -camera.yaw, camera.roll, 1);
        camera.front = rotate(rotation, camera.front).xyz();
        camera.up = rotate(rotation, camera.up).xyz();
        camera.view = mat4_view(pos, camera.front, camera.up);
        camera.focal_point = pos;
        camera.position = pos;
    }

}