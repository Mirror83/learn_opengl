#pragma once
#include <algorithm>

#include "ext/glm/glm.hpp"
#include "ext/glm/gtc/matrix_transform.hpp"

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right
};

struct EulerAngles
{
    float yaw;
    float pitch;
    // float roll;
};

struct CameraConfig {
    static constexpr float DEFAULT_YAW = -90.0f;
    static constexpr float DEFAULT_PITCH = 0.0f;
    static constexpr float DEFAULT_SPEED = 2.5f;
    static constexpr float DEFAULT_SENSITIVITY = 0.1f;
    static constexpr float DEFAULT_ZOOM = 45.0f;
    static constexpr float MAX_PITCH = 89.0f;
    static constexpr float MIN_ZOOM = 1.0f;
    static constexpr float MAX_ZOOM = 45.0f;
};

class Camera {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up{};
    glm::vec3 right{};
    glm::vec3 worldUp;

    EulerAngles angles;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    void updateCameraVectors() {
        const glm::vec3 newFront{
            cos(glm::radians(angles.yaw)) * cos(glm::radians(angles.pitch)),
            sin(glm::radians(angles.pitch)),
            sin(glm::radians(angles.yaw)) * cos(glm::radians(angles.pitch))
        };
        
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

public:
    explicit Camera(const glm::vec3& position = glm::vec3(0.0f),
                    const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
                    EulerAngles angles = {CameraConfig::DEFAULT_YAW, CameraConfig::DEFAULT_PITCH}
    )
        : position(position)
        , front(glm::vec3(0.0f, 0.0f, -1.0f))
        , worldUp(up)
        , angles(angles)
        , movementSpeed(CameraConfig::DEFAULT_SPEED)
        , mouseSensitivity(CameraConfig::DEFAULT_SENSITIVITY)
        , zoom(CameraConfig::DEFAULT_ZOOM) {
        updateCameraVectors();
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        
        switch (direction) {
            case CameraMovement::Forward:
                position += front * velocity;
                break;
            case CameraMovement::Backward:
                position -= front * velocity;
                break;
            case CameraMovement::Left:
                position -= right * velocity;
                break;
            case CameraMovement::Right:
                position += right * velocity;
                break;
        }
    }

    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        angles.yaw += xOffset;
        angles.pitch += yOffset;

        if (constrainPitch) {
            angles.pitch = std::clamp(angles.pitch, -CameraConfig::MAX_PITCH, CameraConfig::MAX_PITCH);
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yOffset) {
        zoom = std::clamp(zoom - yOffset, 
                         CameraConfig::MIN_ZOOM,
                         CameraConfig::MAX_ZOOM);
    }

    [[nodiscard]] float getZoom() const { return zoom; }
    [[nodiscard]] const glm::vec3& getPosition() const { return position; }
    [[nodiscard]] const glm::vec3& getFront() const { return front; }
};