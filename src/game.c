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

#include "attacker.h"
#include "defender.h"

// Main header
#include "game.h"

// Macros
#define MAX_SINGLE_OCCURRENCE 1UL
#define UNUSED(x) (void)(x) // Auxiliary to avoid error of unused parameter

/*----------------------------------------------------------------------------*/
/*                        PRIVATE STRUCT IMPLEMENTATION                       */
/*----------------------------------------------------------------------------*/

struct game
{
  Field field;

  PlayerStrategy execute_attacker_strategy;
  PlayerStrategy execute_defender_strategy;

  Item attacker;
  Item defender;
  Item obstacle;

  Spy attacker_spy;
  Spy defender_spy;

  Attacker attacker_data;
  Defender defender_data;
};

/*----------------------------------------------------------------------------*/
/*                          PRIVATE FUNCTIONS HEADERS                         */
/*----------------------------------------------------------------------------*/

Game allocate_game(
    dimension_t field_dimension,
    PlayerStrategy execute_attacker_strategy,
    PlayerStrategy execute_defender_strategy);

bool has_map_exceeded_max_occurrences_of_symbol(
    Map map, char symbol, size_t max_occurrences);
void set_item_in_field_from_map(Field field, Item item, Map map);

void set_attacker_in_field(Field field, Item attacker);
void set_defender_in_field(Field field, Item defender);
void set_obstacles_in_field(Field field, Item obstacle);

bool has_spy_exceeded_max_number_uses(Spy opponent_spy,
                                      size_t max_number_spies);
bool has_defender_captured_attacker(Item defender, Item attacker);
bool has_attacker_arrived_end_field(Field field, Item attacker);

void move_item(Field field,
               Item item,
               Spy opponent_spy,
               PlayerStrategy execute_item_strategy,
               void *data);

/*----------------------------------------------------------------------------*/
/*                              PUBLIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

Game new_game(
    dimension_t field_dimension,
    PlayerStrategy execute_attacker_strategy,
    PlayerStrategy execute_defender_strategy)
{
  Game game = allocate_game(
      field_dimension,
      execute_attacker_strategy,
      execute_defender_strategy);

  set_obstacles_in_field(game->field, game->obstacle);

  char *field_string = string_field(game->field);
  game->attacker_data = new_attacker(field_dimension, field_string);
  game->defender_data = new_defender(field_dimension, field_string);

  free(field_string);

  set_attacker_in_field(game->field, game->attacker);
  set_defender_in_field(game->field, game->defender);

  return game;
}

/*----------------------------------------------------------------------------*/

Game new_game_from_map(
    Map map,
    PlayerStrategy execute_attacker_strategy,
    PlayerStrategy execute_defender_strategy)
{
  if (map == NULL)
    return NULL;

  dimension_t field_dimension = get_map_dimension(map);

  Game game = allocate_game(
      field_dimension,
      execute_attacker_strategy,
      execute_defender_strategy);

  if (has_map_exceeded_max_occurrences_of_symbol(
          map, get_item_symbol(game->attacker), MAX_SINGLE_OCCURRENCE))
  {
    fprintf(stderr, "ERROR: Map exceeded max occurrences of symbol %c\n",
            get_item_symbol(game->attacker));
    delete_game(game);
    return NULL;
  }

  if (has_map_exceeded_max_occurrences_of_symbol(
          map, get_item_symbol(game->defender), MAX_SINGLE_OCCURRENCE))
  {
    fprintf(stderr, "ERROR: Map exceeded max occurrences of symbol %c\n",
            get_item_symbol(game->defender));
    delete_game(game);
    return NULL;
  }

  set_item_in_field_from_map(game->field, game->obstacle, map);

  set_item_in_field_from_map(game->field, game->attacker, map);
  set_item_in_field_from_map(game->field, game->defender, map);

  return game;
}

/*----------------------------------------------------------------------------*/

void delete_game(Game game)
{
  if (game == NULL)
    return;

  delete_spy(game->defender_spy);
  game->defender_spy = NULL;

  delete_spy(game->attacker_spy);
  game->attacker_spy = NULL;

  delete_item(game->obstacle);
  game->obstacle = NULL;

  delete_item(game->defender);
  game->defender = NULL;

  delete_item(game->attacker);
  game->attacker = NULL;

  game->execute_defender_strategy = NULL;
  game->execute_attacker_strategy = NULL;

  delete_field(game->field);
  game->field = NULL;

  free(game);
}

/*----------------------------------------------------------------------------*/

void print_game(Game game)
{
  if (game == NULL)
    return;

  print_field_grid(game->field);
}

/*----------------------------------------------------------------------------*/

void play_game(Game game, size_t max_turns)
{
  if (game == NULL)
    return;

  printf("Turn 0\n");
  print_game(game);

  set_spy_position(game->defender_spy, get_item_position(game->defender));
  set_spy_position(game->attacker_spy, get_item_position(game->attacker));

  for (size_t turn = 0; turn < max_turns; turn++)
  {
    printf("Turn %ld\n", turn + 1);

    move_item(game->field,
              game->attacker,
              game->defender_spy,
              game->execute_attacker_strategy,
              game->attacker_data);

    move_item(game->field,
              game->defender,
              game->attacker_spy,
              game->execute_defender_strategy,
              game->defender_data);

    print_game(game);

    if (has_attacker_arrived_end_field(game->field, game->attacker))
    {
      printf("GAME OVER! Attacker wins!\n");
      return;
    }

    if (has_defender_captured_attacker(game->attacker, game->defender))
    {
      printf("GAME OVER! Defender wins!\n");
      return;
    }
  }

  // A draw happens only if nobody wins before max_turns
  printf("GAME OVER! Attacker and Defender draw!\n");
}

/*----------------------------------------------------------------------------*/
/*                             PRIVATE FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

Game allocate_game(
    dimension_t field_dimension,
    PlayerStrategy execute_attacker_strategy,
    PlayerStrategy execute_defender_strategy)
{
  Game game = malloc(sizeof(*game));

  game->field = new_field(field_dimension);

  game->execute_attacker_strategy = execute_attacker_strategy;
  game->execute_defender_strategy = execute_defender_strategy;

  game->attacker = new_item('A', true);
  game->defender = new_item('D', true);
  game->obstacle = new_item('X', false);

  game->attacker_spy = new_spy(game->attacker);
  game->defender_spy = new_spy(game->defender);

  return game;
}

/*----------------------------------------------------------------------------*/

bool has_map_exceeded_max_occurrences_of_symbol(Map map,
                                                char symbol,
                                                size_t max_occurrences)
{
  if (max_occurrences == 0)
    return false;

  dimension_t map_dimension = get_map_dimension(map);

  size_t item_symbol_occurrences = 0;
  for (size_t i = 0; i < map_dimension.height; i++)
  {
    for (size_t j = 0; j < map_dimension.width; j++)
    {
      position_t current_position = {i, j};
      char current_map_symbol = get_map_symbol(map, current_position);

      if (current_map_symbol == symbol)
      {
        item_symbol_occurrences++;
      }
    }
  }

  return item_symbol_occurrences > max_occurrences ? true : false;
}

/*----------------------------------------------------------------------------*/

void set_item_in_field_from_map(Field field,
                                Item item,
                                Map map)
{
  char item_symbol = get_item_symbol(item);

  dimension_t map_dimension = get_map_dimension(map);
  dimension_t field_dimension = get_field_dimension(field);

  // By design, field should have the same dimension as the map
  assert(field_dimension.height == map_dimension.height);
  assert(field_dimension.width == map_dimension.width);

  for (size_t i = 0; i < map_dimension.height; i++)
  {
    for (size_t j = 0; j < map_dimension.width; j++)
    {
      position_t current_position = {i, j};
      char current_map_symbol = get_map_symbol(map, current_position);

      if (current_map_symbol == item_symbol)
      {
        add_item_to_field(field, item, current_position);
      }
    }
  }
}

/*----------------------------------------------------------------------------*/

void set_attacker_in_field(Field field, Item attacker)
{
  if (field == NULL || attacker == NULL)
    return;

  dimension_t field_dimension = get_field_dimension(field);

  position_t attacker_initial_position = {
      field_dimension.height / 2, 1 // Left side of field
  };
  add_item_to_field(field, attacker, attacker_initial_position);
}

/*----------------------------------------------------------------------------*/

void set_defender_in_field(Field field, Item defender)
{
  if (field == NULL || defender == NULL)
    return;

  dimension_t field_dimension = get_field_dimension(field);

  position_t defender_initial_position = {
      field_dimension.height / 2, field_dimension.width - 2 // Right side of field
  };
  add_item_to_field(field, defender, defender_initial_position);
}

/*----------------------------------------------------------------------------*/

void set_obstacles_in_field(Field field, Item obstacle)
{
  dimension_t field_dimension = get_field_dimension(field);
  size_t field_height = field_dimension.height;
  size_t field_width = field_dimension.width;

  for (size_t i = 0; i < field_height; i++)
  {
    position_t left_border = {i, 0};
    add_item_to_field(field, obstacle, left_border);
  }

  for (size_t i = 0; i < field_height; i++)
  {
    position_t right_border = {i, field_width - 1};
    add_item_to_field(field, obstacle, right_border);
  }

  for (size_t j = 0; j < field_width; j++)
  {
    position_t top_border = {0, j};
    add_item_to_field(field, obstacle, top_border);
  }

  for (size_t j = 0; j < field_width; j++)
  {
    position_t bottom_border = {field_height - 1, j};
    add_item_to_field(field, obstacle, bottom_border);
  }
}

/*----------------------------------------------------------------------------*/

bool has_defender_captured_attacker(Item defender, Item attacker)
{
  if (attacker == NULL || defender == NULL)
    return false;

  position_t attacker_position = get_item_position(attacker);
  position_t defender_position = get_item_position(defender);

  return neighbor_positions(defender_position, attacker_position);
}

/*----------------------------------------------------------------------------*/

bool has_attacker_arrived_end_field(Field field, Item attacker)
{
  if (field == NULL || attacker == NULL)
    return false;

  position_t attacker_position = get_item_position(attacker);
  dimension_t field_dimension = get_field_dimension(field);

  return attacker_position.j == field_dimension.width - 2;
}

/*----------------------------------------------------------------------------*/

void move_item(Field field,
               Item item,
               Spy opponent_spy,
               PlayerStrategy execute_item_strategy,
               void *data)
{
  position_t item_position = get_item_position(item);

  direction_t item_direction = execute_item_strategy(item_position, opponent_spy, data);

  if (equal_directions(item_direction, (direction_t)DIR_STAY))
  {
    set_spy_position(opponent_spy, get_item_position(get_spy_item(opponent_spy)));
  }
  else
  {
    move_item_in_field(field, item, item_direction);
  }
}

/*----------------------------------------------------------------------------*/
