#ifndef GOAL_H
#define GOAL_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// Internal headers
#include "position.h"
#include "map.h"

struct goal
{
    position_t pos_in_map;
    int distance_from_attacker;
    int distance_from_defensor;
};

typedef struct goal goal_t;

void update_distance_to_player(char player_type, goal_t goal);
goal_t initialize_goal(position_t att_pos, position_t def_pos, position_t *att_path, direction_t *att_actions, position_t pos_in_map, char *map_data, dimension_t map_dims);
#endif
