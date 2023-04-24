#include "display.h"
#include "vga.h"

void kernelMain(void) {

    Display::init_display();

    Debug::printf("we're fine");
}