#include "cube_avg.h"
#include <stdio.h>

void reset(memory *self) {
    self->freq_cubes = 0;
    self->sum_cubes = 0;
}

void step(int x, out *_out, memory *self) {
    long long cube = x * x * x;
    self->freq_cubes++;
    self->sum_cubes += cube;

    _out->out = (self->sum_cubes * 1.0 / self->freq_cubes);
}
