/****************************************************************************************
* Akhror Rakhmatov, arakhmat@ucsc.edu, PA4
*  Matrix.c
*  Public section for Matrix ADT
*  Used Queue.c from examples
*****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include"Matrix.h"


// structs --------------------------------------------------------------------
// private Entry type
typedef struct EntryObj* Entry;
// private EntryObj type
typedef struct EntryObj{
    int col;
    double val;
} EntryObj;

// private MatrixObj type
typedef struct MatrixObj{
    List* row;
    int size;
    int nnz;
} MatrixObj;


// Constructors-Destructors ---------------------------------------------------
Matrix newMatrix(int n){
    if (n < 0) {
        printf("Matrix Error: Cannot make an empty matrix of size n.\n");
        exit(EXIT_FAILURE);
    }
    Matrix M;
    M = malloc(sizeof(MatrixObj));
    M->row = malloc(sizeof(List) *(n+1));
    assert(M != NULL);
    M->size = n;
    M->nnz = 0;
    for (int i = 1; i <= n; ++i) {
        M->row[i] = newList();
    }
    return M;
}

void freeMatrix(Matrix* pM) {
    if(pM!=NULL && *pM!=NULL) { 
        makeZero(*pM);
        int n  = size(*pM);
        for (int i = 1; i <= n; ++i) {
            freeList(&(*pM)->row[i]);
        }
        free(*pM);
        *pM = NULL;
    }
}

Entry newEntry(int col, double val) {
    Entry E = malloc(sizeof(EntryObj));
    assert(E!=NULL);
    E->col = col;
    E->val = val;
    return (E);
}

void freeEntry(Entry* pE){
    if( pE!=NULL && *pE!=NULL ){
        free(*pE);
        *pE = NULL;
    }
}

//Access functions

int size(Matrix M) {
    if( M==NULL ){
        fprintf(stderr, "Matrix Error: Size called on NULL Matrix\n");
        exit(1);
    }
    return M->size;
}

int NNZ(Matrix M) {
    if( M==NULL ){
        fprintf(stderr, "Matrix Error: Size called on NULL Matrix\n");
        exit(1);
    }
    return M->nnz;

}

int equals(Matrix A, Matrix B) {
    if (size(A) != size(B)) {
        return 0;
    }
    for (int i = 1; i<=size(A); ++i) {
        List rowa = A->row[i];
        List rowb = B->row[i];
        if (length(rowa)) {
            moveFront(rowa);
            moveFront(rowb);
            while (index(rowa) != -1) {
                Entry eA = get(rowa);
                Entry eB = get(rowb);
                if (eA->col != eB->col || eA->val != eB->val) {
                    return 0;
                }
                moveNext(rowa);
                moveNext(rowb);
            }
        }
    }
    return 1;
}

void makeZero(Matrix M){
    if( M==NULL ){
        fprintf(stderr, "Graph Error: makeZero called on NULL Graph\n");
        exit(1);
    }
    for (int i = 1; i <=M->size; ++i) {
        List rowi = M->row[i];
        if (length(rowi)) {
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry ent = get(rowi);
                if (ent->col > 0){
                    freeEntry(&ent);
                } //also try *(Entry*)get(rowi);
            }
            clear(rowi);
        }
    }
    M->nnz = 0;
}

void changeEntry(Matrix M, int i, int j, double x) {
    // i is row, j is coloumn
    if (i < 1 || j < 1 || i > M->size || j > M->size) {
        fprintf(stderr, "Graph Error: changeEntry cannot take a colomn/row that is less than 1.\n");
        exit(1);
    }
    List rowi = M->row[i];
    Entry new = newEntry(j, x);
    if (length(rowi)) {
        moveFront(rowi);
        int found = 0;
        while(index(rowi) != -1) {
            Entry ent = get(rowi); //also try *(Entry*)get(rowi);
            if (ent->col < j) {
                moveNext(rowi);
            }
            if (ent->col > j && x) {
                insertBefore(rowi, new);
                ++M->nnz;
                found = 1;
                break;
            }
            if (ent->col == j) {
                if (x) {   
                    set(rowi, new);
                    ++M->nnz;
                    found = 1;
                    break;
                }
                if (!x) {
                    freeEntry(&ent);
                    --M->nnz;
                    found = 1;
                    break;
                }
            }
        }
        if (!found && x) {
            ++M->nnz;
            append(rowi, new);
        }
    }
    else if (!length(rowi) && x) {
        ++M->nnz;
        prepend(rowi, new);
    }
}

Matrix copy(Matrix A){
    int n = size(A);
    Matrix B = newMatrix(n);
    for (int r = 1; r <= n; ++r) {
        List rowib = B->row[r];
        List rowi = A->row[r];
        if (length(rowi)) {
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry new = get(rowi);
                Entry newb = newEntry(new->col, new->val);  //also try *(Entry*)get(rowi);
                append(rowib, newb);
                ++B->nnz;
            }
        }
   }
   return B;
}

Matrix transpose(Matrix A){
    int n = size(A);
    Matrix B = newMatrix(size(A));
    for (int r = 1; r <= n; ++r) {
        List rowi = A->row[r];
        if (length(rowi)) {
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry new = get(rowi);
                int roow = new->col;
                List rowb = B->row[roow];
                Entry newb = newEntry(r, new->val);
                append(rowb, newb);
                ++B->nnz;
            }
        }
    }
    return B;
}

Matrix scalarMult(double x, Matrix A){
    int n = size(A);
    Matrix B = newMatrix(n);
    for (int r = 1; r <= n; ++r) {
        List rowi = A->row[r];
        List rowb = B->row[r];
        if (length(rowi)) {
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry new = get(rowi);
                double newv = (new->val)*x;
                Entry newb = newEntry(new->col, newv);
                append(rowb, newb);
                ++B->nnz;
            }
        }
    }
    return B;
}

Matrix sum(Matrix A, Matrix B) {
    if( A==NULL ){
        fprintf(stderr, "Matrix Error: sum called on NULL A Matrix\n");
        exit(1);
    }
    if( B==NULL ){
        fprintf(stderr, "Matrix Error: sum called on NULL B Matrix\n");
        exit(1);
    }
    if(size(A) != size(B)){
        fprintf(stderr, "Matrix Error: sum called on different size matrices\n");
        exit(1);
    }
    int n = size(A);
    Matrix C = newMatrix(n);
    for (int r = 1; r <= n; ++r) {
        List rowi = A->row[r];
        List rowe = B->row[r];
        List rowb = copyList(rowe);
        List rowc = C->row[r];
        int la = length(rowi);
        int lb = length(rowb);
        if (la == lb && la && lb ) {
            moveFront(rowb);
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry enta = get(rowi);
                Entry entb = get(rowb);
                if (enta->col == entb->col && enta->val != -1*(entb->val)) {
                    Entry entc = newEntry(enta->col, enta->val+entb->val);
                    append(rowc, entc);
                    ++C->nnz;
                    moveNext(rowb);
                }
                else if (enta->col < entb->col) {
                    Entry entc = newEntry(enta->col, enta->val);
                    append(rowc, entc);
                    ++C->nnz;
                }
                else if (enta->col > entb->col){
                    Entry entc = newEntry(entb->col, entb->val);
                    append(rowc, entc);
                    ++C->nnz;
                    moveNext(rowb);
                }
                else{
                    moveNext(rowb);
                }
            }
            while (index(rowb) != -1) {
                Entry entb = get(rowb);
                Entry entc = newEntry(entb->col, entb->val);
                append(rowc, entc);
                ++C->nnz;
                moveNext(rowb);
            }
        }
        else if (la > lb && la && lb) {
            moveFront(rowb);
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry enta = get(rowi);
                if (index(rowb) != -1) {
                    Entry entb = get(rowb);
                    if (enta->col == entb->col && enta->val != -1*(entb->val)) {
                        Entry entc = newEntry(enta->col, enta->val+entb->val);
                        append(rowc, entc);
                        ++C->nnz;
                        moveNext(rowb);
                    }
                    else if (enta->col < entb->col) {
                        Entry entc = newEntry(enta->col, enta->val);
                        append(rowc, entc);
                        ++C->nnz;
                    }
                    else if (enta->col > entb->col){
                        Entry entc = newEntry(entb->col, entb->val);
                        append(rowc, entc);
                        ++C->nnz;
                        moveNext(rowb);
                    }
                    else {
                        moveNext(rowb);
                    }
                }
                else {
                    Entry entc = newEntry(enta->col, enta->val);
                    append(rowc, entc);
                    ++C->nnz;
                }
            }
             while (index(rowb) != -1) {
                Entry entb = get(rowb);
                Entry entc = newEntry(entb->col, entb->val);
                append(rowc, entc);
                ++C->nnz;
                moveNext(rowb);
            }
        }
        else if (la < lb && la && lb) {
            moveFront(rowi);
            for (moveFront(rowb); index(rowb) != -1; moveNext(rowb)) {
                Entry entb = get(rowb);
                if (index(rowi) != -1) {
                    Entry enta = get(rowi);
                    if (enta->col == entb->col && enta->val != -1*(entb->val)) {
                        Entry entc = newEntry(enta->col, enta->val+entb->val);
                        append(rowc, entc);
                        ++C->nnz;
                        moveNext(rowi);
                    }
                    else if (enta->col < entb->col) {
                        Entry entc = newEntry(enta->col, enta->val);
                        append(rowc, entc);
                        ++C->nnz;
                        moveNext(rowi);
                    }
                    else if (enta->col > entb->col){
                        Entry entc = newEntry(entb->col, entb->val);
                        append(rowc, entc);
                        ++C->nnz;
                    }
                    else{
                        moveNext(rowb);
                    }
                }
                else {
                    Entry entc = newEntry(entb->col, entb->val);
                    append(rowc, entc);
                    ++C->nnz;
                }
            }
             while (index(rowi) != -1) {
                Entry entb = get(rowi);
                Entry entc = newEntry(entb->col, entb->val);
                append(rowc, entc);
                ++C->nnz;
                moveNext(rowi);
            }
        }
        else if (la && !lb ) {
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry enta = get(rowi);
                Entry entc = newEntry(enta->col, enta->val);
                append(rowc, entc);
                ++C->nnz;
            }
        }
        else if (!la && lb ) {
            for (moveFront(rowb); index(rowb) != -1; moveNext(rowb)) {
                Entry entb = get(rowb);
                Entry entc = newEntry(entb->col, entb->val);
                append(rowc, entc);
                ++C->nnz;
            }
        }
    }
    return C;
}

Matrix diff(Matrix A, Matrix B){
    if( A==NULL ){
        fprintf(stderr, "Matrix Error: diff called on NULL A Matrix\n");
        exit(1);
    }
    if( B==NULL ){
        fprintf(stderr, "Matrix Error: diff called on NULL B Matrix\n");
        exit(1);
    }
    if(size(A) != size(B)){
        fprintf(stderr, "Matrix Error: diff called on different size matrices\n");
        exit(1);
    }
    int n = size(A);
    Matrix C = newMatrix(n);
    for (int r = 1; r <= n; ++r) {
        List rowi = A->row[r];
        List rowb = B->row[r];
        List rowc = C->row[r];
        int la = length(rowi);
        int lb = length(rowb);
        if (la == lb && la && lb ) {
            moveFront(rowb);
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry enta = get(rowi);
                Entry entb = get(rowb);
                if (enta->val != entb->val) {
                    if (enta->col == entb->col) {
                        Entry entc = newEntry(enta->col, enta->val-entb->val);
                        append(rowc, entc);
                        ++C->nnz;
                        moveNext(rowb);
                    }
                    else if (enta->col < entb->col) {
                        Entry entc = newEntry(enta->col, enta->val);
                        append(rowc, entc);
                        ++C->nnz;
                    }
                    else {
                        Entry entc = newEntry(entb->col, (entb->val)*-1);
                        append(rowc, entc);
                        ++C->nnz;
                        moveNext(rowb);
                    }
                }
            }
            while (index(rowb) != -1) {
                Entry entb = get(rowb);
                Entry entc = newEntry(entb->col, entb->val);
                append(rowc, entc);
                ++C->nnz;
                moveNext(rowb);
            }
        }
        else if (la > lb && la && lb) {
            moveFront(rowb);
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry enta = get(rowi);
                if (index(rowb) != -1) {
                    Entry entb = get(rowb);
                    if (enta->val != entb->val) {
                        if (enta->col == entb->col) {
                            Entry entc = newEntry(enta->col, enta->val-entb->val);
                            append(rowc, entc);
                            ++C->nnz;
                            moveNext(rowb);
                        }
                        else if (enta->col < entb->col) {
                            Entry entc = newEntry(enta->col, enta->val);
                            append(rowc, entc);
                            ++C->nnz;
                        }
                        else {
                            Entry entc = newEntry(entb->col, (entb->val)*-1);
                            append(rowc, entc);
                            ++C->nnz;
                            moveNext(rowb);
                        }
                    }
                }
                else {
                    Entry entc = newEntry(enta->col, enta->val);
                    append(rowc, entc);
                    ++C->nnz;
                }
            }
            while (index(rowb) != -1) {
                Entry entb = get(rowb);
                Entry entc = newEntry(entb->col, entb->val);
                append(rowc, entc);
                ++C->nnz;
                moveNext(rowb);
            }
        }
        else if (la < lb && la && lb) {
            moveFront(rowi);
            for (moveFront(rowb); index(rowb) != -1; moveNext(rowb)) {
                Entry entb = get(rowb);
                if (index(rowi) != -1) {
                    Entry enta = get(rowi);
                    if (enta->val != entb->val) {
                        if (enta->col == entb->col) {
                            Entry entc = newEntry(enta->col, enta->val-entb->val);
                            append(rowc, entc);
                            ++C->nnz;
                            moveNext(rowi);
                        }
                        else if (enta->col < entb->col) {
                            Entry entc = newEntry(enta->col, enta->val);
                            append(rowc, entc);
                            ++C->nnz;
                            moveNext(rowi);
                        }
                        else {
                            Entry entc = newEntry(entb->col, (entb->val)*-1);
                            append(rowc, entc);
                            ++C->nnz;
                        }
                    }
                }
                else {
                    Entry entc = newEntry(entb->col, (entb->val)*-1);
                    append(rowc, entc);
                    ++C->nnz;
                }
            }
             while (index(rowi) != -1) {
                Entry entb = get(rowi);
                Entry entc = newEntry(entb->col, entb->val);
                append(rowc, entc);
                ++C->nnz;
                moveNext(rowi);
            }
        }
        else if (la && !lb ) {
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry enta = get(rowi);
                Entry entc = newEntry(enta->col, enta->val);
                append(rowc, entc);
                ++C->nnz;
            }
        }
        else if (!la && lb ) {
            for (moveFront(rowb); index(rowb) != -1; moveNext(rowb)) {
                Entry entb = get(rowb);
                Entry entc = newEntry(entb->col, (entb->val)*-1);
                append(rowc, entc);
                ++C->nnz;
            }
        }
    }
    return C;
}

Matrix product(Matrix A, Matrix B){
    if( A==NULL ){
        fprintf(stderr, "Matrix Error: product called on NULL A Matrix\n");
        exit(1);
    }
    if( B==NULL ){
        fprintf(stderr, "Matrix Error: product called on NULL B Matrix\n");
        exit(1);
    }
    if(size(A) != size(B)){
        fprintf(stderr, "Matrix Error: product called on different size matrices\n");
        exit(1);
    }
    int n = size(A);
    Matrix C = newMatrix(n);
    Matrix D = transpose(B);
    for (int r = 1; r <= n; ++r) {
        List rowi = A->row[r];
        List rowc = C->row[r];
        int colcount = 0;
        for (int rb = 1; rb <=n; ++rb) {
            ++colcount;
            List rowb = D->row[rb];
            int la = length(rowi);
            int lb = length(rowb);
            if(la && lb) {
                double valc = 0;
                moveFront(rowb);
                for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)){
                    Entry entb = get(rowb);
                    Entry enta = get(rowi);
                    valc = valc + (enta->val)*(entb->val);
                }
                Entry entc = newEntry(colcount, valc);
                append(rowc, entc);
                ++C->nnz;
            }
        }
    }
    freeMatrix(&D);
    return C;
}

void printMatrix(FILE* out, Matrix M) {
    if( M==NULL ){
        fprintf(stderr, "Matrix Error: product called on NULL A Matrix\n");
        exit(1);
    }
    for (int r = 1; r <= size(M); ++r) {
        List rowi = M->row[r];
        if (length(rowi)) {
            fprintf(out, "%d: ", r);
            for (moveFront(rowi); index(rowi) != -1; moveNext(rowi)) {
                Entry new = get(rowi);
                fprintf(out, " (%d, %0.1f)", new->col, new->val);
            }
            fprintf(out, "\n");
        }
    }
}