#include "life.h"

// You are only allowed to change the contents of this file with respect
// to improving the performance of this program



// If you need to initialize some data structures for your implementation
// of life then make changes to the following function, otherwise
// do not change this function.

void initLife() {
}



// When completed this file will contain several versions of the life() function and
// conditional compilation will be used to determine which version runs.  Each
// version will be surrounded with  an ifdef, undef, and endif as illustrated
// immediately below this comment block. To select the version of the life function
// to use, put the #define USE USE immediately infront of the version of the life()
// to compile.  Your version of life() must produce a correct result.

// You are required to document the changes you make in the README.txt file. For each entry
// in the README.txt file there is to be a version of the matching  life() function here such
// that the markers can see, and run if needed,  the actual code that you used
// to produce your results for the logged change to the life function.


#ifdef USE
int life(long oldWorld[N][N], long newWorld[N][N]) {
    return 0;
}
#undef USE
#endif



#define USE


// For each version of life you are testing duplicate the code between the
// ifdef and endif and make your changes. To use the new version, move the the #define
// just about this comment to be just in front of the ifdef for the new version.

#ifdef USE

int life(long oldWorld[N][N], long newWorld[N][N]) {
    
    int i, j, k, l;
    
    //clear the new world
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            newWorld[i][j] =  0;    // change order of i and j
        }
    
    int col, row;
    int q = -1;
    
    
    // Count the cells to the top left
    for (i = 0; i < N; i++) {
        col = (i -1 + N) % N;
        for (j = 0; j < N; j++) {
            row = (j - 1 + N ) % N;
            newWorld[i][j] += oldWorld[col][row];   // change order of i and j
        }
    }
    
    // Count the cells immediately above
    for (j = 0; j < N; j++) {                     // change order of i and j in for loop
        row = (j - 1 + N ) % N;                   // and move col =  row = (j - 1 + N ) % N;  in j loop
        for (i = 0; i < N; i++) {
            newWorld[j][i] += oldWorld[row][i];
        }
    }
    
    
    // Count the cells to the top right
    for (i = 0; i < N; i++) {
        col = (i + 1 + N) % N;
        for (j = 0; j < N; j++) {
            row = (j - 1  + N ) % N;
            newWorld[i][j] += oldWorld[col][row];    // change order of i and j
        }
    }
    
    // Count the cells to the immediate left
    for (j = 0; j < N; j++) {
        for (i = 0; i < N; i++) {                  //change order of i and j in for loop
            col = (i -1 + N) % N;                  // and move col = (i -1 + N) % N; in i loop
            newWorld[j][i] += oldWorld[j][col];
        }
    }
    
    // Count the cells to the immediate right
    for (j = 0; j < N; j++) {
        for (i = 0; i < N; i++) {
            col = (i + 1 + N) % N;                   //change order of i and j in for loop
            // and move col = (i -1 + N) % N; in i loop
            
            newWorld[j][i] += oldWorld[j][col];
        }
    }
    
    // Count the cells to the bottom left
    for (i = 0; i < N; i++) {
        col = (i - 1 + N) % N;
        for (j = 0; j < N; j++) {
            row = (j + 1 + N ) % N;
            newWorld[i][j] += oldWorld[col][row];    // change order of i and j
        }
    }
    
    // Count the cells immediately below
    for (j = 0; j < N; j++) {
        row = (j + 1  + N ) % N;                 //change order of i and j in for loop
        for (i = 0; i < N; i++) {                  //and row = (j + 1  + N ) % N; in j loop
            newWorld[j][i] += oldWorld[row][i];
        }
    }
    
    // Count the cells to the bottom right
    for (i = 0; i < N; i++) {
        col = (i + 1 + N) % N;
        for (j = 0; j < N; j++) {
            row = (j + 1  + N ) % N;
            newWorld[i][j] += oldWorld[col][row];   // change order of i and j
        }
    }
    
    // Check each cell to see if it should come to life, continue to live, or die
    int alive = 0;
    
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
                                                            // add new int haha to improve temporal locality
            newWorld[i][j] = checkHealth(newWorld[i][j], oldWorld[i][j]); // change order of i and j
            alive += newWorld[i][j] ? 1:0;
        }
    
    return alive;
}
#undef USE
#endif
