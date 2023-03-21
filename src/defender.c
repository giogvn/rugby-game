// Standard headers
#include <stdio.h>

// Internal headers
#include "dimension.h"
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "defender.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                        PRIVATE STRUCT IMPLEMENTATION                       */
/*----------------------------------------------------------------------------*/

// struct defender {
  // TODO: implement the struct
// };

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(
    position_t defender_position, Spy attacker_spy, void* data) {
  // TODO: unused parameters, remove these lines later
  UNUSED(defender_position);
  UNUSED(attacker_spy);
  Defender defender_data = (Defender) data;
  UNUSED(defender_data);

  // TODO: Implement Defender logic here
  return (direction_t) DIR_LEFT;
}

/*----------------------------------------------------------------------------*/

Defender new_defender(dimension_t dimension, char* map_data){
  // TODO: unused parameters, remove these lines later
  UNUSED(dimension);
  UNUSED(map_data);

  // TODO: Implement Attacker data storage here
  return (Defender) NULL;
}

/*----------------------------------------------------------------------------*/
