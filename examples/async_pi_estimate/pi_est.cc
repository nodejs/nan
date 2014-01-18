/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors
 *
 * MIT +no-false-attribs License <https://github.com/rvagg/nan/blob/master/LICENSE>
 **********************************************************************************/

#include <cstdlib>
#include "./pi_est.h"

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

double Estimate (int points) {
  int i = points;
  int inside = 0;
  unsigned int randseed = 1;
  // unique seed for each run, for threaded use
  unsigned int seed = rand_r(&randseed);

  while (i-- > 0) {
    // rand_r() is used to avoid thread locking
    double x = rand_r(&seed) / static_cast<double>(RAND_MAX);
    double y = rand_r(&seed) / static_cast<double>(RAND_MAX);

    // x & y and now values between 0 and 1
    // now do a pythagorean diagonal calculation
    // `1` represents our 1/4 circle
    if ((x * x) + (y * y) <= 1)
      inside++;
  }

  // calculate ratio and multiply by 4 for π
  return (inside / static_cast<double>(points)) * 4;
}
