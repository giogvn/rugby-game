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

// Main header
#include "attacker.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                        PRIVATE STRUCT IMPLEMENTATION                       */
/*----------------------------------------------------------------------------*/

struct attacker
{
  int action_index;
  direction_t *actions_to_goal;
  position_t def_pos;
  position_t *path_to_goal;
  direction_t *actions_to_def;
  position_t *path_to_def;
  int dist_to_goal;
  int dist_to_def;
  Map map;
};

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_attacker_strategy(
    position_t attacker_position, Spy defender_spy, void *data)
{
  direction_t action;
  position_t *neighbors;
  position_t neighbor_behind;
  int i = -1;
  bool can_go_back = false;
  direction_t behind[3] = {DIR_LEFT, DIR_DOWN_LEFT, DIR_UP_LEFT};
  Attacker attacker_data = (Attacker)data;
  Map map = attacker_data->map;
  attacker_data->dist_to_def = bfs(attacker_position, attacker_data->def_pos, map, attacker_data->path_to_def, attacker_data->actions_to_def);

  if (attacker_data->dist_to_def < attacker_data->dist_to_goal)
  {
    action = (direction_t)DIR_STAY;
    attacker_data->def_pos = get_spy_position(defender_spy);
    neighbors = get_neighbors(attacker_position, behind, 3);
    while (!can_go_back && i < 3)
    {
      i++;
      neighbor_behind = neighbors[i];
      char neighbor_content = get_map_symbol(attacker_data->map, neighbor_behind);
      can_go_back = possible_region(neighbor_content);
    }
    free(neighbors);
    if (can_go_back)
    {
      attacker_position = move_position(attacker_position, behind[i]);
    }
    position_t new_goal = find_goal_to_attacker(attacker_data->def_pos, map);
    attacker_data->dist_to_goal = bfs(attacker_position, new_goal, map, attacker_data->path_to_goal, attacker_data->actions_to_goal);

    if (can_go_back)
    {
      dimension_t map_dims = get_map_dimension(map);
      add_action(behind[i], attacker_data->actions_to_goal, map_dims);
      attacker_data->dist_to_goal++;
    }
    attacker_data->action_index = 0;
  }
  else
  {
    int i = attacker_data->action_index;
    action = attacker_data->actions_to_goal[i];
    attacker_data->dist_to_goal--;
  }
  attacker_data->dist_to_goal--;
  attacker_data->action_index++;
  attacker_data->dist_to_def = bfs(attacker_position, attacker_data->def_pos, map, attacker_data->path_to_def, attacker_data->actions_to_def);

  return action;
}

/*----------------------------------------------------------------------------*/

Attacker new_attacker(dimension_t dimension, char *map_data)
{
  Map map = new_map(map_data);
  position_t start, goal, oponent;
  start = find_player('A', map);
  oponent = find_player('D', map);
  goal = find_goal_to_attacker(oponent, map);
  int map_width = dimension.width;
  int map_height = dimension.height;
  position_t *path_to_goal = initialize_path(map_width * map_height);
  direction_t *actions_to_goal = (direction_t *)malloc(map_width * map_height * sizeof(direction_t));
  position_t *path_to_def = initialize_path(map_width * map_height);
  direction_t *actions_to_def = (direction_t *)malloc(map_width * map_height * sizeof(direction_t));
  int dist_to_goal = bfs(start, goal, map, path_to_goal, actions_to_goal);
  int dist_to_oponent = bfs(start, oponent, map, path_to_def, actions_to_def);

  Attacker atckr = malloc(sizeof(struct attacker));

  atckr->def_pos = oponent;
  atckr->action_index = 0;
  atckr->actions_to_goal = actions_to_goal;
  atckr->path_to_goal = path_to_goal;
  atckr->actions_to_def = actions_to_def;
  atckr->path_to_def = path_to_def;
  atckr->path_to_goal = path_to_goal;
  atckr->dist_to_goal = dist_to_goal;
  atckr->dist_to_def = dist_to_oponent;
  atckr->map = map;
  return (Attacker)atckr;
}

int main()
{
  Map map = new_map("/home/giovani/bcc/2023.1/mac218/miniep3/rugby-game/data/simple.map");
  dimension_t map_dims = get_map_dimension(map);
  Attacker atckr = new_attacker(map_dims, "/home/giovani/bcc/2023.1/mac218/miniep3/rugby-game/data/simple.map");

  position_t atckr_pos = find_player('A', map);
  position_t def_pos = find_player('D', map);
  Item it = new_item('D', true);
  Spy defender_spy = new_spy(it);
  set_spy_position(defender_spy, def_pos);
  int dist_to_goal = atckr->dist_to_goal;
  int action_index = atckr->action_index;
  direction_t action;
  while (dist_to_goal > 0)
  {
    printf("Dist to goal %d\n", atckr->dist_to_goal);
    action = execute_attacker_strategy(atckr_pos, defender_spy, atckr);
    atckr_pos = move_position(atckr_pos, action);
    printf("Action %d %d\n", atckr->actions_to_goal[action_index].i, atckr->actions_to_goal[action_index].j);
    dist_to_goal = atckr->dist_to_goal;
    action_index = atckr->action_index;
  }
}

/*----------------------------------------------------------------------------*/
