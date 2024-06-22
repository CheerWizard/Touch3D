#include <sf_camera.hpp>

namespace sf {

    float2 Camera::get_pan_speed() const {
        float x = std::min(static_cast<float>(frame_size.x) / 1000.0f, 2.4f);
        float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(static_cast<float>(frame_size.y) / 1000.0f, 2.4f);
        float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { x_factor * pan_acceleration, y_factor * pan_acceleration };
    }

    float Camera::get_zoom_speed() const {
        float zoom_speed = move_speed * zoom_acceleration;
        zoom_speed = std::max(zoom_speed, 0.0f);
        return zoom_speed * zoom_speed;
    }

    void Camera::pan(double2 pan) {
        float2 pan_speed = get_pan_speed();
        quat rotation = { -pitch, -yaw, roll, 0 };
        float3 right = rotate(rotation, { 1.0f, 0.0f, 0.0f }).xyz();
        float3 up = rotate(rotation, { 0.0f, 1.0f, 0.0f }).xyz();
        m_focal_point = m_focal_point + -right * static_cast<float>(pan.x) * pan_speed.x * move_speed;
        m_focal_point = m_focal_point + up * static_cast<float>(pan.y) * pan_speed.y * move_speed;
        update_view(m_focal_point);
    }

    void Camera::move_forward() {
        position = position - front * move_speed;
        update_view(position);
    }

    void Camera::move_backward() {
        position = position + front * move_speed;
        update_view(position);
    }

    void Camera::move_left() {
        position = position - cross(front, up) * move_speed;
        update_view(position);
    }

    void Camera::move_right() {
        position = position + cross(front, up) * move_speed;
        update_view(position);
    }

    void Camera::zoom_in() {
        fov -= get_zoom_speed();
        fov = clamp(min_fov, max_fov, fov);
        update_perspective();
    }

    void Camera::zoom_out() {
        fov += get_zoom_speed();
        fov = clamp(min_fov, max_fov, fov);
        update_perspective();
    }

    void Camera::on_scroll_changed(double2 scroll) {
        fov -= static_cast<float>(scroll.y) * get_zoom_speed();
        fov = clamp(min_fov, max_fov, fov);
        update_perspective();
    }

    void Camera::look(double2 look, SF_CAMERA_MODE camera_mode) {
        quat rotation = { -pitch, -yaw, roll, 0 };
        float3 up = rotate(rotation, { 0.0f, 1.0f, 0.0f }).xyz();
        float look_sign = static_cast<float>(camera_mode);
        float yaw_sign = up.y < 0 ? -1.0f : 1.0f;
        yaw += static_cast<float>(look.x) * yaw_sign * horizontal_sens * look_sign;
        pitch += static_cast<float>(look.y) * vertical_sens * look_sign;
        update_view(position);
    }

    void Camera::on_window_changed(int2 frame_size) {
        this->frame_size = frame_size;
        on_window_ratio_changed(static_cast<float>(frame_size.x) / static_cast<float>(frame_size.y));
    }

    void Camera::on_window_ratio_changed(float ratio) {
        aspect_ratio = ratio;
        update_perspective();
    }

    void Camera::update_perspective() {
        perspective = mat4_perspective(aspect_ratio, degree(fov), z_near, z_far);
    }

    void Camera::update_ortho() {
        ortho = mat4_ortho(left, right, bottom, top, z_near, z_far);
    }

    void Camera::update_view(const float3& pos) {
        // yaw = pitch = 0.0f; // lock the camera's rotation
        quat rotation = quat(-pitch, -yaw, roll, 1);
        front = rotate(rotation, front).xyz();
        up = rotate(rotation, up).xyz();
        view = mat4_view(pos, front, up);
        m_focal_point = pos;
        this->position = pos;
    }

}