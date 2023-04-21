#include "display.h"
#include "threads.h"
#include "ext2.h"

struct fs_display {
    Shared<Ext2> fs;
    Shared<Node> cur_dir;
    int width;
    int height;
    int font_size;

    void display_files() {
        ASSERT(cur_dir->is_dir());
        
        Display::draw_rect(0, 0, 100, 10, 0xc7c7);
        
    }
};