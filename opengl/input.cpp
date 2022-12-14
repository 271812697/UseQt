#include "input.h"
#define VK_SPACE          0x20
#define VK_RETURN         0x0D
#define VK_ESCAPE         0x1B
#pragma warning(push)
#pragma warning(disable : 4244)

// internally, all keys are defined by standard Win32 virtual-key codes and ASCII codes
// if an input library defines its own key codes, they must be remapped to our standard
std::unordered_map<unsigned char, bool> Input::keybook {
    { 'w', 0 }, { 's', 0 }, { 'a', 0 }, { 'd', 0 }, {'q',0},{'e',0},{ 'z', 0 }, { 'r', 0 },
    { VK_SPACE, 0 }, { VK_RETURN, 0 }, { VK_ESCAPE, 0 }

};

void Input::Clear() {
    // clean up all key states
    for (auto& keystate : keybook) {
        keystate.second = 0;
    }

    SetCursor(cursor_pos_x, cursor_pos_y);

    // reset cursor offsets, mouse clicks and scroll offset
    cursor_delta_x = cursor_delta_y = 0.0f;
    mouse_button_l = mouse_button_r = mouse_button_m = false;
    scroll_offset = 0.0f;
}

void Input::ShowCursor() {

}

void Input::HideCursor() {

}

void Input::SetKeyDown(unsigned char key, bool pressed) {
    // ignore keys that are not registered in the keybook
    if (keybook.find(key) == keybook.end()) {
        return;
    }
    keybook[key] = pressed;
}

bool Input::GetKeyDown(unsigned char key) {
    // ignore keys that are not registered in the keybook
    if (keybook.find(key) == keybook.end()) {
        return false;
    }
    return keybook[key];
}

void Input::SetMouseDown(MouseButton button, bool pressed) {
    switch (button) {
        case MouseButton::Left:   mouse_button_l = pressed;  break;
        case MouseButton::Right:  mouse_button_r = pressed;  break;
        case MouseButton::Middle: mouse_button_m = pressed;  break;
        default:
            return;
    }
}

bool Input::GetMouseDown(MouseButton button) {
    switch (button) {
        case MouseButton::Left:   return mouse_button_l;
        case MouseButton::Right:  return mouse_button_r;
        case MouseButton::Middle: return mouse_button_m;
        default:
            return false;
    }
}

void Input::SetCursor(float new_x, float new_y) {
    // x and y are measured in window coordinates, with the origin at the top-left corner
    cursor_delta_x = new_x - cursor_pos_x;
    cursor_delta_y = new_y - cursor_pos_y;
    cursor_pos_x = new_x;
    cursor_pos_y = new_y;

   {

   }
}

float Input::GetCursorOffset(MouseAxis axis) {
    float offset = 0.0f;

    if (axis == MouseAxis::Horizontal) {
        offset = cursor_delta_x;
        cursor_delta_x = 0;  // reset cursor offset
    }
    else if (axis == MouseAxis::Vertical) {
        offset = cursor_delta_y;
        cursor_delta_y = 0;  // reset cursor offset
    }

    return offset;
}

float Input::GetCursorPosition(MouseAxis axis) {
    return (axis == MouseAxis::Horizontal) ? cursor_pos_x : cursor_pos_y;
}

glm::ivec2 Input::GetCursorPosition() {
    return glm::ivec2(cursor_pos_x, cursor_pos_y);
}

void Input::SetScroll(float offset) {
    scroll_offset += offset;
}

float Input::GetScrollOffset() {
    float offset = scroll_offset;
    scroll_offset = 0.0f;  // reset scroll offset so that we never read twice
    return offset;
}

#pragma warning(pop)
