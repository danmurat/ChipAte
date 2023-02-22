#ifndef DSSEMBLE_H
#define DSSEMBLE_H

#include <iostream>

void run_disassembler(char*);

class Dssemble {
    /* uint8_t *code; */
    /* int pc; */
    /* uint8_t firstnib; */

public:
    void disassemble(uint8_t *codebuffer, int pc);
};

#endif
