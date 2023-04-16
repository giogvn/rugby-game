#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// Internal headers
#include "position.h"
#include "queue.h"
#include "map.h"

position_t find_player(char player_type, Map map);
int bfs(position_t ini_pos, position_t goal, Map map);
#endif
