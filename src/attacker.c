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
#include "game.h"

// Main header
#include "attacker.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter
#define SAFETY_THRESHOLD 1
/*----------------------------------------------------------------------------*/
/*                        PRIVATE STRUCT IMPLEMENTATION                       */
/*----------------------------------------------------------------------------*/

struct attacker
{
  char *map_data;
  dimension_t map_dims;
  current_round_t curr_round;
  position_t *curr_path;
  position_t *curr_def_path;
  direction_t *curr_actions;
  goal_t curr_goal;
  position_t cross_pos;
  int distance_to_cross;
  int distance_to_goal;
  int index_curr_decision;
  bool spied_last;
};

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_attacker_strategy(
    position_t attacker_position, Spy defender_spy, void *data)
{
  Attacker att = (Attacker)data;

  char *map_data;
  map_data = att->map_data;

  dimension_t map_dims = att->map_dims;
  if (att->distance_to_cross < SAFETY_THRESHOLD && att->curr_round.rounds_left > 1 && !att->spied_last)
  {
    att->spied_last = true;
    direction_t dir_stay = DIR_STAY;
    goal_t new_goal;
    position_t def_new_pos = get_spy_position(defender_spy);
    position_t new_goal_pos = find_goal_to_attacker(att->curr_round.rounds_left, def_new_pos, map_data, map_dims, att->curr_goal.pos_in_map);
    new_goal = initialize_goal(attacker_position, def_new_pos, att->curr_path, att->curr_actions, new_goal_pos, map_data, map_dims);
    direction_t back_dir = backwards_dir(attacker_position, map_data, map_dims);

    position_t new_cross = find_closest_position_in_path(att->curr_round.rounds_left, def_new_pos, att->curr_path, att->index_curr_decision, att->curr_def_path, new_goal.distance_from_attacker, map_data, map_dims);
    att->cross_pos = new_cross;
    int distance_to_cross = distance_in_path(att->index_curr_decision, new_cross, att->curr_path, new_goal.distance_from_attacker);
    // This means the attacker can go backwards
    if (back_dir.i != -1 && new_goal.distance_from_attacker <= att->curr_round.rounds_left)
    {
      position_t att_back_pos = move_position(attacker_position, back_dir);
      new_goal = initialize_goal(att_back_pos, def_new_pos, att->curr_path, att->curr_actions, new_goal_pos, map_data, map_dims);
      add_action(dir_stay, att->curr_actions, map_dims);
      add_action(back_dir, att->curr_actions, map_dims);
      distance_to_cross++;
      new_goal.distance_from_attacker++;
    }
    else
    {
      add_action(dir_stay, att->curr_actions, map_dims);
    }
    att->curr_goal = new_goal;
    att->distance_to_cross = distance_to_cross;
    att->index_curr_decision = 0;
  }
  else
  {
    att->curr_goal.distance_from_attacker--;
    att->spied_last = false;
  }
  direction_t action = att->curr_actions[att->index_curr_decision];
  att->index_curr_decision++;
  att->curr_round.rounds_left--;
  att->distance_to_cross--;
  return action;
}

/*----------------------------------------------------------------------------*/

Attacker new_attacker(dimension_t dimension, char *map_data)
{

  // Map curr_map = new_map(map_data);
  position_t att_pos, def_pos;
  att_pos = find_player('A', map_data, dimension);
  def_pos = find_player('D', map_data, dimension);

  int map_width = dimension.width;
  int map_height = dimension.height;

  dimension_t map_dims;
  map_dims.height = map_height;
  map_dims.width = map_width;

  // Attacker's struct data
  current_round_t curr_round;
  position_t *curr_path = initialize_path(map_width * map_height);
  position_t *curr_def_path = initialize_path(map_width * map_height);
  goal_t curr_goal;
  direction_t *curr_actions = (direction_t *)malloc(map_width * map_height * sizeof(direction_t));
  int distance_to_cross;

  // Data necessary to fill Attacker's struct

  position_t last_goal;
  last_goal.i = -1;
  last_goal.j = -1;

  position_t goal_to_att = find_goal_to_attacker(MAX_ROUNDS, def_pos, map_data, map_dims, last_goal);
  curr_goal = initialize_goal(att_pos, def_pos, curr_path, curr_actions, goal_to_att, map_data, map_dims);
  curr_round.current_attacker_path = curr_path;
  curr_round.current_defensor_path = curr_def_path;
  curr_round.rounds_left = MAX_ROUNDS;
  curr_round.n_curr_round = 1;

  position_t cross_pos = find_closest_position_in_path(curr_round.rounds_left, def_pos, curr_path, 0, curr_def_path, curr_goal.distance_from_attacker, map_data, map_dims);
  distance_to_cross = distance_in_path(0, cross_pos, curr_path, curr_goal.distance_from_attacker);
  Attacker att = malloc(sizeof(struct attacker));

  att->cross_pos = cross_pos;
  att->map_dims = map_dims;
  att->map_data = map_data;
  att->curr_round = curr_round;
  att->curr_path = curr_path;
  att->curr_def_path = curr_def_path;
  att->curr_actions = curr_actions;
  att->curr_goal = curr_goal;
  att->index_curr_decision = 0;
  att->distance_to_cross = distance_to_cross;
  att->spied_last = true;
  return (Attacker)att;
}

/*int main()
{
  Map map = new_map("/home/giovani/bcc/2023.1/mac218/miniep3/rugby-game/data/simple.map");
  dimension_t map_dims = get_map_dimension(map);
  Attacker att = new_attacker(map_dims, "/home/giovani/bcc/2023.1/mac218/miniep3/rugby-game/data/simple.map");

  position_t att_pos = find_player('A', map);
  position_t def_pos = find_player('D', map);
  Item it = new_item('D', true);
  Spy defender_spy = new_spy(it);
  set_spy_position(defender_spy, def_pos);
  int dist_to_goal = att->curr_goal.distance_from_attacker;
  int action_index = att->index_curr_decision;
  direction_t action;

  for (int i = 0; i < att->curr_goal.distance_from_attacker; i++)
  {
    printf("%ld %ld\n", att->curr_path[i].i, att->curr_path[i].j);
  }
  printf("\n");
  printf("Actions to goal\n");
  for (int i = 0; i < att->curr_goal.distance_from_attacker; i++)
  {
    printf("%d %d\n", att->curr_actions[i].i, att->curr_actions[i].j);
  }
  printf("\n");
  printf("Cross position %ld %ld\n", att->cross_pos.i, att->cross_pos.j);
  printf("Distance to cross position %d\n", att->distance_to_cross);
  while (dist_to_goal > 0)
  {
    printf("Position %ld %ld\n", att_pos.i, att_pos.j);
    action = execute_attacker_strategy(att_pos, defender_spy, att);
    att_pos = move_position(att_pos, action);
    printf("Action %d %d\n", att->curr_actions[action_index].i, att->curr_actions[action_index].j);
    dist_to_goal = att->curr_goal.distance_from_attacker;
    action_index = att->index_curr_decision;
  }
  printf("Position %ld %ld\n", att_pos.i, att_pos.j);
}*/

/*----------------------------------------------------------------------------*/
