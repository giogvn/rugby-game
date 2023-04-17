// Standard headers
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Internal headers
#include "position.h"
#include "direction.h"
#include "dimension.h"
#include "field.h"
#include "item.h"
#include "map.h"
#include "spy.h"
#include "map_finder.h"
#include "queue.h"

position_t find_player(char player_type, Map map)
{
    dimension_t map_dims = get_map_dimension(map);
    position_t pos;
    char elem;

    for (int i = 0; i < map_dims.height; i++)
    {
        for (int j = 0; j < map_dims.width; j++)
        {
            pos.i = i;
            pos.j = j;
            elem = get_map_symbol(map, pos);

            if (elem == player_type)
            {
                return pos;
            }
        }
    }
}
int **initialize_matrix(int rows, int columns)
{
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(columns * sizeof(int));
    }
    return matrix;
}

position_t **initialize_prev_pos(int rows, int columns)
{
    position_t **path = (position_t **)malloc(rows * sizeof(position_t *));
    for (int i = 0; i < rows; i++)
    {
        path[i] = (position_t *)malloc(columns * sizeof(position_t));
    }
    return path;
}

direction_t **initialize_prev_actions(int rows, int columns)
{
    direction_t **actions = (direction_t **)malloc(rows * sizeof(direction_t *));
    for (int i = 0; i < rows; i++)
    {
        actions[i] = (direction_t *)malloc(columns * sizeof(direction_t));
    }
    return actions;
}

position_t *initialize_path(int size)
{
    position_t *path = (position_t *)malloc(size * sizeof(position_t));
}

int **fill_matrix(int **matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

void destroy_matrix(int **matrix, int columns, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}
int possible_region(char content)
{
    return (content != 'X');
}

position_t *get_neighbors(position_t pos, direction_t *directions)
{
    position_t *neighbors = (position_t *)malloc(7 * sizeof(position_t));
    for (int i = 0; i < 7; i++)
    {
        neighbors[i] = move_position(pos, directions[i]);
    }

    return neighbors;
}

int bfs(position_t ini_pos, position_t goal, Map map, position_t *path, direction_t *actions)
{
    dimension_t map_dims = get_map_dimension(map);
    int map_width = map_dims.width;

    int map_height = map_dims.height;
    int **visited = initialize_matrix(map_height, map_width);
    visited = fill_matrix(visited, map_height, map_width);
    position_t **prev_pos = initialize_prev_pos(map_height, map_width);
    direction_t **prev_actions = initialize_prev_actions(map_height, map_width);
    prev_pos[ini_pos.i][ini_pos.j] = ini_pos;

    int distance = 0;
    position_t *neighbors;
    position_t neighbor;
    direction_t directions[8] = {DIR_UP,
                                 DIR_UP_RIGHT,
                                 DIR_RIGHT,
                                 DIR_DOWN_RIGHT,
                                 DIR_DOWN,
                                 DIR_DOWN_LEFT,
                                 DIR_UP_LEFT,
                                 DIR_STAY

    };
    prev_actions[ini_pos.i][ini_pos.j] = directions[7];
    char neighbor_content;
    queue_t q;
    init_queue(&q);
    enqueue(&q, ini_pos);
    visited[ini_pos.i][ini_pos.j] = true;
    while (!is_empty(&q))
    {
        position_t current = dequeue(&q);
        if (equal_positions(current, goal))
        {
            // Build the path by following the previous positions
            int path_length = 0;
            position_t pos = goal;
            direction_t action;
            while (!equal_positions(pos, ini_pos))
            {
                action = prev_actions[pos.i][pos.j];
                path[path_length] = pos;
                actions[path_length] = action;
                path_length++;
                pos = prev_pos[pos.i][pos.j];
            }
            path[path_length] = ini_pos;
            actions[path_length] = prev_actions[ini_pos.i][ini_pos.j];
            destroy_matrix(prev_pos, map_height, map_width);
            destroy_matrix(prev_actions, map_height, map_width);
            destroy_matrix(visited, map_height, map_width);
            destroy_queue(&q);

            return path_length;
        }
        neighbors = get_neighbors(current, directions);
        // Check the neighbors of the current position
        for (int i = 0; i < 7; i++)
        {
            neighbor = neighbors[i];
            // Compute the neighbor position
            int neighbor_row = neighbor.i;
            int neighbor_col = neighbor.j;
            neighbor_content = get_map_symbol(map, neighbor);
            // Check if the neighbor position is valid and unvisited
            if (neighbor_row >= 0 && neighbor_row < map_height &&
                neighbor_col >= 0 && neighbor_col < map_width &&
                !visited[neighbor_row][neighbor_col] &&
                possible_region(neighbor_content))
            {

                // Enqueue the neighbor position and mark it as visited
                position_t neighbor = {neighbor_row, neighbor_col};
                enqueue(&q, neighbor);
                visited[neighbor_row][neighbor_col] = true;
                prev_pos[neighbor_row][neighbor_col] = current;
                prev_actions[neighbor_row][neighbor_col] = directions[i];
            }
        }
        distance++;
        free(neighbors);
    }
    destroy_matrix(visited, map_width, map_height);
    destroy_queue(&q);
    return -1;
}

int main()
{
    Map map = new_map("/home/giovani/bcc/2023.1/mac218/miniep3/rugby-game/data/not_so_simple.map");
    position_t start, goal;
    start = find_player('A', map);
    goal.i = 1;
    goal.j = 8;
    dimension_t map_dims = get_map_dimension(map);
    int map_width = map_dims.width;

    int map_height = map_dims.height;
    position_t *path = initialize_path(map_width * map_height);
    direction_t *actions = (direction_t *)malloc(map_width * map_height * sizeof(direction_t));
    int dist = bfs(start, goal, map, path, actions);
    delete_map(map);
    position_t pos;
    printf("The path has size %d\n", dist);
    for (int i = 0; i < dist; i++)
    {
        pos = path[i];
        // printf("%ld %ld\n", pos.i, pos.j);
        printf("%d %d \n", actions[i].i, actions[i].j);
    }
    free(path);
    free(actions);
    return 0;
}
