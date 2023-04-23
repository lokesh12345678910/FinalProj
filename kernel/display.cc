 
#include "process.h"
#include "shared.h"
#include "vga.h"
#include "threads.h"

namespace Display {
       
       uint8_t* z_buffer = nullptr;
       size_t buffer_size = 0;
       uint8_t* back_buffer = nullptr;
       const int X_RES = 320;
       const int Y_RES = 200;
       const int BYTES_PER_PIXEL = 1;
       int cursor_pos[] = {0, 0};

       void draw_rect(int x, int y, int width, int height, uint8_t color) {
              if (x + width > 320 || y + height > 200) return;
              if (x < 0 || y < 0) return;
              for (int i = x; i < width; i++) {
                     for (int j = y; j < height; j++) {
                            VGA::write_pixel8(i, j, color);
                     }
              }
       }

       void setup_background() {
              draw_rect(0, 0, X_RES, Y_RES, uint8_t(0xc7c7));
       }

       void show_cursor() {

       }

       /*
       // cursor_start & cursor_end = rows where cursor starts & ends
      void show_cursor(uint8_t cursor_start, uint8_t cursor_end) {
             outb(0x3D4, 0x0A);
             outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
     
             outb(0x3D4, 0x0B);
             outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
      }


      void hide_cursor(){
             outb(0x3D4, 0x0A);
             outb(0x3D5, 0x20);
      }


      // updates location of cursor
      void moving_cursor(int x, int y){  // x & y = current coordinates of cursor
             uint16_t pos = y * Y_RES + x;      // Y_RES = VGA_WIDTH?
             outb(0x3D4, 0x0F);
             outb(0x3D5, (uint8_t) (pos & 0xFF));
             outb(0x3D4, 0x0E);
             outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
      }


      uint16_t get_cursor_pos(){
             uint16_t pos = 0;


             outb(0x3D4, 0x0F);
             pos |= inb(0x3D5);
             outb(0x3D4, 0x0E);
             pos |= ((uint16_t)inb(0x3D5)) << 8;


             return pos;
      }

       */

       void refresh(){
              while (true) {

                     setup_background(); //paints the desktop and all windows
                     show_cursor(); //finally paints the cursor so it's on top

                     /* for calling show_cursor: 
                     // get x and y from pos?
                     int x;
                     int y;
                     show_cursor(x, y);  */

                     #if WAIT_FOR_VERTICAL_RETRACE
                            while (inb(0x3DA) & 0x08);
                            while (!(inb(0x3DA) & 0x08));
                     #endif
                     memcpy((uint8_t*) VGA::get_fb_seg(), z_buffer, buffer_size);
              }
       }

       void init_display() {
              VGA::write_regs(VGA::g_320x200x256_modex);

              buffer_size = X_RES * Y_RES * BYTES_PER_PIXEL;
              z_buffer = new uint8_t[buffer_size];
              for (uint32_t i = 0; i < buffer_size; i++) z_buffer[i] = 0;

              auto proc = Shared<Process>::make(true);
              thread(proc, [] {
                     refresh();
              });
       }
 }