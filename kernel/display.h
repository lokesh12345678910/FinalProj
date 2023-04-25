
#include "stdint.h"

namespace Display {

    // Colors that we can work with
    const uint32_t BYTE_MASK = 0xFF;
    const uint32_t RED = 0xFF6666;
    const uint32_t BLUE = 0x9999FF;
    const uint32_t WHITE = 0xFFFFFF;
    const uint32_t BLACK = 0x0;
    const uint32_t GREY = 0xA0A0A0;

    extern uint8_t* cur_state;
    extern size_t buffer_size;
    const int X_RES = 320;
    const int Y_RES = 200;
    const int BYTES_PER_PIXEL = 1;
    extern int cursor_pos[];
    extern InterruptSafeLock ref_lock;

    struct Window {
        int x, y, width, height;
        Window* next = nullptr;

        Window(int x, int y, int wd, int ht): x(x), y(y), width(wd), height(ht) {}

        void draw_base() {
            Display::draw_rect(0, 0, 100, 10, convert_to_8_bit(WHITE));
        }
    };

    uint8_t convert_to_8_bit(uint32_t color);

    void init_display();

    void destroy_display();

    void draw_rect(int X, int Y, int Width, int Height, uint8_t RGB);

    void setup_background();
}