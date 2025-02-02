typedef struct memory {
    int first_element;
    int freq;
} memory;

typedef struct out {
    int out;
} out;

void reset(memory *self);

void step(int x, out *_out, memory *self);
