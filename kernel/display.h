
#include "stdint.h"
#include "atomic.h"

namespace Display {

    // Colors that we can work with
    const uint32_t BYTE_MASK = 0xFF;
    const uint32_t RED = 0xFF6666;
    const uint32_t GREEN = 0x66FF66;
    const uint32_t BLUE = 0x9999FF;
    const uint32_t WHITE = 0xFFFFFF;
    const uint32_t BLACK = 0x0;
    const uint32_t GREY = 0x3F3F3F;

    const int X_RES = 320;
    const int Y_RES = 200;
    const int X_TEXT = 8;
    const int Y_TEXT = 16;
    const int BYTES_PER_PIXEL = 1;
    extern uint8_t* cur_state;
    extern size_t buffer_size;
    extern int cursor_pos[];
    extern InterruptSafeLock ref_lock;

    uint8_t convert_to_6_bit(uint32_t color);

    void init_320x200_display();

    void destroy_display();

    void draw_rect(int X, int Y, int Width, int Height, uint8_t RGB);

    void draw_char(char chr, int x, int y, uint8_t color);

    void write_line(char* str, int x, int y, uint8_t color);

    void setup_background();
    
    void add_window();

    struct Window {
        int x, y, width, height;
        Window* next = nullptr;

        Window(int x, int y, int wd, int ht): x(x), y(y), width(wd), height(ht), next(nullptr) {}

        ~Window() {}

        void draw_window() {
            draw_rect(x, y, width, width, convert_to_6_bit(WHITE));
        }
    };
}