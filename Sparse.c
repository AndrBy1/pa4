/***
* Andrew Byi
* abyi
* 2024 Winter CSE101 PA4
* Sparse.c
* Uses functions from Matrix.h to create and display Matrices with different operations. 
*
* Compile on Bash with gcc: "gcc -o Sparse Sparse.c Matrix.c List.c" or just type "make"
* Run after compiled: ./Sparse infile5.txt outfile5.txt
***/ 

#include "Matrix.h"

int main(int argc, char* argv[])
{
    int n, a, b;
    int row, col;
    double val;
    //size of matrix is n x n
    //a will specify the non-zero entries of an 𝑛𝑛 × 𝑛𝑛 matrix A
    //b lines specifying the non-zero entries of an 𝑛𝑛 × 𝑛𝑛 matrix B
    FILE *in, *out;
    if( argc != 3 ){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
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
    fscanf(in, "%d", &n);
    fscanf(in, "%d", &a);
    fscanf(in, "%d", &b);
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    Matrix M;
    //create A matrix:
    for(int i = 1; i <= a; i++)
    {
        if(!feof(in))
        {
            fscanf(in, "%d", &row);
            fscanf(in, "%d", &col);
            fscanf(in, "%lf", &val);
            changeEntry(A, row, col, val);
        }
    }
    for(int i = 1; i <= b; i++)
    {
        if(!feof(in))
        {
            fscanf(in, "%d", &row);
            fscanf(in, "%d", &col);
            fscanf(in, "%lf", &val);
            changeEntry(B, row, col, val);
        }
    }
    fprintf(out, "%s%d%s\n", "A has ", NNZ(A), " non-zero entries:");
    printMatrix(out, A);
    fprintf(out, "\n");
    fprintf(out, "%s%d%s\n", "B has ", NNZ(B), " non-zero entries:");
    printMatrix(out, B);
    fprintf(out, "\n");

    M = scalarMult(1.5, A);
    fprintf(out, "%s\n", "(1.5)*A = ");
    printMatrix(out, M);
    fprintf(out, "\n");
    freeMatrix(&M);

    M = sum(A, B);
    fprintf(out, "%s\n", "A+B = ");
    printMatrix(out, M);
    fprintf(out, "\n");
    freeMatrix(&M);

    M = sum(A, A);
    fprintf(out, "%s\n", "A+A = ");
    printMatrix(out, M);
    fprintf(out, "\n");
    freeMatrix(&M);
    
    M = diff(B, A);
    fprintf(out, "%s\n", "B-A = ");
    printMatrix(out, M);
    fprintf(out, "\n");
    freeMatrix(&M);

    M = diff(A, A);
    fprintf(out, "%s\n", "A-A = ");
    printMatrix(out, M);
    fprintf(out, "\n");
    freeMatrix(&M);

    M = transpose(A);
    fprintf(out, "%s\n", "Transpose(A) = ");
    printMatrix(out, M);
    fprintf(out, "\n");
    freeMatrix(&M);
    
    M = product(A, B);
    fprintf(out, "%s\n", "A*B = ");
    printMatrix(out, M);
    fprintf(out, "\n");
    freeMatrix(&M);

    M = product(B, B);
    fprintf(out, "%s\n", "B*B = ");
    printMatrix(out, M);
    fprintf(out, "\n");

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&M);
}