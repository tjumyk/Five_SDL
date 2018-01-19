/********************************************************************************
 * Please define the logic of your bot in this function. It will be called
 * automatically for each move.
 *
 * @param data: current chess map (15*15). In each cell, 0 means "no piece",
 *              1 means "your piece", 2 means "the opponent's piece"
 * @param x: the horizontal position of your next move
 * @param y: the vertical position of your next move
*********************************************************************************/

void AIwork(const int data[][15], int &x, int &y) {
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            if (data[i][j] == 0) {
                x = i;
                y = j;
                return;
            }
}