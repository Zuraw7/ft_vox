#ifndef FT_VOX_CAMERA_HPP
#define FT_VOX_CAMERA_HPP

#include <iostream>
#include <array>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../utils/utils.hpp"

#define NEAR 0.1f
#define FAR 100.0f
#define SENSITIVITY 0.1f

enum CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera() = delete;
    Camera(const glm::vec3 position, const glm::vec3 target, const glm::vec3 worldUp);
    Camera(Camera &) = delete;
    Camera(Camera &&) = delete;
    ~Camera() = default;

    Camera &operator=(const Camera&) = delete;
    Camera &operator=(const Camera&&) = delete;

    const glm::mat4 &getCamView();
    const glm::mat4 &getCamProjection() const;
    const glm::vec3 &getPosition() const;

    void updateCameraDirection(double dx, double dy);
    void updateCameraPos(CameraDirection dir, double deltaTime);
    void updateCameraZoom(double dy);
    void updateCameraVectors();

private:
    // Vectors
    glm::vec3 m_camPosition;
    glm::vec3 m_targetPos;
    glm::vec3 m_worldUp;

    glm::vec3 m_camDirection;
    glm::vec3 m_camRight;
    glm::vec3 m_camUp;

    // Matrices
    glm::mat4 m_view;
    glm::mat4 m_projection;

    float m_yaw;
    float m_pitch;
    float m_speed;
    float m_fov;

    glm::mat4 lookAt() const;
};

#endif //FT_VOX_CAMERA_HPP
