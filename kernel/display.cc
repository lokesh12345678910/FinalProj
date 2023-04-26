#include "kernel.h"
#include "process.h"
#include "shared.h"
#include "vga.h"
#include "smp.h"
#include "ext2.h"
#include "fs_display.h"

namespace Display {

       // Basic info about our display   
       uint8_t* cur_state = nullptr;
       size_t buffer_size = 0;
       int cursor_pos[] = {0, 0};
       InterruptSafeLock ref_lock{};
       unsigned char* font = nullptr;
       int baseline = 0;
       Window* first_window;
       

       // By Connor Byerman: converts a 32 bit color to 8 bit
       uint8_t convert_to_6_bit(uint32_t color) {
              uint8_t r = color >> 16;
              uint8_t g = (color >> 8) & BYTE_MASK;
              uint8_t b = color & BYTE_MASK;

              r = r >> 6;
              g = g >> 6;
              b = b >> 6;

              return (r << 4) + (g << 2) + b;
       }

       // Updates a pixel in the current state
       void pixel_in_cur(int x, int y, uint8_t color) {
              if (x < 0 || y < 0) return;
              if (x > X_RES || y > Y_RES) return;
              cur_state[y * X_RES + x] = color;
       }

       // Draws a rectangle of a given color into a space in the 
       void draw_rect(int x, int y, int width, int height, uint8_t color) {
              for (int i = x; i < width; i++) {
                     for (int j = y; j < height; j++) {
                            pixel_in_cur(i, j, color);
                     }
              }
       }

       void setup_background() {
              draw_rect(0, 0, X_RES, Y_RES, convert_to_6_bit(BLUE));
       }
       
       // cursor_start & cursor_end = rows where cursor starts & ends
      /*void show_cursor(uint8_t cursor_start, uint8_t cursor_end) {
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
      }*/

       // Draws a given character in the initialized text mode
       void draw_char(uint8_t chr, int x, int y, uint8_t color) {
              
              uint32_t mask = 1;

              uint8_t* chr_symbol = font + (uint32_t(chr) << 4);

              for (int i = 0; i < Y_TEXT; i++) {
                     // Bit map in reverse, so make sure to account for that
                     for (int j = X_TEXT - 1; j >= 0; j--) {
                            if (chr_symbol[i] & mask) pixel_in_cur(x + j, y + i - baseline, color);
                            mask = mask << 1;
                     }
                     mask = 1;
              }
       }

       // Writes out a given string in the display using a 8x16 bit-mapping
       void write_line(const char* str, int x, int y, uint8_t color) {
              auto chr = (uint8_t*) str;
              auto index = 0;

              while (chr[index] != '\0') {
                     draw_char(*(chr + index), (x + (index * X_TEXT)), y, color);
                     index++;
              }
       }

       // Adds a file window to the display
       void add_window() {
              FsDisplay* window = new FsDisplay(gheith::root_fs);
              if (first_window == nullptr) first_window = window;
              else {
                     auto temp = first_window;
                     while (temp->next != nullptr) temp = temp->next;
                     temp->next = window;
              }
       }

       void display_window(int id) {
              if (first_window == nullptr) return;
              auto temp = first_window;
              auto cur_id = 1;
              while (temp != nullptr) {
                     if (cur_id == id) {
                            temp->draw_window();
                            return;
                     }
                     id++;
              }
       }

       void refresh(){
              while (true) {
                     //ref_lock.lock();
                     setup_background(); //paints the desktop and all windows
                     //show_cursor(); //finally paints the cursor so it's on top
                     display_window(1);
                     //ref_lock.unlock();

                     // for calling show_cursor:
                     //show_cursor(cursor_pos[0], cursor_pos[1]);

                     while (inb(0x3DA) & 0x08);
                     while (!(inb(0x3DA) & 0x08));

                     //ref_lock.lock();
                     for (uint32_t i = 0; i < buffer_size; i++) {
                            auto color = cur_state[i];
                            VGA::write_pixel8(i % X_RES, i / X_RES, color, X_RES);
                     }
                     //ref_lock.unlock();
              }
       }

       void init_320x200_display() {
              VGA::write_regs(VGA::g_320x200x256);
              font = VGA::g_8x16_font;
              baseline = 12;

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