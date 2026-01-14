#include"Camera.h"

Camera::Camera(int width, int height, EngineMath::Vec3 posiiton) {

	Camera::width = width;
	Camera::height = height;
	Position = posiiton;
}

void Camera::Matrix(float fovDeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {

	EngineMath::Mat4 view = EngineMath::Mat4::identity();
	EngineMath::Mat4 projection = EngineMath::Mat4::identity();

	view = EngineMath::Mat4::lookAt(Position, Position + Orientation, Up);
	projection = EngineMath::Mat4::perspective(EngineMath::radians(fovDeg), (float)width / (float)height, nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, EngineMath::value_ptr(projection * view));
}

void Camera::Inputs(Window* window, float deltaTime) {
    GLFWwindow* _window = window->handle;

    // 1. Kumpulkan semua input ke dalam satu vektor arah
    EngineMath::Vec3 inputDir(0, 0, 0);

    // Maju Mundur
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) inputDir += Orientation;
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) inputDir -= Orientation;

    // Kiri Kanan (Strafe)
    EngineMath::Vec3 right = Orientation.cross(Up).normalize();
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) inputDir += right;
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) inputDir -= right;

    // Naik Turun
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS) inputDir += Up;
    if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) inputDir -= Up;

    // 2. Terapkan Smoothing (Lerp)
    // inputDir.normalize() memastikan kecepatan diagonal tidak lebih kencang
    if (inputDir.length() > 0) {
        inputDir = inputDir.normalize();
    }

    // 2. SMOOTHING PADA VELOCITY (Bukan Posisi)
        // Target adalah kecepatan maksimal (input * speed)
    EngineMath::Vec3 targetVelocity = inputDir * speed;

    float lerpFactor = friction * deltaTime;
    if (lerpFactor > 1.0f) lerpFactor = 1.0f;

    // CurrentVelocity mengejar targetVelocity
    CurrentVelocity = EngineMath::Vec3::lerp(CurrentVelocity, targetVelocity, friction * lerpFactor);

    // 3. Update Posisi berdasarkan Velocity saat ini
    Position += CurrentVelocity * deltaTime;

    // --- MOUSE INPUTS (Smooth Look) ---
    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (firstClick) {
            glfwSetCursorPos(_window, (width / 2.0), (height / 2.0));
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(_window, &mouseX, &mouseY);

        // Update TARGET, bukan nilai saat ini
        float rotX = (float)(mouseY - (height / 2.0)) / height * sensitivity * 100.0f * deltaTime;
        float rotY = (float)(mouseX - (width / 2.0)) / width * sensitivity * 100.0f * deltaTime;

        targetYaw += rotY;
        targetPitch -= rotX;

        if (targetPitch > 89.0f) targetPitch = 89.0f;
        if (targetPitch < -89.0f) targetPitch = -89.0f;

        glfwSetCursorPos(_window, (width / 2.0), (height / 2.0));
    }
    else {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }

    // --- POST-PROCESS SMOOTHING UNTUK MOUSE ---
    // Interpolasi yaw dan pitch saat ini menuju target
    float mSmoothFactor = mouseSmoothing * deltaTime;
    if (mSmoothFactor > 1.0f) mSmoothFactor = 1.0f;

    currentYaw = EngineMath::lerp(currentYaw, targetYaw, mSmoothFactor);
    currentPitch = EngineMath::lerp(currentPitch, targetPitch, mSmoothFactor);

    // Kalkulasi Orientation berdasarkan nilai yang sudah di-smooth
    EngineMath::Vec3 direction;
    direction.x = cos(EngineMath::radians(currentYaw)) * cos(EngineMath::radians(currentPitch));
    direction.y = sin(EngineMath::radians(currentPitch));
    direction.z = sin(EngineMath::radians(currentYaw)) * cos(EngineMath::radians(currentPitch));
    Orientation = direction.normalize();
}