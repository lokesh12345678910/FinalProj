
namespace Display {

    uint8_t convert_to_8_bit(uint32_t color);

    void init_display();

    void draw_rect(int X, int Y, int Width, int Height, int RGB);

    void draw_line(int X1, int Y1, int X2, int Y2, int RGB);

    void setup_background();

    void show_cursor();
}