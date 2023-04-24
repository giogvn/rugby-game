#ifndef MAPFINDER_H
#define MAPFINDER_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// Internal headers
#include "position.h"
#include "queue.h"
#include "map.h"

#define MAX_ROUNDS 42

position_t find_player(char player_type, char *map_data, dimension_t map_dims);
int bfs(position_t ini_pos, position_t goal, char *map_data, dimension_t map_dims, position_t *path, direction_t *actions);
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
position_t find_goal_to_attacker(int curr_round, position_t def_pos, char *map_data, dimension_t map_dims, position_t last_goal);
void add_action(direction_t new_action, direction_t *actions, dimension_t map_dims);
position_t find_closest_position_in_path(int rounds_left, position_t from, position_t *path, int index_path, position_t *final_path, int path_size, char *map_data, dimension_t map_dims);
direction_t backwards_dir(position_t pos, char *map_data, dimension_t map_dims);
int distance_in_path(int index_from, position_t dest, position_t *path, int path_size);
#endif
