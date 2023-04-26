#include "machine.h"
#include "stdint.h"
#include "debug.h"

namespace VGA {

    extern unsigned char g_320x200x256[];
    extern unsigned char g_8x16_font[4096];

    void dump_regs(uint8_t* regs);

    void read_regs(uint8_t* regs);

    void write_regs(uint8_t* regs);

    void write_pixel8(uint32_t x, uint32_t y, uint8_t c, uint32_t );
    
    uint32_t get_fb_seg();

    void dump_state();

    void set_text_mode(int hi_res);

    
}