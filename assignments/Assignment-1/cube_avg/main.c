#include <stdio.h>
#include "cube_avg.h"

memory mem;
int main(int argc, char** argv) {
  int step_c = 0;
  double x;
  out _res;

  reset(&mem);
  while (1) {
    step_c = (step_c + 1);
    printf("x = ");
    if ((scanf("%lf", &x)==EOF)) {
      return 0;
    }
    step(x, &_res, &mem);
    printf("cube_avg = %.6f\n", _res.out);
    fflush(stdout);
  };

  return 0;
}
