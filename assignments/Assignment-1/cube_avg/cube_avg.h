typedef struct memory {
    long long sum_cubes;
    long long freq_cubes;
} memory;

typedef struct out {
    double out;
} out;

void reset(memory *self);

void step(int x, out *_out, memory *self);
