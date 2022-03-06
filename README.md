# ND-Minesweeper

ND-Minesweeper is played on a n-dimensional grid of n-dimensional hypercubes, where n is a pos-
itive integer and the grid is of arbitrarily defined size. We will refer to each hypercube in the game
grid as a “cell”. Each cell can therefore be defined by a set of n integer coordinates (k1, k2 . . . kn)
where 0 ≤ ki ≤ xi for 1 ≤ i ≤ n where xi is the maximum coordinate (one less than the size) of the
given i-th dimension of the grid. The dimensions of the grid do not necessarily have the same size as
each other. The cell at coordinates (0, 0 . . . 0) is always a “corner” of the grid. Each cell may contain
a mine, which is hidden from the player. The game starts with all cells unselected. Every turn, the
player selects a cell.
