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
int **initialize_matrix(int columns, int rows)
{
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(columns * sizeof(int));
    }
}
int bfs(position_t ini_pos, position_t goal, Map map)
{
    int grid_width = abs(goal.j - ini_pos.j);
    int grid_height = abs(goal.i - goal.i);
    int **visited = (int **)malloc(grid_height * sizeof(int *));
    queue_t *q;
}
