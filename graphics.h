#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "/opt/homebrew/Cellar/sdl2/2.26.2/include/SDL2/SDL.h"
#include <string>

class Graphics {
    static const int WIDTH {64};
    static const int HEIGHT {32};

    SDL_Window* m_window {};
    SDL_Renderer* m_renderer {};
    SDL_Texture* m_texture {};

    const char* m_title {};
    int m_width {};
    int m_height {};

    uint32_t m_pixels[WIDTH * HEIGHT] {};

public:
  // Create a texture
    Graphics(const char *title, int w, int h);

    void set_pixels(uint32_t gfx[]);
    uint32_t* get_pixels();


    void draw_colour();

    void update_t(void const*, int pitch);
    void input(uint8_t key[]);
    void quit();
};

#endif
