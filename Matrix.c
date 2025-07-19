/***
* Andrew Byi
* abyi
* 2024 Winter CSE101 PA4
* Matrix.c
* Defines the Matrix functions
***/ 

#include "Matrix.h"

typedef struct EntryObj * Entry;

typedef struct EntryObj
{
    int column;
    double value;

}EntryObj;

typedef struct MatrixObj
{
    List* row;
    int size;
    int NNZ; //number of non-zero elements
}MatrixObj;

Entry newEntry(int col, double val)
{
    Entry E = malloc(sizeof(EntryObj));
    assert(E != NULL);
    E->column = col;
    E->value = val;
    return E;
}

void freeEntry(Entry* pE)
{
    if(pE!= NULL && *pE != NULL)
    {
        free(*pE);
        *pE = NULL;
    }
}
// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n)
{
    Matrix M = malloc(sizeof(MatrixObj));
    assert(M != NULL);
    M->row = malloc((n + 1) * sizeof(List));
    for(int i = 1; i <= n; i++)
    {
        M->row[i] = newList();
    }
    M->size = n;
    M->NNZ = 0;
    return (M);
}
// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM)
{
    makeZero(*pM);
    if(pM != NULL && *pM != NULL)
    {
        //printf("freeing Matrix\n");
        for(int i = 1; i <= (*pM)->size; i++)
        {
            freeList(&((*pM)->row[i]));
        }
        free((*pM)->row);
        (*pM)->row = NULL;
        free(*pM);
    }
    *pM = NULL;
}

// Access functions 
// size()
// Return the size of square Matrix M.
int size(Matrix M)
{
    if(M == NULL){
        printf("Matrix Error: at size(), NULL Matrix");
        exit(EXIT_FAILURE);
    }
    return M->size;
}
// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M)
{
    if(M == NULL){
        printf("Matrix Error: at NNZ(), NULL Matrix");
        exit(EXIT_FAILURE);
    }
    return M->NNZ;
}
// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B)
{
    Matrix cB = copy(B);
    Entry eA = NULL;
    Entry eB = NULL;
    if(A == NULL){
        printf("Matrix Error: at NNZ(), NULL Matrix A");
        exit(EXIT_FAILURE);
    }
    if(B == NULL){
        printf("Matrix Error: at NNZ(), NULL Matrix B");
        exit(EXIT_FAILURE);
    }

    if(size(A) != size(cB))
    {
        freeMatrix(&cB);
        return 0;
    }
    if(NNZ(A) != NNZ(cB))
    {
        freeMatrix(&cB);
        return 0;
    }
    for(int i = 1; i < A->size; i++)
    {
        if(length(A->row[i]) > 0)
        {
            for(moveFront(A->row[i]), moveFront(cB->row[i]); (index(A->row[i]) >= 0) && (index(cB->row[i]) >= 0); moveNext(A->row[i]), moveNext(cB->row[i]))
            {
                eA = get(A->row[i]);
                eB = get(cB->row[i]);
                if(eA->column != eB->column)
                {
                    freeMatrix(&cB);
                    return 0;
                }
                if(eA->value != eA->value)
                {
                    freeMatrix(&cB);
                    return 0;
                }
            }
        }
        
    }
    freeMatrix(&cB);
    return 1;
}
// Manipulation procedures 
// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M)
{
    //first free the entries, then the list
    Entry eM = NULL;
    
    for(int i = 1; i <= size(M); i++)
    {
        if(length(M->row[i]) > 0)
        {
            for(moveFront(M->row[i]); index(M->row[i]) >= 0; moveNext(M->row[i]))
            {//free all entries
                eM = get(M->row[i]);
                //printf("%s%d%s%d%s\n", "at: ", i, ", ", eM->column, ": ");
                //printf("%s%lf\n", "data: ", eM->value);
                freeEntry(&eM);
                //printf("%s%p\n", "pointer is now: ", eM);
            }
        }
        clear(M->row[i]);
    }
    M->NNZ = 0;
}
// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x)
{
    if(i > size(M))
    {
        printf("Matrix Error: at changeEntry(), i > size(M)");
        exit(EXIT_FAILURE);
    }
    if(j > size(M))
    {
        printf("Matrix Error: at changeEntry(), j > size(M)");
        exit(EXIT_FAILURE);
    }
    if(i < 1)
    {
        printf("Matrix Error: at changeEntry(), i < 1");
        exit(EXIT_FAILURE);
    }
    if(j < 1)
    {
        printf("Matrix Error: at changeEntry(), j < 1");
        exit(EXIT_FAILURE);
    }
    Entry newE = NULL;
    Entry Mij = NULL;
    //printf("%s%d%s%d%s%f\n", "entry should be: ",i , ", ", j , ": ", x);
    if(length(M->row[i]) > 0)
    {
        for(moveFront(M->row[i]); index(M->row[i]) >= 0; moveNext(M->row[i]))
        {
            Mij = get(M->row[i]);
            if(x != 0)
            {
                if(Mij->column == j)//Mij != 0, x != 0: overwrite
                {
                    newE = newEntry(j, x);
                    set(M->row[i], newE);
                    freeEntry(&Mij);
                    //printf("%s%d%s%d%s%f\n", "new entry : ",i , ", ", newE->column , ": ", newE->value);
                    //printf("%s%d\n", "at index: ", index(M->row[i]));
                    //printf("%s%d\n", "with length: ", length(M->row[i]));
                    break;
                }
                else if(Mij->column > j)//Mij == 0, x != 0: insert so column is in increasing order
                {
                    newE = newEntry(j, x);
                    insertBefore(M->row[i], newE);
                    M->NNZ++;
                    //printf("%s%d%s%d%s%f\n", "new entry : ",i , ", ", newE->column , ": ", newE->value);
                    //printf("%s%d\n", "at index: ", index(M->row[i]));
                    //printf("%s%d\n", "with length: ", length(M->row[i]));
                    break;
                }
                if(back(M->row[i]) == get(M->row[i]))//in case there is 
                {
                    newE = newEntry(j, x);
                    append(M->row[i], newE);
                    M->NNZ++;
                    //printf("%s%d%s%d%s%f\n", "new entry : ",i , ", ", newE->column , ": ", newE->value);
                    //printf("%s%d\n", "at index: ", index(M->row[i]));
                    //printf("%s%d\n", "with length: ", length(M->row[i]));
                    break;
                }
                
            }
            else if(x == 0)
            {
                if(Mij->column == j)//Mij != 0, x == 0: delete
                {
                    //printf("%s%d\n", "delete index: ", index(M->row[i]));
                    //printf("%s%d\n", "with length: ", length(M->row[i]));
                    delete(M->row[i]);
                    M->NNZ--;
                    freeEntry(&Mij);
                    break;
                }
                if(Mij->column > j)//Mij == 0, x == 0: do nothing
                {
                    break;
                }
            }
        }
    }
    else
    {
        if(x != 0)
        {
            newE = newEntry(j, x);
            append(M->row[i], newE);
            M->NNZ++;
            //printf("%s%d%s%d%s%f\n", "new entry : ",i , ", ", newE->column , ": ", newE->value);
            //printf("%s%d\n", "new length: ", length(M->row[i]));
        }
    }
}
// Matrix Arithmetic operations 
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A)
{
    Matrix copyA = newMatrix(A->size);
    Entry eA = NULL;
    Entry eC = NULL;
    copyA->NNZ = A->NNZ;
    for(int i = 1; i <= A->size; i++)
    {
        if(length(A->row[i]) > 0)
        {
            for(moveFront(A->row[i]); index(A->row[i]) >= 0; moveNext(A->row[i]))
            {
                
                eA = get(A->row[i]);
                eC = newEntry(eA->column, eA->value);
                append(copyA->row[i], eC);
                // NNZ++ already happened
            }
        }
    }
    return copyA;
}
// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A)
{
    Matrix transA = newMatrix(A->size);
    Entry eA = NULL;
    Entry eT = NULL;
    transA->NNZ = A->NNZ;
    for(int iA = 1; iA <= A->size; iA++)
    {
        if(length(A->row[iA]))
        {
            for(moveFront(A->row[iA]); index(A->row[iA]) >= 0; moveNext(A->row[iA]))
            {
                eA = get(A->row[iA]);
                eT = newEntry(iA, eA->value);
                append(transA->row[eA->column], eT);
                //NNZ++ called already
            }
        }
    }
    return transA;
}
// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A)
{
    
    Matrix xA = newMatrix(size(A));
    Entry eA = NULL;
    Entry newE = NULL;
    for(int i = 1; i <= size(A); i++)
    {
        //printf("%s%d\n", "scalarMult() i: ", i);
        //printf("%s%d\n", "scalarMult() length: ", length(A->row[i]));
        if(length(A->row[i]) > 0)
        {
            for(moveFront(A->row[i]); index(A->row[i]) >= 0; moveNext(A->row[i]))
            {
                //printf("%s%d\n", "scalarMult() index: ", index(A->row[i]));
                eA = get(A->row[i]);
                //changeEntry(xA, i, eA->column, x * eA->value);
                newE = newEntry(eA->column, x * eA->value);
                append(xA->row[i], newE);
                xA->NNZ++;
            }
        }
    }
    return xA;
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) //change changeEntry to append?
{
    if(size(A) != size(B))
    {
        printf("Matrix Error: at changeEntry(), size(A) != size(B)");
        exit(EXIT_FAILURE);
    }
    Matrix sumAB = newMatrix(size(A));
    Matrix cB = copy(B); // incase the matrices A and B are pointing to the same
    Entry eA = NULL;
    Entry eB = NULL;
    Entry newE = NULL;
    for(int i = 1; i <= size(A); i++)//goes through array
    { 
        if((length(A->row[i]) > 0) && (length(cB->row[i]) > 0))//check if row i in A or B exist
        { 
            moveFront(A->row[i]);
            moveFront(cB->row[i]);
            while( (index(A->row[i]) >= 0) && (index(cB->row[i]) >= 0))
            {
                eA = get(A->row[i]);
                eB = get(cB->row[i]);
                //printf("%s%d%s%d%s%lf\n", "Aij: ", i, " ", eA->column, ", value: ", eA->value);
                //printf("%s%d%s%d%s%lf\n", "Bij: ", i, " ", eB->column, ", value: ", eB->value);
                if(eA->column > eB->column) 
                {
                    //printf("%s%d%s%d%s%lf\n", "==adding Aij: ", i, " ", eA->column, ", value: ", eA->value);
                    newE = newEntry(eB->column, eB->value);
                    append(sumAB->row[i], newE);
                    sumAB->NNZ++;
                    moveNext(cB->row[i]);
                }
                else if(eA->column < eB->column)
                {
                    //printf("%s%d%s%d%s%lf\n", "==adding Bij: ", i, " ", eB->column, ", value: ", eB->value);
                    newE = newEntry(eA->column, eA->value);
                    append(sumAB->row[i], newE);
                    sumAB->NNZ++;
                    moveNext(A->row[i]);
                }
                else if(eA->column == eB->column)
                {
                    //printf("%s%d%s%d%s%lf\n", "==adding Aij + BiJ: ", i, " ", eA->column, ", value: ", eA->value + eB->value);
                    
                    if(eA->value + eB->value != 0)
                    {
                        newE = newEntry(eA->column, eA->value + eB->value);
                        append(sumAB->row[i], newE);
                        sumAB->NNZ++;
                    }
                    moveNext(A->row[i]);
                    moveNext(cB->row[i]);
                }
            }
            if((index(A->row[i]) >= 0))
            {
                while((index(A->row[i]) >= 0))
                {
                    eA = get(A->row[i]);
                    newE = newEntry(eA->column, eA->value);
                    append(sumAB->row[i], newE);
                    sumAB->NNZ++;
                    moveNext(A->row[i]);
                }
            }
            else if((index(cB->row[i]) >= 0))
            {
                
                while((index(cB->row[i]) >= 0))
                {
                    eB = get(cB->row[i]);
                    newE = newEntry(eB->column, eB->value);
                    append(sumAB->row[i], newE);
                    sumAB->NNZ++;
                    moveNext(cB->row[i]);
                }
            }
        }
        else if(length(A->row[i]) > 0)
        {
            moveFront(A->row[i]);
            while((index(A->row[i]) >= 0))
            {
                eA = get(A->row[i]);
                newE = newEntry(eA->column, eA->value);
                append(sumAB->row[i], newE);
                sumAB->NNZ++;
                moveNext(A->row[i]);
            }
        }
        else if(length(cB->row[i]) > 0)
        {
            moveFront(cB->row[i]);
            while((index(cB->row[i]) >= 0))
            {
                eB = get(cB->row[i]);
                newE = newEntry(eB->column, eB->value);
                append(sumAB->row[i], newE);
                sumAB->NNZ++;
                moveNext(cB->row[i]);
            }
        }
    }
    freeMatrix(&cB);
    return sumAB;
}
// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B)//change changeEntry to append?
{
    if(size(A) != size(B))
    {
        printf("Matrix Error: at changeEntry(), size(A) != size(B)");
        exit(EXIT_FAILURE);
    }
    Matrix diffAB = newMatrix(A->size);
    Matrix cB = copy(B);
    Entry eA = NULL;
    Entry eB = NULL;
    Entry newE = NULL;
    for(int i = 1; i <= size(A); i++)//goes through array
    {
        if((length(A->row[i]) > 0) && (length(cB->row[i]) > 0))//check if row i in A or B exist
        { 
            moveFront(A->row[i]);
            moveFront(cB->row[i]);
            
            while( (index(A->row[i]) >= 0) && (index(cB->row[i]) >= 0))
            {
                
                eA = get(A->row[i]);
                eB = get(cB->row[i]);
                
                if(eA->column > eB->column) 
                {
                    newE = newEntry(eB->column, 0 - eB->value);
                    append(diffAB->row[i], newE);
                    diffAB->NNZ++;
                    moveNext(cB->row[i]);
                }
                else if(eA->column < eB->column)
                {
                    newE = newEntry(eA->column, eA->value);
                    append(diffAB->row[i], newE);
                    diffAB->NNZ++;
                    moveNext(A->row[i]);
                }
                else if(eA->column == eB->column)
                {
                    
                    if((eA->value - eB->value) != 0)
                    {
                        newE = newEntry(eA->column, eA->value - eB->value);
                        append(diffAB->row[i], newE);
                        diffAB->NNZ++;
                    }
                    moveNext(A->row[i]);
                    moveNext(cB->row[i]);
                }
            }
            if((index(A->row[i]) >= 0))
            {
                while((index(A->row[i]) >= 0))
                {
                    eA = get(A->row[i]);
                    newE = newEntry(eA->column, eA->value);
                    append(diffAB->row[i], newE);
                    diffAB->NNZ++;
                    moveNext(A->row[i]);
                }
            }
            else if((index(cB->row[i]) >= 0))
            {
                while((index(cB->row[i]) >= 0))
                {
                    eB = get(cB->row[i]);
                    newE = newEntry(eB->column, 0 - eB->value);
                    append(diffAB->row[i], newE);
                    diffAB->NNZ++;
                    moveNext(cB->row[i]);
                }
            }
        }
        else if(length(A->row[i]) > 0)
        {
            moveFront(A->row[i]);
            while((index(A->row[i]) >= 0))
            {
                eA = get(A->row[i]);
                newE = newEntry(eA->column, eA->value);
                append(diffAB->row[i], newE);
                diffAB->NNZ++;
                moveNext(A->row[i]);
            }
        }
        else if(length(cB->row[i]) > 0)
        {
            moveFront(cB->row[i]);
            while((index(cB->row[i]) >= 0))
            {
                eB = get(cB->row[i]);
                newE = newEntry(eB->column, 0 - eB->value);
                append(diffAB->row[i], newE);
                diffAB->NNZ++;
                moveNext(cB->row[i]);
            }
        }
    }
    freeMatrix(&cB);
    return diffAB;
}
// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B)
{
    if(size(A) != size(B))
    {
        printf("Matrix Error: at product(), size(A) != size(B)");
        exit(EXIT_FAILURE);
    }
    Matrix AB = newMatrix(A->size);
    Matrix TB = transpose(B);
    Entry eA = NULL;
    Entry eB = NULL;
    Entry newE = NULL;
    double dot;
    
    for(int iA = 1; iA <= size(A); iA++)
    {
        if(length(A->row[iA]) > 0)
        {
            for(int iB = 1; iB <= size(TB); iB++)
            {
                dot = 0;
                if((length(TB->row[iB]) > 0))
                {
                    moveFront(A->row[iA]);
                    moveFront(TB->row[iB]);
                    while( (index(A->row[iA]) >= 0) && (index(TB->row[iB]) >= 0))
                    {
                        eA = get(A->row[iA]);
                        eB = get(TB->row[iB]);
                        if(eA->column > eB->column)
                        {
                            moveNext(TB->row[iB]);
                        }
                        if(eA->column < eB->column)
                        {
                            moveNext(A->row[iA]);
                        }
                        if(eA->column == eB->column)
                        {
                            dot = dot + (eA->value * eB->value);
                            moveNext(A->row[iA]);
                            moveNext(TB->row[iB]);
                        }
                    }
                    //changeEntry(AB, iA, iB, dot);
                    if(dot != 0)
                    {
                        newE = newEntry(iB, dot);
                        append(AB->row[iA], newE);
                        AB->NNZ++;
                    }
                    
                }
            }
        }
    }
    freeMatrix(&TB);
    return (AB);
}
// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows 
// are not printed. Each non-zero row is represented as one line consisting 
// of the row number, followed by a colon, a space, then a space separated 
// list of pairs "(col, val)" giving the column numbers and non-zero values 
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M)
{
    Entry E = NULL;
    for(int i = 1; i <= size(M); i++)
    {
        
        if(length(M->row[i]) > 0) //if there is anything to print
        {
            fprintf(out, "%d%s", i, ": ");
            for(moveFront(M->row[i]); index(M->row[i]) >= 0; moveNext(M->row[i]))
            {
                E = get(M->row[i]);
                fprintf(out, "%s%d%s%0.1f%s", "(", E->column, ", ", E->value, ") ");
            }
            fprintf(out, "\n");
        }
    }
}
