#include <stdbool.h>

__inline bool upperBoundary(int i) {
    return i == 0;
}

__inline bool lowerBoundary(int i, int height) {
    return i == height-1;
}

__inline bool leftBoundary(int j) {
    return j == 0;
}

__inline bool rightBoundary(int j, int width) {
    return j == width-1;
}

void findAdj(int adj[][2], int i, int j, int width, int height) {
    //Getting the adjacent cell of a specific cell
    //adj = {{i,j+1},{i-1,j+1},{i-1,j},{i-1,j-1},{i,j-1},{i+1,j-1},{i+1,j},{i+1,j+1}};
    
    if (upperBoundary(i)) {
        for (int l = 1; l < 4; l++) {
            for (int m = 0; m < 2; m++) {
                adj[l][m] = -1;
            }
        }
    }

    if (lowerBoundary(i, height)) {
        for (int l = 5; l < 8; l++) {
            for (int m = 0; m < 2; m++) {
                adj[l][m]  = -1;
            }
        }
    }

    if (leftBoundary(j)) {
        for (int l = 3; l < 6; l++) {
            for (int m = 0; m < 2; m++) {
                adj[l][m]  = -1;
            }
        }
    }

    if (rightBoundary(j, width)) {
        for (int l = 0; l < 2; l++) {
            for (int m = 0; m < 2; m++) {
                adj[l][m]  = -1;
            }
        }
        adj[7][0]=-1;
        adj[7][1]=-1;
    }


    return ;
}

