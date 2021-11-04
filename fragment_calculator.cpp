#include "fragment_calculator.h"

#include<stdio.h>
#include<stdlib.h>



void print_matrix(term matrix[])
{
    int i;
    printf("Row   Col   Value\n");
    for(i=0;i<=matrix[0].value;i++){
        printf("%-5d %-5d %-5d\n",matrix[i].row,matrix[i].col,matrix[i].value);
    }
    printf("\n");
    int j,k = 1;
    for(i=0;i < matrix[0].row;i++){
        for(j=0;j < matrix[0].col;j++){
            if(matrix[k].row == i && matrix[k].col == j){
                printf("%-5d ",matrix[k].value);
                k++;
            }else printf("%-5d ",0);
        }
        printf("\n");
    }
}
void Transpose_matrix(term a[],term b[])
{
    int i, j, currentb = 1;

    b[0].row = a[0].col;
    b[0].col = a[0].row;
    b[0].value = a[0].value;

    if(a[0].value > 0) {
        for(i=0; i < a[0].col; i++){
            for (j=1; j <= a[0].value; j++){
                if (a[j].col == i) {
                    b[currentb].row = a[j].col;
                    b[currentb].col = a[j].row;
                    b[currentb].value = a[j].value;
                    currentb++;
                }
            }
        }
    }
}
void fastTranspose(term a[],term b[])
{
    int i, j, rowTerms[100]={0}, startingPos[100]={1};

    if(a[0].col > 0){
        for(i=0;i <= a[0].value;i++)
            rowTerms[a[i].col]++;
            rowTerms[5]
        for(i=1;i < a[0].col;i++)
            startingPos[i] = startingPos[i-1] + rowTerms[i-1];
        for(i=0;i <= a[0].value;i++){
            j = startingPos[a[i].col]++;
            b[j].row = a[i].col;
            b[j].col = a[i].row;
            b[j].value = a[i].value;
        }
    }
}
void mmult(term a[], term b[], term d[])
{
    int i, j, column, totald = 0;
    int row_begin = 1, row = a[1].row, sum = 0;
    term new_b[MAX_TERMS];

    if (a[0].col != b[0].row){ //矩陣A的Column與矩陣B的Row相等才能相乘.
        fprintf (stderr,"Incompatible matrices\n");
        exit (1);
    }

    fastTranspose(b,new_b); //將B矩陣轉置存到 new_b.

    a[a[0].value+1].row = a[0].row; //設置分界點.
    new_b[b[0].value+1].row = b[0].col;
    new_b[b[0].value+1].col = 0;

    for (i = 1; i <= a[0].value; ) {
        sum = 0;
        column = new_b[1].row;
        for (j = 1; j <= b[0].value+1;) {
            if (a[i].row != row) {
                storesum(d, &totald, row, column, &sum);
                i = row_begin;
                for (; b[j].col == column; j++);//換到下一個不同個column 非零元素所在的column
                column = new_b[j].row;
            } else if (new_b[j].row != column) {
                storesum(d, &totald, row, column, &sum);
                i = row_begin;
                column = new_b[j].row;
            } else {
                switch (COMPARE(a[i].col,new_b[j].col)) {
                    case 1: i++; break;  //前 < 後.
                    case -1: j++; break; //前 > 後.
                    case 0: sum += (a[i++].value * new_b[j++].value);

                }
            }
        }
        for (; a[i].row == row; i++);//換到下一個不同個row 非零元素所在的row
        row_begin = i;
        row = a[i].row;
    }


    d[0].row = a[0].row;
    d[0].col = b[0].col;
    d[0].value = totald;
}



void storesum(term d[], int totald, int row, int column, int *sum)
{
    if (sum != 0){
        if (totald < MAX_TERMS) {
            totald++;
            term last_of_d = d[totald];
            last_of_d.row = row;
            last_of_d.col = column;
            last_of_d.value = sum;
            sum = 0;
        } else {
            fprintf(stderr, "Numbers of terms in productexceed %d\n", MAX_TERMS);
            exit(1);
        }
    }
}

int COMPARE(int a, int b) //比較前後項大小.
{
    if(a>b)return -1;
    if(a==b)return 0;
    if(a<b)return 1;
    return 0;
}

void random_data(int m, int n, term d[]){
    srand( time(NULL) );

    int totald;

    for(int i=1; i<m; i++){
        if(rand() % 10 <= 3)
            continue;

        for(int j=1; j<n; j++){
            if(rand() % 10 <= 3)
                continue;
            d[++totald].row = i;
            d[totald].col = j;
            d[totald].value = rand() % 20;
        }
        //d[0]
    }
}
