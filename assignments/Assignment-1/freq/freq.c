#include "freq.h"
#include <stdio.h>

void reset(memory *self) {
    self->freq = 0;
    self->first_element = 0;
}

void step(int x, out *_out, memory *self) {
    if (self->freq == 0)
        self->first_element = x;
    if (self->first_element == x)
        self->freq++;
    _out->out = self->freq;
}
