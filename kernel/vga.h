#include "machine.h"
#include "stdint.h"
#include "debug.h"

namespace VGA {

    extern unsigned char g_90x60_text[];
    extern unsigned char g_640x480x16[];
    extern unsigned char g_320x200x256[];
    extern unsigned char g_320x200x256_modex[];

    void dump_regs(uint8_t* regs);

    void read_regs(uint8_t* regs);

    void write_regs(uint8_t* regs);

    extern uint32_t g_wd, g_ht;

    void write_pixel8(uint32_t x, uint32_t y, uint8_t c, uint32_t );
    
    uint32_t get_fb_seg();

    void dump_state();

    void set_text_mode(int hi_res);

    
}