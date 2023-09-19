/***************************************************************************************
* Akhror Rakhmatov, arakhmat@ucsc.edu, PA4
*  Sparse.c
*  Uses functions in Matrix.c to find 
****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"Matrix.h"
#include"List.h"

int main(int argc, char **argv) {
    FILE *in, *out;
    if (argc != 3) {
        fprintf( stderr, "Incorrect number of command line arguments, please include a file in and file out only\n" );
        exit(1);
    }
    in = fopen(argv[1], "r");
    if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
    }
    out = fopen(argv[2], "w");
    if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
    }
    int n, a, b;
    fscanf(in, "%d %d %d", &n, &a, &b);
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    for (int i = 0; i < a; ++i) {
        int r, c;
        double val;
        fscanf(in, "%d %d %lf", &r, &c, &val);
        changeEntry(A, r, c, val);
    }
    for (int j = 0; j < b; ++j) {
        int r2, c2;
        double val2;
        fscanf(in, "%d %d %lf", &r2, &c2, &val2);
        changeEntry(B, r2, c2, val2);
    }
    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\n");

    fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    fprintf(out, "1.5*(A) =\n");
    Matrix C = scalarMult(1.5, A);
    printMatrix(out, C);
    fprintf(out, "\n");

    fprintf(out, "A+B =\n");
    Matrix D = sum(A, B);
    printMatrix(out, D);
    fprintf(out, "\n");

    fprintf(out, "A+A =\n");
    Matrix E = sum(A, A);
    printMatrix(out, E);
    fprintf(out, "\n");

    fprintf(out, "B-A =\n");
    Matrix F = diff(B, A);
    printMatrix(out, F);
    fprintf(out, "\n");

    fprintf(out, "A-A =\n");
    Matrix G = diff(A, A);
    printMatrix(out, G);
    fprintf(out, "\n");

    fprintf(out, "Transpose(A) =\n");
    Matrix H = transpose(A);
    printMatrix(out, H);
    fprintf(out, "\n");

    fprintf(out, "A*B = \n");
    Matrix I = product(A,B);
    printMatrix(out, I);
    fprintf(out, "\n");

    fprintf(out, "B*B = \n");
    Matrix J = product(B,B);
    printMatrix(out, J);
    fprintf(out, "\n");

    return 0;
}