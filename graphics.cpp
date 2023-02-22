#include <iostream>
#include "graphics.h"
#include "/opt/homebrew/Cellar/sdl2/2.26.2/include/SDL2/SDL.h"


Graphics::Graphics(const char* title, int w, int h)
    : m_title{ title }, m_width{ w }, m_height{ h }
{
    SDL_Init(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow(m_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_width, m_height, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

// allows Update_t to update the texture
void Graphics::set_pixels(uint32_t gfx[]) {
    for(int i = 0; i < 2048; ++i) {
        m_pixels[i] = (0x00FFFFFF * gfx[i]) | 0xFF000000; // figure out how this works
    }
}


uint32_t* Graphics::get_pixels() {
    return m_pixels;
}


void Graphics::update_t(void const* buffer, int pitch) {
    SDL_UpdateTexture(m_texture, NULL, buffer, pitch);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}


void Graphics::input(uint8_t keys[]) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                quit();
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit();
                        break;
                    case SDLK_x:
                        keys[0x0] = 1;
                        break;
                    case SDLK_1:
                        keys[0x1] = 1;
                        std::cout << "P1: UP\n";
                        break;
                    case SDLK_2:
                        keys[0x2] = 1;
                        break;
                    case SDLK_3:
                        keys[0x3] = 1;
                        break;
                    case SDLK_q:
                        keys[0x4] = 1;
                        std::cout << "P1: DOWN\n";
                        break;
                    case SDLK_w:
                        keys[0x5] = 1;
                        break;
                    case SDLK_e:
                        keys[0x6] = 1;
                        break;
                    case SDLK_a:
                        keys[0x7] = 1;
                        break;
                    case SDLK_s:
                        keys[0x8] = 1;
                        break;
                    case SDLK_d:
                        keys[0x9] = 1;
                        break;
                    case SDLK_z:
                        keys[0xa] = 1;
                        break;
                    case SDLK_c:
                        keys[0xb] = 1;
                        break;
                    case SDLK_4:
                        keys[0xc] = 1;
                        std::cout << "P1: UP\n";
                        break;
                    case SDLK_r:
                        keys[0xd] = 1;
                        std::cout << "P1: DOWN\n";
                        break;
                    case SDLK_f:
                        keys[0xe] = 1;
                        break;
                    case SDLK_v:
                        keys[0xf] = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                    case SDLK_x:
                        keys[0x0] = 0;
                        break;
                    case SDLK_1:
                        keys[0x1] = 0;
                        break;
                    case SDLK_2:
                        keys[0x2] = 0;
                        break;
                    case SDLK_3:
                        keys[0x3] = 0;
                        break;
                    case SDLK_q:
                        keys[0x4] = 0;
                        break;
                    case SDLK_w:
                        keys[0x5] = 0;
                        break;
                    case SDLK_e:
                        keys[0x6] = 0;
                        break;
                    case SDLK_a:
                        keys[0x7] = 1;
                        break;
                    case SDLK_s:
                        keys[0x8] = 1;
                        break;
                    case SDLK_d:
                        keys[0x9] = 1;
                        break;
                    case SDLK_z:
                        keys[0xa] = 1;
                        break;
                    case SDLK_c:
                        keys[0xb] = 1;
                        break;
                    case SDLK_4:
                        keys[0xc] = 1;
                        break;
                    case SDLK_r:
                        keys[0xd] = 1;
                        break;
                    case SDLK_f:
                        keys[0xe] = 1;
                        break;
                    case SDLK_v:
                        keys[0xf] = 1;
                        break;

                }
        }
    }
}


void Graphics::quit() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    exit(0);
}
