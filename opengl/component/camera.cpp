#include"pch.h"
#include "camera.h"
#include "transform.h"
#include "../util/math.h"
#include"input.h"
#include"win_dow.h"

using namespace utils::math;

namespace component {

    Camera::Camera(Transform* T, View view) :
        Component(),
        fov(45.0f),
        near_clip(0.1f),
        far_clip(100.0f),
        move_speed(5.0f),
        zoom_speed(0.04f),
        rotate_speed(0.3f),
        orbit_speed(0.05f),
        initial_position(T->position),
        initial_rotation(T->rotation),
        T(T), view(view) {}

    Camera::Camera():
        Component(),
        fov(60.0f),
        near_clip(1.0f),
        far_clip(10000.0f),
        move_speed(5.0f),
        zoom_speed(0.04f),
        rotate_speed(0.9f),
        orbit_speed(0.05f)

    {
        T=new Transform();
        initial_position=T->position;
        initial_rotation=T->rotation;
        view=View::Perspective;

    }

    glm::mat4 Camera::GetViewMatrix() const {

        if constexpr (true) {
            return glm::inverse(T->transform);
        }
        else {
            return glm::lookAt(T->position, T->position + T->forward, T->up);
        }
    }

    glm::mat4 Camera::GetProjectionMatrix() const {
        return (view == View::Orthgraphic)
            ? glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_clip, far_clip)
            : glm::perspective(glm::radians(fov), aspect, near_clip, far_clip);
    }

    void Camera::Resize(int w, int h)
    {
      aspect=float(w)/float(h);
    }
   //需要修复间隔时间、宽高比
    void Camera::Update(float dt) {
        Resize(Window::width,Window::height);

        if(Input::GetMouseDown(MouseButton::Right)){
        // rotation is limited to the X and Y axis (pitch and yawn only, no roll)
        float euler_y = T->euler_y - Input::GetCursorOffset(MouseAxis::Horizontal) * rotate_speed;
        float euler_x = T->euler_x - Input::GetCursorOffset(MouseAxis::Vertical) * rotate_speed;

        euler_y = glm::radians(euler_y);
        euler_x = glm::radians(glm::clamp(euler_x, -89.0f, 89.0f));  // clamp vertical rotation
        glm::quat rotation = glm::quat_cast(glm::eulerAngleYXZ(euler_y, euler_x, 0.0f));

        T->SetRotation(rotation);
        }
        // translation (not normalized, movement is faster along the diagonal)
        if (Input::GetKeyDown('w')) {
            T->Translate(T->forward * (move_speed * dt));
        }

        if (Input::GetKeyDown('s')) {
            T->Translate(-T->forward * (move_speed * dt));
        }

        if (Input::GetKeyDown('a')) {
            T->Translate(-T->right * (move_speed * dt));
        }

        if (Input::GetKeyDown('d')) {
            T->Translate(T->right * (move_speed * dt));
        }

        if (Input::GetKeyDown('z')) {
            T->Translate(-T->up * (move_speed * dt));
        }

        if (Input::GetKeyDown(0x20)) {  // VK_SPACE
            T->Translate(T->up * (move_speed * dt));
        }

    }

}
