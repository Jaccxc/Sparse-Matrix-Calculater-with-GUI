#ifndef FRAGMENT_CALCULATOR_H
#define FRAGMENT_CALCULATOR_H

#define MAX_TERMS 101
#define MAX_SIZE 100





typedef struct {
    int row;
    int col;
    int value;
}term;

int COMPARE(int a, int b);
void print_matrix(term matrix[]);
void Transpose_matrix(term a[],term b[]);
void fastTranspose(term a[],term b[]);
void mmult(term a[], term b[], term d[]);
void storesum(term d[], int *totald, int row, int column, int *sum);

#endif // FRAGMENT_CALCULATOR_QT_H
