#include <Camera.hpp>

float2 Camera::GetPanSpeed() const {
    float x = min(static_cast<float>(frame_size.x) / 1000.0f, 2.4f);
    float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = min(static_cast<float>(frame_size.y) / 1000.0f, 2.4f);
    float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { x_factor * pan_acceleration, y_factor * pan_acceleration };
}

float Camera::GetZoomSpeed() const {
    float zoom_speed = move_speed * zoom_acceleration;
    zoom_speed = max(zoom_speed, 0.0f);
    return zoom_speed * zoom_speed;
}

void Camera::Pan(double2 pan) {
    float2 pan_speed = GetPanSpeed();
    quatf rotation = { -pitch, -yaw, roll, 0 };
    float3 right = rotate({ 1, 0, 0 }, rotation).xyz();
    float3 up = rotate({ 0, 1, 0 }, rotation).xyz();
    m_focal_point = m_focal_point + -right * static_cast<float>(pan.x) * pan_speed.x * move_speed;
    m_focal_point = m_focal_point + up * static_cast<float>(pan.y) * pan_speed.y * move_speed;
    UpdateView(m_focal_point);
}

void Camera::MoveForward() {
    position = position - front * move_speed;
    UpdateView(position);
}

void Camera::MoveBackward() {
    position = position + front * move_speed;
    UpdateView(position);
}

void Camera::MoveLeft() {
    position = position - cross(front, up) * move_speed;
    UpdateView(position);
}

void Camera::MoveRight() {
    position = position + cross(front, up) * move_speed;
    UpdateView(position);
}

void Camera::ZoomIn() {
    fov -= GetZoomSpeed();
    fov = clamp(min_fov, max_fov, fov);
    UpdatePerspective();
}

void Camera::ZoomOut() {
    fov += GetZoomSpeed();
    clamp(min_fov, max_fov, fov);
    UpdatePerspective();
}

void Camera::OnScrollChanged(double2 scroll) {
    fov -= static_cast<float>(scroll.y) * GetZoomSpeed();
    clamp(min_fov, max_fov, fov);
    UpdatePerspective();
}

void Camera::Look(double2 look, T3D_CAMERA_MODE camera_mode) {
    quatf rotation = { -pitch, -yaw, roll, 0 };
    float3 up = rotate({ 0, 1, 0 }, rotation).xyz();
    float look_sign = static_cast<float>(camera_mode);
    float yaw_sign = up.y < 0 ? -1.0f : 1.0f;
    yaw += static_cast<float>(look.x) * yaw_sign * horizontal_sens * look_sign;
    pitch += static_cast<float>(look.y) * vertical_sens * look_sign;
    UpdateView(position);
}

void Camera::OnWindowFrameResized(int2 frame_size) {
    this->frame_size = frame_size;
    OnWindowRatioChanged(static_cast<float>(frame_size.x) / static_cast<float>(frame_size.y));
}

void Camera::OnWindowRatioChanged(float ratio) {
    aspect_ratio = ratio;
    UpdatePerspective();
}

void Camera::UpdatePerspective() {
    perspective = mat4_perspective(aspect_ratio, fov, z_near, z_far);
}

void Camera::UpdateOrtho() {
    ortho = mat4_ortho(left, right, bottom, top, z_near, z_far);
}

void Camera::UpdateView(const float3& position) {
    // yaw = pitch = 0.0f; // Lock the camera's rotation
    quatf rotation = quatf(-pitch, -yaw, roll, 1);
    front = rotate(front, rotation).xyz();
    up = rotate(up, rotation).xyz();
    view = mat4_view(position, front, up);
    m_focal_point = position;
    this->position = position;
}