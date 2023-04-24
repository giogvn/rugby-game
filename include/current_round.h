#ifndef CURRENTROUND_H
#define CURRENTROUND_H

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

// Internal headers
#include "position.h"

struct current_round
{
    position_t *current_attacker_path;
    position_t *current_defensor_path;
    int n_curr_round;
    int rounds_left;
};

typedef struct current_round current_round_t;

#endif
