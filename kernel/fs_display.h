#include "display.h"
#include "ext2.h"

struct FsDisplay: Display::Window {
    Shared<Ext2> fs;
    Shared<Node> cur_dir;


    FsDisplay(Shared<Ext2> file_s): Display::Window(0, 0, Display::X_RES, Display::Y_RES),
                                    fs(file_s), cur_dir(file_s->root) {}

    void draw_window() {
        ASSERT(cur_dir->is_dir());
        auto index = 0;

        uint8_t grey = Display::convert_to_6_bit(Display::GREY);
        uint8_t black = 0x0;
        uint8_t white = Display::convert_to_6_bit(Display::WHITE);

        Display::draw_rect(x, y, width, height, white);
        auto wd = width;
        
        cur_dir->entries([index, wd, grey, black] (uint32_t number, const char* nm) {
            Display::draw_rect(20, 32 * index, wd, 32, grey);
            Display::write_line((char*) nm, 20, 32 * index, black);
        });
    }
};