#include "vga.h"

void vga_set_mode(uint8_t mode) {
  _asm
  {
    mov ah, 0x00
    mov al, [mode]
    int 0x10
  }
}

void vga_vertical_sync(void) { // block until vertical refresh cycle has been complete
  while (inp(0x3da) & 0x08);
  while (!(inp(0x03da) & 0x08));
}

void vga_set_pallete_mask(uint8_t mask) {
  _asm
  {
    mov dx, 0x03c6
    mov al, [mask]
    out dx, al
  }
}

void vga_set_color(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {

  _asm
  {
    mov dx, 0x03c8
    mov al, [index]
    out dx, al

    mov dx, 0x03c9
    mov al, [r]
    out dx, al

    mov al, [g]
    out dx, al

    mov al, [b]
    out dx, al
  }
}

void vga_get_color(uint8_t index, uint8_t *r, uint8_t *g, uint8_t *b) {
  uint8_t red, green, blue;

  _asm
  {
    mov dx, 0x03c7
    mov al, [index]
    out dx, al

    mov dx, 0x03c9
    in al, dx
    mov [red], al

    in al, dx
    mov [green], al
    
    in al, dx
    mov [blue], al
  }

  *r = red;
  *g = green;
  *b = blue;
}

uint8_t *vga_get_address(void) {
  return (uint8_t *) 0xA0000000;
}