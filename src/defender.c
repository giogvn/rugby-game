// Standard headers
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "dimension.h"
#include "direction.h"
#include "position.h"
#include "spy.h"
#include "goal.h"
#include "current_round.h"
#include "map.h"
#include "map_finder.h"
// Main header
#include "defender.h"

// Macros
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter
#define SAFETY_THRESHOLD 2

/*----------------------------------------------------------------------------*/
/*                        PRIVATE STRUCT IMPLEMENTATION                       */
/*----------------------------------------------------------------------------*/

struct defender
{
  char *map_data;
  dimension_t map_dims;
  current_round_t curr_round;
  position_t *curr_def_path;
  position_t *curr_att_path;
  direction_t *curr_actions;
  direction_t *curr_att_actions;
  position_t curr_att_goal;
  position_t cross_pos;
  int distance_to_cross;
  int curr_att_path_size;
  int index_curr_decision;
  bool spied_last;
};

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

direction_t execute_defender_strategy(
    position_t defender_position, Spy attacker_spy, void *data)
{
  Defender def = (Defender)data;
  char *map_data;
  map_data = def->map_data;
  dimension_t map_dims = def->map_dims;
  if (def->distance_to_cross < SAFETY_THRESHOLD && !def->spied_last)
  {
    direction_t dir_stay = DIR_STAY;
    goal_t new_att_goal;
    position_t att_new_pos = get_spy_position(attacker_spy);
    position_t att_new_goal_pos = find_goal_to_attacker(def->curr_round.rounds_left, defender_position, map_data, map_dims, def->curr_att_goal);

    new_att_goal = initialize_goal(att_new_pos, defender_position, def->curr_att_path, def->curr_att_actions, att_new_goal_pos, map_data, map_dims);

    position_t new_cross = find_closest_position_in_path(def->curr_round.rounds_left, defender_position, def->curr_att_path, def->index_curr_decision, def->curr_def_path, new_att_goal.distance_from_attacker, map_data, map_dims);
    def->distance_to_cross = bfs(defender_position, new_cross, map_data, map_dims, def->curr_def_path, def->curr_actions);
    add_action(dir_stay, def->curr_actions, map_dims);
    def->index_curr_decision = 0;
    def->curr_att_goal = att_new_goal_pos;
    def->cross_pos = new_cross;
    def->curr_att_path_size = new_att_goal.distance_from_attacker;
    def->index_curr_decision = 0;
    def->spied_last = true;
  }
  else
  {
    def->spied_last = false;
  }
  direction_t action = def->curr_actions[def->index_curr_decision];
  def->index_curr_decision++;
  def->curr_round.rounds_left--;
  def->distance_to_cross--;
  return action;
}

/*----------------------------------------------------------------------------*/

Defender new_defender(dimension_t dimension, char *map_data)
{
  {

    dimension_t map_dims = dimension;
    position_t att_pos, def_pos;
    att_pos = find_player('A', map_data, map_dims);
    def_pos = find_player('D', map_data, map_dims);

    int map_width = dimension.width;
    int map_height = dimension.height;

    map_dims.height = map_height;
    map_dims.width = map_width;

    // Defender's struct data
    current_round_t curr_round;
    position_t *curr_path = initialize_path(map_width * map_height);
    position_t *curr_def_path = initialize_path(map_width * map_height);
    direction_t *curr_att_actions = (direction_t *)malloc(map_width * map_height * sizeof(direction_t));
    goal_t curr_goal;
    direction_t *curr_actions = (direction_t *)malloc(map_width * map_height * sizeof(direction_t));
    int distance_to_cross;

    // Data necessary to fill Defender's struct

    position_t last_goal;
    last_goal.i = -1;
    last_goal.j = -1;

    position_t curr_att_goal = find_goal_to_attacker(MAX_ROUNDS, def_pos, map_data, map_dims, last_goal);
    curr_goal = initialize_goal(att_pos, def_pos, curr_path, curr_att_actions, curr_att_goal, map_data, map_dims);

    curr_round.current_attacker_path = curr_path;
    curr_round.current_defensor_path = curr_def_path;
    curr_round.rounds_left = MAX_ROUNDS;
    curr_round.n_curr_round = 1;

    position_t def_goal_pos = find_closest_position_in_path(curr_round.rounds_left, def_pos, curr_path, 0, curr_def_path, curr_goal.distance_from_attacker, map_data, map_dims);
    distance_to_cross = bfs(def_pos, def_goal_pos, map_data, map_dims, curr_def_path, curr_actions);
    Defender def = malloc(sizeof(struct defender));

    def->map_data = map_data;
    def->map_dims = map_dims;
    def->curr_actions = curr_actions;
    def->curr_att_actions = curr_att_actions;
    def->curr_att_path = curr_path;
    def->curr_def_path = curr_def_path;
    def->curr_round = curr_round;
    def->curr_att_goal = curr_att_goal;
    def->index_curr_decision = 0;
    def->distance_to_cross = distance_to_cross;
    def->spied_last = true;

    return (Defender)def;
  }
}

/*int main()
{
  Map map = new_map("/home/giovani/bcc/2023.1/mac218/miniep3/rugby-game/data/simple.map");
  dimension_t map_dims = get_map_dimension(map);
  Defender def = new_defender(map_dims, "/home/giovani/bcc/2023.1/mac218/miniep3/rugby-game/data/simple.map");

  position_t att_pos = find_player('A', map);
  position_t def_pos = find_player('D', map);
  Item it = new_item('A', true);
  Spy attacker_spy = new_spy(it);
  set_spy_position(attacker_spy, def_pos);
  int dist_to_goal = def->distance_to_cross;
  int action_index = def->index_curr_decision;
  direction_t action;

  for (int i = 0; i < def->distance_to_cross; i++)
  {
    printf("%ld %ld\n", def->curr_def_path[i].i, def->curr_def_path[i].j);
  }
  printf("\n");
  printf("Actions to goal\n");
  for (int i = 0; i < def->distance_to_cross; i++)
  {
    printf("%d %d\n", def->curr_actions[i].i, def->curr_actions[i].j);
  }
  printf("\n");
  printf("Cross position %ld %ld\n", def->cross_pos.i, def->cross_pos.j);
  printf("Distance to cross position %d\n", def->distance_to_cross);
  while (dist_to_goal > 0)
  {
    printf("Position %ld %ld\n", def_pos.i, def_pos.j);
    action = execute_defender_strategy(def_pos, attacker_spy, def);
    def_pos = move_position(def_pos, action);
    printf("Action %d %d\n", def->curr_actions[action_index].i, def->curr_actions[action_index].j);
    dist_to_goal = def->distance_to_cross;
    action_index = def->index_curr_decision;
  }
  printf("Position %ld %ld\n", def_pos.i, def_pos.j);
}*/

/*----------------------------------------------------------------------------*/
