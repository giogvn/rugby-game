// Standard headers
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "dimension.h"
#include "direction.h"
#include "position.h"
#include "spy.h"
#include "map_finder.h"
#include "map.h"
#include "item.h"
#include "goal.h"
#include "current_round.h"

// Main header
#include "goal.h"

goal_t initialize_goal(position_t att_pos, position_t def_pos, position_t *att_path, direction_t *att_actions, position_t pos_in_map, char *map_data, dimension_t map_dims)
{
    int map_width, map_height;
    map_width = map_dims.width;
    map_height = map_dims.height;

    goal_t goal;

    goal.pos_in_map = pos_in_map;

    direction_t *def_actions_to_att_goal = (direction_t *)malloc(map_width * map_height * sizeof(direction_t));
    position_t *def_path_to_att_goal = (position_t *)malloc(map_width * map_height * sizeof(position_t));

    goal.distance_from_attacker = bfs(att_pos, goal.pos_in_map, map_data, map_dims, att_path, att_actions);
    goal.distance_from_defensor = bfs(def_pos, goal.pos_in_map, map_data, map_dims, def_path_to_att_goal, def_actions_to_att_goal);

    free(def_actions_to_att_goal);
    free(def_path_to_att_goal);
    return goal;
}
