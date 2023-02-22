#include <iostream>
#include "dssemble.h"

void Dssemble::disassemble(uint8_t* codebuff, int pc) {
    uint8_t *code = &codebuff[pc];
    uint8_t firstnib = (code[0] >> 4); // 1NNN = 1 (eg. we know instruction starts with 1)

    printf("%04x %02x %02x", pc, code[0], code[1]); // [0], [1] hold instruction at pc address

    uint8_t rmvlh = code[0] & 0x0f;
    uint8_t rmvlh2 = code[1] & 0x0f;

    switch(firstnib) {
        case 0x00:
        {
            switch(code[1]) {
                case 0xe0: std::cout << " clear screen!"; break;
                case 0xee: std::cout << " return from subroutine"; break;
            }
        } break;
        case 0x01: printf(" jmp to address %01x%02x", rmvlh, code[1]); break;
        case 0x02: printf(" call subroutine at adress %01x%02x", rmvlh, code[1]); break;
        case 0x03: printf(" if (v%01x != %02x)", rmvlh, code[1]); break;
        case 0x04: printf(" if (v%01x == %02x)", rmvlh, code[1]); break;
        case 0x05: printf(" if (v%01x != v%01x)", rmvlh, rmvlh2); break;
        case 0x06: printf(" v%01x = %d", rmvlh, code[1]); break;
        case 0x07: printf(" v%01x += %d", rmvlh, code[1]); break;
        case 0x08:
        {
            switch((code[1] & 0x0f)) {
                case 0x00: printf(" v%01x = v%01x", rmvlh, (code[1] & 0xf0)); break;
                case 0x01: printf(" v%01x |= v%01x", rmvlh, (code[1] & 0xf0)); break;
                case 0x02: printf(" v%01x &= v%01x", rmvlh, (code[1] & 0xf0)); break;
                case 0x03: printf(" v%01x ^= v%01x", rmvlh, (code[1] & 0xf0)); break;
                case 0x04: printf(" v%01x += v%01x", rmvlh, (code[1] & 0xf0)); break;
                case 0x05: printf(" v%01x -= v%01x", rmvlh, (code[1] & 0xf0)); break;
                case 0x06: printf(" v%01x >>= v%01x", rmvlh, (code[1] & 0xf0)); break;
                case 0x07: printf(" v%01x = v%01x - V%01x", rmvlh, (code[1] & 0xf0), (code[1] & 0xf0)); break;
                case 0x0e: printf(" v%01x <<= v%01x", rmvlh, (code[1] & 0xf0)); break;
            }
        }; break;
        case 0x09: printf( " if (v%01x == v%01x)", rmvlh, (code[1] & 0xf0)); break;
        case 0x0a: printf(" mem index register = address %01x%02x", rmvlh, code[1]); break;
        case 0x0b: printf(" jmp to address %01x%02x + v0", rmvlh, code[1]); break;
        case 0x0c: printf(" v%01x = random num(0-255) AND %d", rmvlh, code[1]); break;
        case 0x0d: printf(" sprite v%01x, v%01x, %d", rmvlh, (code[1] & 0xf0), rmvlh2); break;
        case 0x0e:
            {
                switch(code[1]) {
                    case 0x9e: std::cout << " key not pressed?"; break;
                    case 0xa1: std::cout << " key pressed?"; break;
                }
            }; break;
        case 0x0f:
        {
            switch(code[1]) {
                case 0x07: printf(" v%01x = delay timer", rmvlh); break;
                case 0x0a: printf(" v%01x = key press", rmvlh); break;
                case 0x15: printf(" delay timer = v%01x", rmvlh); break;
                case 0x18: printf(" sound timer = v%01x", rmvlh); break;
                case 0x1e: printf(" mem index register += v%01x", rmvlh); break;
                case 0x29: printf(" mem index register = sprite location of v%01x", rmvlh); break;
                case 0x33: printf(" bcd of v%01x", rmvlh); break;
                case 0x55: printf(" save v%01x", rmvlh); break;
                case 0x65: printf(" load v%01x", rmvlh); break;
            }
        }; break;
    }
}

void run_disassembler(char* d) {

    Dssemble ds;

    FILE *f= fopen(d, "rb");
    if (f==NULL) {
        printf("error: Couldn't open %s\n", d);
        exit(1);
    }

    //Get the file size
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    std::cout << "File size: " << fsize << std::endl;

    //CHIP-8 convention puts programs in memory at 0x200
    // They will all have hardcoded addresses expecting that
    //
    //Read the file into memory at 0x200 and close it.
    uint8_t* buffer = new uint8_t[(uint8_t)fsize+0x200]; // f lded to array (index holds 2 hex digits)
    fread(buffer+0x200, (size_t)fsize, 1, f);            // instructions 2 bytes long (index 1,2 will hold 1 full instruction)
    fclose(f);

    int pc = 0x200;
    while (pc < (fsize+0x200))
    {
        ds.disassemble(buffer, pc);
        pc += 2;
        printf("\n");
    }
}
