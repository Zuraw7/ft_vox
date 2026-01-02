#include "Camera.hpp"

#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../utils/declarations.hpp"

Camera::Camera(const glm::vec3 position, const glm::vec3 target, const glm::vec3 worldUp) : m_camPosition(position), m_targetPos(target), m_worldUp(worldUp), m_yaw(-90.0f), m_pitch(0.0f), m_speed(1.0f), m_fov(80.0f) {
    updateCameraVectors();
    m_view = lookAt();
    m_projection = glm::perspective(glm::radians(m_fov), Resolution::aspect, NEAR, FAR);
}

const glm::mat4 &Camera::getCamView() {
    m_view = lookAt();
    return m_view;
}

const glm::mat4 &Camera::getCamProjection() const {
    return m_projection;
}

const glm::vec3 & Camera::getPosition() const {
    return m_camPosition;
}

void Camera::updateCameraDirection(double dx, double dy) {
    m_yaw += dx * SENSITIVITY;
    m_pitch -= dy * SENSITIVITY;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    else if (m_pitch < -89.0f)
        m_pitch = -89.0f;
    updateCameraVectors();
}

void Camera::updateCameraPos(const CameraDirection dir, const double deltaTime) {
    float velocity = static_cast<float>(deltaTime * m_speed);

    switch (dir) {
        case CameraDirection::FORWARD:
            m_camPosition += m_camDirection * velocity;
            break;
        case CameraDirection::BACKWARD:
            m_camPosition -= m_camDirection * velocity;
            break;
        case CameraDirection::RIGHT:
            m_camPosition += m_camRight * velocity;
            break;
        case CameraDirection::LEFT:
            m_camPosition -= m_camRight * velocity;
            break;
        case CameraDirection::UP:
            m_camPosition += m_worldUp * velocity;
            break;
        case CameraDirection::DOWN:
            m_camPosition -= m_worldUp * velocity;
            break;
        case CameraDirection::NONE:
            break;
    }
}

void Camera::updateCameraZoom(double dy) {
    if (m_fov >= 1.0f && m_fov <= 45.0f) {
        m_fov -= dy;
    } else if (m_fov < 1.0f) {
        m_fov = 1.0f;
    } else {
        m_fov = 45.0f;
    }
    m_projection = glm::perspective(glm::radians(m_fov), Resolution::aspect, NEAR, FAR);;
}

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    direction.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    direction.y = std::sin(glm::radians(m_pitch));
    direction.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

    m_camDirection = glm::normalize(direction);
    m_camRight = glm::normalize(glm::cross(m_camDirection, m_worldUp));
    m_camUp = glm::normalize(glm::cross( m_camRight, m_camDirection));
}

glm::mat4 Camera::lookAt() const {
    return glm::lookAt(m_camPosition, m_camPosition + m_camDirection, m_worldUp);
}
