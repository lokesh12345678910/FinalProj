#include "display.h"
#include "threads.h"
#include "ext2.h"

struct FsDisplay {
    Shared<Ext2> fs;
    Shared<Node> cur_dir;
    int x, y, width, height, font_size;


    FsDisplay(Shared<Ext2> file_s): fs(file_s), cur_dir(file_s->root), 
                                    x(0), y(0), width(Display::X_RES), height(Display::Y_RES) {
                                        using namespace Display;
                                        draw_rect(x, y, width, height, convert_to_8_bit(BLACK));
                                    }

    void display_files() {
        ASSERT(cur_dir->is_dir());
        char** entries = new char*[cur_dir->entry_count()];
        
        cur_dir->entries([entries](uint32_t number, const char* nm) {
            
        });

        delete entries;
    }
        
    }
};