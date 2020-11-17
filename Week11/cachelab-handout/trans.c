//2019044711
/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);
void trans_32x32(int M, int N, int A[N][M], int B[M][N]);
void trans_64x64(int M, int N, int A[N][M], int B[M][N]);
void trans_61x67(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    switch (M)
    {
    case 32:
        trans_32x32(M, N, A, B);
        break;
    case 64:
        trans_64x64(M, N, A, B);
        break;
    case 61:
        trans_61x67(M, N, A, B);
        break;
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
void trans_32x32(int M, int N, int A[N][M], int B[M][N])
{
    int row, col, i, j;
    int diag_idx, diag_val;
    for (col = 0; col < M; col += 8)
    {
        for (row = 0; row < N; row += 8)
        {
            for (i = row; i < row + 8 && i < N; ++i)
            {
                for (j = col; j < col + 8 && j < M; ++j)
                {
                    if (i == j)
                    {
                        diag_idx = i;
                        diag_val = A[i][j];
                    }
                    else
                        B[j][i] = A[i][j];
                }

                if (col == row)
                {
                    B[diag_idx][diag_idx] = diag_val;
                }
            }
        }
    }
}

void trans_64x64(int M, int N, int A[N][M], int B[M][N])
{
    // int row, col;
    int v0, v1, v2, v3, v4, v5, v6, v7;
    int i, j, k;
    // int *t;
    for(i = 0; i < N; i+= 8)
    {
        for(j = 0; j < M; j+= 8)
        {
            for(k = i; k < i + 4; ++k)
            {
                v0 = A[k][j];
                v1 = A[k][j + 1];
                v2 = A[k][j + 2];
                v3 = A[k][j + 3];
                v4 = A[k][j + 4];
                v5 = A[k][j + 5];
                v6 = A[k][j + 6];
                v7 = A[k][j + 7];
                B[j][k] = v0;
                B[j][k + 4] = v4;
                B[j + 1][k] = v1;
                B[j + 1][k + 4] = v5;
                B[j + 2][k] = v2;
                B[j + 2][k + 4] = v6;
                B[j + 3][k] = v3;
                B[j + 3][k + 4] = v7;
            }
            for(k = j + 4; k < j + 8; ++k)
            {
                v4 = A[i + 4][k - 4];
                v5 = A[i + 5][k - 4];
                v6 = A[i + 6][k - 4];
                v7 = A[i + 7][k - 4];
                
                v0 = B[k - 4][i + 4];
                v1 = B[k - 4][i + 5];
                v2 = B[k - 4][i + 6];
                v3 = B[k - 4][i + 7];

                B[k - 4][i + 4] = v4;
                B[k - 4][i + 5] = v5;
                B[k - 4][i + 6] = v6;
                B[k - 4][i + 7] = v7;

                B[k][i] = v0;
                B[k][i + 1] = v1;
                B[k][i + 2] = v2;
                B[k][i + 3] = v3;

                B[k][i + 4] = A[i + 4][k];
                B[k][i + 5] = A[i + 5][k];
                B[k][i + 6] = A[i + 6][k];
                B[k][i + 7] = A[i + 7][k];
            }
        }
    }

    // for (row = 0; row < N; row += 8)
    // {
    //     for (col = 0; col < M; col += 8)
    //     {
    //         t = &A[col][row + 4];
    //         v4 = t[0];
    //         v5 = t[1];
    //         v6 = t[2];
    //         v7 = t[3];
    //         for (i = 0; i < 8; i++)
    //         {
    //             t = &A[col + i][row];
    //             v0 = t[0];
    //             v1 = t[1];
    //             v2 = t[2];
    //             v3 = t[3];
    //             t = &B[row][col + i];
    //             t[0] = v0;
    //             t[64] = v1;
    //             t[128] = v2;
    //             t[192] = v3;
    //         }
    //         for (i = 7; i > 0; i--)
    //         {
    //             t = &A[col + i][row + 4];
    //             v0 = t[0];
    //             v1 = t[1];
    //             v2 = t[2];
    //             v3 = t[3];
    //             t = &B[row + 4][col + i];
    //             t[0] = v0;
    //             t[64] = v1;
    //             t[128] = v2;
    //             t[192] = v3;
    //         }
    //         t = &B[row + 4][col];
    //         t[0] = v4;
    //         t[64] = v5;
    //         t[128] = v6;
    //         t[192] = v7;
    //     }
    // }
}

void trans_61x67(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, col, row;
    int tmp;
    for (row = 0; row < N; row += 16)
    {
        for (col = 0; col < M; col += 4)
        {
            for(i = row; i < (row + 16) && (i < N); ++i)
            {
                for(j = col; j < (col + 4) && (j < M); ++j)
                {
                    if((i - row) == (j - col))
                    {
                        tmp = A[i][j];
                    }
                    else
                    {
                        B[j][i] = A[i][j];
                    }
                }
                for(j = col; j < (col + 4) && (j < M); ++j)
                {
                    if((i - row) == (j - col))
                    {
                        B[j][i] = tmp;
                    }
                }
            }
        }
    }
}
/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    // registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

