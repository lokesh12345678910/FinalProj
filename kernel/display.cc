 
#include "process.h"
#include "shared.h"
#include "vga.h"
#include "threads.h"
#include "smp.h"
#include "fs_display.h"

namespace Display {
       
       // Colors that we can work with
       const uint32_t BYTE_MASK = 0xFF;
       const uint32_t RED = 0xFF6666;
       const uint32_t BLUE = 0x9999FF;
       const uint32_t WHITE = 0xFFFFFF;
       const uint32_t BLACK = 0x0;
       const uint32_t GREY = 0xA0A0A0;

       // Basic info about our display   
       uint8_t* cur_state = nullptr;
       size_t buffer_size = 0;
       const int X_RES = 320;
       const int Y_RES = 200;
       const int BYTES_PER_PIXEL = 1;
       int cursor_pos[] = {0, 0};
       InterruptSafeLock ref_lock{};
       Window* first_window;
       

       // By Connor Byerman: converts a 32 bit color to 8 bit
       uint8_t convert_to_8_bit(uint32_t color) {
              auto r = color >> 16;
              auto g = (color >> 8) & 0xFF;
              auto b = color & BYTE_MASK;

              r = r >> 5;
              g = r >> 5;
              b = b >> 6;

              return (r << 5) || (g << 2) || b;
       }

       // Bye Connor Byerman: Draws a rectangle of a given color into a space in the 
       void draw_rect(int x, int y, int width, int height, uint8_t color) {
              if (x + width > X_RES || y + height > Y_RES) return;
              if (x < 0 || y < 0) return;
              for (int i = y; i < height; i++) {
                     for (int j = x; j < width; j++) {
                            cur_state[i * X_RES + j] = color;
                     }
              }
       }

       void setup_background() {
              draw_rect(0, 0, X_RES, Y_RES, convert_to_8_bit(WHITE));
              draw_rect(X_RES / 3, Y_RES / 3, 2 * X_RES / 3, 2 * Y_RES / 3, Display::convert_to_8_bit(Display::BLUE));
       }

       
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
             uint16_t pos = y * X_RES + x;      // Y_RES = VGA_WIDTH?
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

       

       void refresh(){
              while (true) {
                     ref_lock.lock();
                     setup_background(); //paints the desktop and all windows
                     //show_cursor(); //finally paints the cursor so it's on top
                     ref_lock.unlock();

                     // for calling show_cursor:
                     //show_cursor(cursor_pos[0], cursor_pos[1]);

                     //while (inb(0x3DA) & 0x08);
                     //while (!(inb(0x3DA) & 0x08));

                     ref_lock.lock();
                     for (uint32_t i = 0; i < buffer_size; i++) {
                            auto color = cur_state[i];
                            VGA::write_pixel8(i % X_RES, i / X_RES, color, X_RES);
                     }
                     ref_lock.unlock();
              }
       }

       void init_display() {
              VGA::write_regs(VGA::g_320x200x256);
              VGA::g_wd = X_RES;

              buffer_size = X_RES * Y_RES * BYTES_PER_PIXEL;
              cur_state = new uint8_t[buffer_size];
              for (uint32_t i = 0; i < buffer_size; i++) cur_state[i] = 0;

              auto proc = Shared<Process>::make(true);
              thread(proc, [] {
                     refresh();
              });
       }

       void destroy_display() {
              delete cur_state;
       }
 }