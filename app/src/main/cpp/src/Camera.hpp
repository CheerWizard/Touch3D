#pragma once

#include <Math.hpp>

enum T3D_CAMERA_MODE
{
    T3D_CAMERA_MODE_GAME = -1,
    T3D_CAMERA_MODE_EDITOR = 1,
};

class Camera final {

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

    float2 GetPanSpeed() const;
    float GetZoomSpeed() const;

    void ZoomOut();
    void ZoomIn();
    void MoveForward();
    void MoveBackward();
    void MoveLeft();
    void MoveRight();
    void Pan(double2 pan);
    void Look(double2 look, T3D_CAMERA_MODE camera_mode);

    void OnScrollChanged(double2 scroll);
    void OnWindowFrameResized(int2 frame_size);
    void OnWindowRatioChanged(float ratio);

private:
    void UpdatePerspective();
    void UpdateOrtho();
    void UpdateView(const float3& pos);

    float3 m_focal_point;
};