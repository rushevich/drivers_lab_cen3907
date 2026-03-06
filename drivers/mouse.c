#include "mouse.h"

bool mouse_reset(void) {
  uint16_t mouse_status;

  _asm
  {
    mov ax, 0x00
    int 0x33
    mov [mouse_status], ax
  }

  return mouse_status != 0x00;
}

int mouse_get_button_presses(int16_t button) { // since last call
  int16_t press_count;
  
  _asm
  {
    mov ax, 0x05
    mov bx, [button]
    int 0x33
    mov [press_count], bx
  }

  return press_count;
}

int mouse_get_button_releases(int16_t button) { // since last call
  int16_t release_count;
  
  _asm
  {
    mov ax, 0x06
    mov bx, [button]
    int 0x33
    mov [release_count], bx
  }

  return release_count;
}

void mouse_get_mickeys(int16_t *mickeys_x, int16_t *mickeys_y) { // add to pointers
  int16_t x, y;
_asm
{
  mov ax, 0x0b
  int 0x33
  mov [x], cx
  mov [y], dx
}

  *mickeys_x += x;
  *mickeys_y += y;
}

void mouse_calculate_pos(int16_t* value, int16_t* mickeys, int16_t rate) {
  int16_t total_possible_movement = *mickeys * 8;

  int16_t delta_pos = total_possible_movement / rate;

  *value += delta_pos;

  *mickeys = (total_possible_movement % rate) / 8;
}

void mouse_show_cursor(void) {
  _asm
  {
    mov ax, 0x01
    int 0x33
  }
}

void mouse_hide_cursor(void) {
  _asm
  {
    mov ax, 0x02
    int 0x33
  }
}

void mouse_set_limits_x(int16_t minimum, int16_t maximum) {
  _asm
  {
    mov ax, 0x07
    mov cx, [minimum]
    mov dx, [maximum]
    int 0x33
  }
}

void mouse_set_limits_y(int16_t minimum, int16_t maximum) {
  _asm
  {
    mov ax, 0x08
    mov cx, [minimum]
    mov dx, [maximum]
    int 0x33
  }
}

void mouse_set_position(int16_t x, int16_t y) {
  _asm
  {
    mov ax, 0x04
    mov cx, [x]
    mov dx, [y]
    int 0x33
  }
}

void mouse_get_state(struct mouse_state* state) {
  int16_t x_pos, y_pos;
  uint16_t buttons;
  _asm{
    mov ax, 0x03
    int 0x33
    mov [x_pos], cx
    mov [y_pos], dx
    mov [buttons], bx
  }

  state->x = x_pos;
  state->y = y_pos;

  state->left = (buttons & 0x01) != 0;
  state->right = (buttons & 0x02) != 0;
  state->middle = (buttons & 0x04) != 0;
}

void mouse_set_movement_rate(int16_t rate_x, int16_t rate_y) {
  int16_t scaled_rate_x = 8 * rate_x;
  int16_t scaled_rate_y = 8 * rate_y;
  _asm{
    mov ax, 0x0f
    mov cx, [scaled_rate_x]
    mov dx, [scaled_rate_y]
    int 0x33
  }
}

void mouse_set_sensitivity(int16_t level_x, int16_t level_y) {
  _asm
  {
    mov ax, 0x01a
    mov bx, [level_x]
    mov cx, [level_y]
    xor dx, dx
    int 0x33
  }
}

void mouse_get_sensitivity(int16_t *level_x, int16_t *level_y) {
  int16_t x_sens, y_sens;

  _asm
  {
    mov ax, 0x1b
    int 0x33
    mov [x_sens], bx
    mov [y_sens], cx
  }

  *level_x = x_sens;
  *level_y = y_sens;
}