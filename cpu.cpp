#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "cpu.h"


CPU::CPU() { // when initialised set up the cpu
    pc = 0x200;
    opcode = 0;
    index = 0;
    stack_pointer = 0;
    draw_flag = false;

    // clear registers / stack
    for (int i = 0; i < 16; i++) {
        v[i] = 0;
        stack[i] = 0;
    }
    // clear memory
    for (int i = 0; i < 4096; i++) {
        mem[i] = 0;
    }
    //font set
    load_font();
    // reset timers
    delay_timer = 0;
    sound_timer = 0;
}


void CPU::print_memory() {
    for (int i = 0; i < 200; i++) {
        printf("%04x %04x\n", pc, fetch_opcode());
    }
}


void CPU::load_rom(char* rom) {
    std::ifstream file {rom, std::ios::binary | std::ios::ate};

    if (!file) {
        std::cout << "error: Couldn't open " <<  rom << std::endl;
        exit(1);
    }
    //Get the file size
    std::streampos fsize = file.tellg();

    char* buffer = new char[(unsigned long)fsize];

    file.seekg(0, std::ios::beg);
    file.read((char*)buffer, fsize);
    file.close();

    for (int i = 0; i < fsize; i++) {
        // full opcode will be i and i+1 since each opcode is 2 bytes
        mem[0x200 + i] = (uint8_t)buffer[i];
    }
}


void CPU::load_font() {
    for (int i = 0; i < 80; i++) {
        mem[i] = font[i];
    }
}


uint16_t CPU::fetch_opcode() {
    opcode = mem[pc] << 8 | mem[pc + 1];
    /* printf("%04x %04x \n", pc, opcode); // tests for correctness */
    pc += 2;

    return opcode;
}


void CPU::decode_execute(uint16_t opcode) {
    // bitmask
    uint16_t nnn = opcode & 0x0fff;
    uint8_t nn = opcode & 0x00ff;
    uint8_t x = (opcode & 0x0f00) >> 8;
    uint8_t y = (opcode & 0x00f0) >> 4;
    uint8_t n = opcode & 0x000f;

    switch (opcode & 0xf000) {
        case 0x0000:
            switch (opcode & 0x00ff) {
                case 0x00e0:
                    // 00E0: Clears the screen
                    for (int i = 0; i < 64*32; i++) {
                        gfx[i] = 0;
                    }
                    break;
                case 0x00ee:
                    // 00EE: Returns from a subroutine
                    pc = stack[stack_pointer];
                    stack_pointer--;
                    break;
            }
            break;
        case 0x1000:
            // 1NNN: Jumps to address NNN
            pc = nnn;
            break;
        case 0x2000:
            // 2NNN: Calls subroutine at NNN
            stack_pointer++;
            stack[stack_pointer] = pc;
            pc = nnn;
            break;
        case 0x3000:
            // 3XNN: Skips the next instruction if VX equals NN
            if (v[x] == nn) {
                pc += 2;
            }
            break;
        case 0x4000:
            // 4XNN: Skips the next instruction if VX doesn't equal NN
            if (v[x] != nn) {
                pc += 2;
            }
            break;
        case 0x5000:
            // 5XY0: Skips the next instruction if VX equals VY
            if (v[x] == v[y]) {
                pc += 2;
            }
            break;
        case 0x6000:
            // 6XNN: Sets VX to NN
            v[x] = nn;
            break;
        case 0x7000:
            // 7XNN: Adds NN to VX
            v[x] += nn;
            break;
        case 0x8000:
            switch(opcode & 0x000f) {
                case 0x0000:
                    // 8XY0: Sets VX to the value of VY
                    v[x] = v[y];
                    break;

                case 0x0001:
                    // 8XY1: Sets VX to VX or VY
                    v[x] = v[x] | v[y];
                    break;

                case 0x0002:
                    // 8XY2: Sets VX to VX and VY
                    v[x] = v[x] & v[y];
                    break;

                case 0x0003:
                    // 8XY3: Sets VX to VX xor VY
                    v[x] = v[x] ^ v[y];
                    break;

                case 0x0004:
                    // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
                    if (v[y] > (0xff - v[x])) {
                        v[0xf] = 1;
                    } else {
                        v[0xf] = 0;
                    }
                    v[x] += v[y];
                    break;

                case 0x0005:
                    // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    if (v[y] > v[x]) {
                        v[0xf] = 0;
                    } else {
                        v[0xf] = 1;
                    }
                    v[x] -= v[y];
                    break;

                case 0x0006:
                    // 8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift
                    v[0xf] = v[x] & 0x1;
                    v[x] >>= 1;
                    break;

                case 0x0007:
                    // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
                    if (v[x] > v[y]) {
                        v[0xf] = 0;
                    } else {
                        v[0xf] = 1;
                    }
                    v[x] = v[y] - v[x];
                    break;

                case 0x000e:
                    // 8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
                    v[0xf] = v[x] >> 7;
                    v[x] <<= 1;
                    break;
            }
            break;

        case 0x9000:
            // 9XY0: Skips to next instruction if VX doesn't equal VY
            if (v[x] != v[y]) {
                pc += 2;
            }
            break;

        case 0xa000:
            // ANNN: Sets I to the address NNN
            index = nnn;
            break;

        case 0xb000:
            // BNNN: Jumps to the address NNN plus V0
            pc = nnn + v[0];
            break;

        case 0xc000:
            // CXNN: Sets VX to a random number and NN
            v[x] = (rand() % 0xff) & nn;
            break;

        case 0xd000: {
             // todo: DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels
            uint16_t height = n;
            uint16_t pixel;
            v[0xf] = 0;

            for (int yline = 0; yline < height; yline++) {
                pixel = mem[index + yline];
                for (int xline = 0; xline < 8; xline++) {
                    if ((pixel & (0x80 >> xline)) != 0) {
                        if (gfx[(v[x] + xline + ((v[y] + yline) * 64))] == 1) {
                            v[0xf] = 1;
                        }
                        gfx[v[x] + xline + ((v[y] + yline) * 64)] ^= 1;
                        draw_flag = true;
                    }
                }
            }
            break;
        }

        case 0xe000:
            switch (opcode & 0x00ff) {
                case 0x009e:
                    // EX9E: Skips the next instruction if the key stored in VX is pressed
                    if (key[v[x]]) {
                        pc += 2;
                    }
                    break;

                case 0x00a1:
                    // EXA1: Skips the next instruction if the key stored in VX isn't pressed
                    if (!key[v[x]]) {
                        pc += 2;
                    }
                    break;
            }
            break;

        case 0xf000:
            switch(opcode & 0x0ff) {
                case 0x0007:
                    // FX07: Sets VX to the value of the delay timer
                    v[x] = delay_timer;
                    break;

                case 0x000a:
                    // FX0A: A key press is awaited, and then stored in VX
                    v[x] = key[v[x]]; // check correctness
                    break;

                case 0x0015:
                    // FX15: Sets the delay timer to VX
                    delay_timer = v[x];
                    break;

                case 0x0018:
                    // FX18: Sets the sound timer to VX
                    sound_timer = v[x];
                    break;

                case 0x001e:
                    // FX1E: Adds VX to I
                    index += v[x];
                    break;

                case 0x0029:
                    // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
                    index = v[x] * 5;
                    break;

                case 0x0033:
                    // FX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2
                    mem[index] = v[x] / 100;
                    mem[index + 1] = (v[x] / 10) % 10;
                    mem[index + 2] = (v[x] % 100) % 10;
                    break;

                case 0x0055:
                    // FX55: Stores V0 to VX in memory starting at address I
                    for (int i = 0; i <= x; i++) {
                        mem[index + i] = v[i];
                    }
                    break;

                case 0x0065:
                    // FX65: Fills V0 to VX with values from memory starting at address I
                    for (int i = 0; i <= x; i++) {
                        v[i] = mem[index + i];
                    }
                    break;
            }
    }
}


/* void CPU::timer() { */
/*     long current_time = std::chrono::high_resolution_clock::now(); */
/* } */


void CPU::emu_cycle() {
    // fetch decode and execute
    decode_execute(fetch_opcode());

    // update timers
    if (sound_timer > 0) {
        sound_timer--;
    }

    if (delay_timer > 0) {
        delay_timer--;
    }
}


void CPU::debug() {
    std::cout << "pc: " << std::hex << pc;
    std::cout << " opcode: " << std::hex << opcode << std::endl;

}
