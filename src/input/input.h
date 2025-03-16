#ifndef INPUT_H
#define INPUT_H

#include "board.h"

int is_valid_input(const char *input);
void get_user_input(char *input);
void get_coordinates(const char *input, int *row, int *col);

#endif