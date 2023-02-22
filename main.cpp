#include <iostream>
#include <chrono>
#include <thread>

#include "cpu.h"
#include "graphics.h"
#include "/opt/homebrew/Cellar/sdl2/2.26.2/include/SDL2/SDL.h"

//TODO:
// - Figure out timers!
// above would likely fix speed up issues in space invaders
//
// - fix player 2 error in pong
// player 2 input doesn`t work as intended. can go hold down once to go down. Doesn`t
// go up. once key lift off down you cannot move again. Plyr 1 works perfectly
// Check opcode correctness?


int main(int argc, char* argv[]) {
    (void)argc;

    char* rom = argv[1];

    CPU chip8;
    chip8.load_rom(rom); // load game to memory

    // init screen
    Graphics g1("ChipAte", 640, 320);
    SDL_Event event;
    bool quit = false;

    auto last_cycle_time = std::chrono::high_resolution_clock::now();

    while(!quit) {

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time).count();

        if (dt > 3) {
            chip8.emu_cycle(); // fetch, decode, execute 1 cycle

            if (chip8.draw_flag) {
                chip8.draw_flag = false;
                g1.set_pixels(chip8.gfx);
                g1.update_t(g1.get_pixels(), sizeof(chip8.gfx[0]) * 64);
            }
            g1.input(chip8.key);

        }
        if (chip8.pc > 0x3800) { // in case of null ptrs
            chip8.pc = 0x200;
        }
        /* chip8.debug(); */
    }


    g1.quit();

    return 0;
}
