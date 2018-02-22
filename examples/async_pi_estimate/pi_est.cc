/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2018 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <cstdlib>
#include "pi_est.h"  // NOLINT(build/include)

/*
Estimate the value of π by using a Monte Carlo method.
Take `points` samples of random x and y values on a
[0,1][0,1] plane. Calculating the length of the diagonal
tells us whether the point lies inside, or outside a
quarter circle running from 0,1 to 1,0. The ratio of the
number of points inside to outside gives us an
approximation of π/4.

See https://en.wikipedia.org/wiki/File:Pi_30K.gif
for a visualization of how this works.
*/

inline int randall(unsigned int *p_seed) {
// windows has thread safe rand()
#ifdef _WIN32
  return rand();  // NOLINT(runtime/threadsafe_fn)
#else
  return rand_r(p_seed);
#endif
}

double Estimate (int points) {
  int i = points;
  int inside = 0;
  unsigned int randseed = 1;

#ifdef _WIN32
  srand(randseed);
#endif

  // unique seed for each run, for threaded use
  unsigned int seed = randall(&randseed);

#ifdef _WIN32
  srand(seed);
#endif

  while (i-- > 0) {
    double x = randall(&seed) / static_cast<double>(RAND_MAX);
    double y = randall(&seed) / static_cast<double>(RAND_MAX);

    // x & y and now values between 0 and 1
    // now do a pythagorean diagonal calculation
    // `1` represents our 1/4 circle
    if ((x * x) + (y * y) <= 1)
      inside++;
  }

  // calculate ratio and multiply by 4 for π
  return (inside / static_cast<double>(points)) * 4;
}
