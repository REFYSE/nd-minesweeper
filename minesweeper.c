#include "minesweeper.h"
#include "params.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {
  //defensive checking
  if(dim <= 0 || num_mines < 0){
    return;
  }
  for(int i = 0; i < dim; i++){
    if(dim_sizes[i] <= 0){
      return;
    }
  }

  //Setup array of indices
  int indices[MAX_DIM];
  for(int n = 0; n < dim; n++){
    indices[n] = 0;
  }
  int size = 0; //tracks used size of game;

  //Create cells
  int finished = 0; //Flag to see if every cell has been created
  int index = 0; //Index in the game array to store the cells
  while(!finished){
    size++; //Increment the used size of game;
    //Have all cells initially set to not mined_cells
    game[index].mined = 0;
    //All cells initially not selected
    game[index].selected = 0;
    //Initialise num_adjacent to 0
    game[index].num_adjacent = 0;
    //Setup the coords of the cell
    for(int n = 0; n < dim; n++){
      game[index].coords[n] = indices[n];
    }
    //Initialise hint to 0 to begin
    game[index].hint = 0;
    //Go to next index;
    finished = 1; //Set finished to complete first, then check if not finished
    for(int n = dim-1; n >= 0; n--){
      if(indices[n] < dim_sizes[n] - 1){
        indices[n] = indices[n] + 1; //Increment the index in the nth dimension
        finished = 0;
        break; //Still more cells to fill
      } else if(indices[n] == dim_sizes[n] - 1){
        indices[n] = 0; //If reached the max size for this dimension,...
                        //... then go back to zero
      }
    }
    //Update the index;
    int temp = find_index(indices, dim, dim_sizes);
    if(temp == -1){
      return;
    }
    index = temp;
  }

  //Setup the mined_cells
  for(int i = 0; i < num_mines; i ++){
    int temp = find_index(mined_cells[i], dim, dim_sizes);
    if(temp == -1){
      return;
    }
    game[temp].mined = 1;
  }

  //Setup the adjacent and hint
  int adjacent;
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      if(i != j){ //Don't compare a cell to itself
        adjacent = 1; //set initially adjacent
        //Check if the coords differ by a max of 1 in each dimension
        for(int k = 0; k < dim; k++){
          if(abs(game[i].coords[k] - game[j].coords[k]) > 1){
            adjacent = 0; //If any of the coords are off by more than 1
            break; // then it is not adjacent
          }
        }
        if(adjacent){
          //Increment the hint
          if(game[j].mined){
            game[i].hint = game[i].hint + 1;
          }
          game[i].adjacent[game[i].num_adjacent] = &game[j];
          game[i].num_adjacent = game[i].num_adjacent + 1;
        }
      }
    }
  }
  return;
}

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {
  //defensive checking
  for(int i = 0; i < dim; i++){
    if(coords[i] >= dim_sizes[i] || coords[i] < 0){
      return 0;
    }
  }
  //Find the corresponding index in game for the given coords
  int index = find_index(coords, dim, dim_sizes);
  if(index == -1){
    return 0;
  }
  //If mined, then game is lost
  if(game[index].mined == 1){
    game[index].selected = 1;
    return 1;
  }
  //If already selected, then do nothing else
  if(game[index].selected == 1){
    return 0;
  }
  //Otherwise select cells until adj cells have mines
  select_recur(&game[index], 0);
  //Check if game is finished
  int size = 1;
  for(int i  = 0; i < dim; i++){
    size = size * dim_sizes[i];
  }
  for(int i = 0; i < size; i++){
    if(game[i].selected == 0 && game[i].mined == 0){
      //Not finished if there are unselected cells that aren't mined
      return 0;
    }
  }
  //If program reaches this point, then game is won
  return 2;
}

void select_recur(struct cell * cell, int count){
  cell->selected = 1;
  if(cell->hint == 0){
    for(int i = 0; i < cell->num_adjacent; i++){
      if(cell->adjacent[i]->selected == 0){
        select_recur(cell->adjacent[i], count);
      }
    }
  }
  return;
}

int find_index(int * coords, int dim, int * dim_sizes){
  //defensive
  for(int i = 0; i < dim; i++){
    if(coords[i] < 0 || coords[i] >= dim_sizes[i]){
      return -1;
    }
  }

  //Since we are representing a n-dimension array linearly, update the index appropriately
  int index = 0;
  for(int n = dim-1; n >= 0; n--){
    int offset = 1; //Last index has an offset of 0
    for(int m = n+1; m < dim; m++){
      offset = offset * dim_sizes[m]; //Then multiply every index ahead to get the offset for the nth dimension
    }
    index += coords[n]*offset; //increment index by the correct multiple of the offset for the nth dimension
  }
  return index;
}
