#ifndef MAPFINDER_H
#define MAPFINDER_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// Internal headers
#include "position.h"
#include "queue.h"
#include "map.h"

position_t find_player(char player_type, Map map);
int bfs(position_t ini_pos, position_t goal, Map map, position_t *path, direction_t *actions);
position_t find_player(char player_type, Map map);
int **initialize_matrix(int rows, int columns);
position_t **initialize_prev_pos(int rows, int columns);
direction_t **initialize_prev_actions(int rows, int columns);
position_t *initialize_path(int size);
int **fill_matrix(int **matrix, int rows, int columns);
void destroy_visited(int **matrix, int columns);
void destroy_positions(position_t **matrix, int rows);
void destroy_directions(direction_t **matrix, int rows);

int possible_region(char content);
position_t *get_neighbors(position_t pos, direction_t *directions, int size);
position_t find_goal_to_attacker(position_t def_pos, Map map);
void add_action(direction_t new_action, direction_t *actions, dimension_t map_dims);
#endif
