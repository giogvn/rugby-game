// Standard headers
#include <stdio.h>

// Internal headers
#include "dimension.h"
#include "direction.h"
#include "position.h"
#include "spy.h"

// Main header
#include "attacker.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                        PRIVATE STRUCT IMPLEMENTATION                       */
/*----------------------------------------------------------------------------*/

// struct attacker {
  // TODO: implement the struct
// };


/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_attacker_strategy(
    position_t attacker_position, Spy defender_spy, void* data) {
  // TODO: unused parameters, remove these lines later
  UNUSED(attacker_position);
  UNUSED(defender_spy);
  Attacker attacker_data = (Attacker) data;
  UNUSED(attacker_data);

  // TODO: Implement Attacker logic here
  return (direction_t) DIR_RIGHT;
}

/*----------------------------------------------------------------------------*/

Attacker new_attacker(dimension_t dimension, char* map_data){
  // TODO: unused parameters, remove these lines later
  UNUSED(dimension);
  UNUSED(map_data);

  // TODO: Implement Attacker data storage here
  return (Attacker) NULL;
}

/*----------------------------------------------------------------------------*/
