// Standard headers
#include <stdbool.h>
#include <stdio.h>

// Main header
#include "direction.h"

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

bool equal_directions(direction_t d1, direction_t d2) {
  return d1.i == d2.i && d1.j == d2.j;
}

/*----------------------------------------------------------------------------*/
