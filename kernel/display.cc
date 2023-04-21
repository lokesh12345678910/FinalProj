 
#
#include "VGA_osdevFile.c"
#include "threads.h"

namespace Display {
       uint8_t* z_buffer = nullptr;
       size_t buffer_size = -1;
       uint8_t* back_buffer = nullptr;
       
       void init_display() {
              VGA::write_regs(VGA::g_320x200x256);

              buffer_size = 320 * 200 * 4;
              z_buffer = new uint8_t[buffer_size];

              auto proc = Shared<Process>::make(true);
              thread(proc, [] {
                     refresh();
              });
       }
 
       void refresh(){
              while (true) {

                     setup_background(); //paints the desktop and all windows
                     show_cursor(); //finally paints the cursor so it's on top

                     #if WAIT_FOR_VERTICAL_RETRACE
                            while ((inportb(0x3DA) & 0x08));
                            while (!(inportb(0x3DA) & 0x08));
                     #endif
                     memcpy((uint8_t*) VGA::get_fb_seg(), z_buffer, buffer_size);
              }
       }

       void draw_rect(int x, int y, int width, int height, uint32_t color) {

       }

       void setup_background() {

       }

       void show_cursor() {

       }
 }