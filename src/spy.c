// Standard headers
#include <stdlib.h>

// Internal headers
#include "item.h"

// Main header
#include "spy.h"

/*----------------------------------------------------------------------------*/
/*                        PRIVATE STRUCT IMPLEMENTATION                       */
/*----------------------------------------------------------------------------*/

struct spy {
  Item item;
  position_t position;
};

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

Spy new_spy(Item item) {
  Spy spy = malloc(sizeof(*spy));

  spy->item = item;
  spy->position = (position_t) {0, 0};

  return spy;
}

/*----------------------------------------------------------------------------*/

void delete_spy(Spy spy) {
  if (spy == NULL) return;

  spy->position = (position_t) {0, 0};
  spy->item = NULL;

  free(spy);
}

/*----------------------------------------------------------------------------*/

position_t get_spy_position(Spy spy) {
  if (spy == NULL) return (position_t) INVALID_POSITION;

  return spy->position;
}

/*----------------------------------------------------------------------------*/

void set_spy_position(Spy spy, position_t position) {
  if (spy == NULL) return;

  spy->position = position;
}

/*----------------------------------------------------------------------------*/

Item get_spy_item(Spy spy) {
  if (spy == NULL) return 0;

  return spy->item;
}
