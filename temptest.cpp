#include "fragment_calculator.h"
#include <stdio.h>
#include <stdlib.h>

//using namespace FragmentCalculator;
term a[MAX_TERMS],b[MAX_TERMS],c[MAX_TERMS];

int main(int argc, char *argv[])
{
    FILE *fp;
    int i;


    //random_data(a);
    //random_data(b);
    scanf("%d\n", &i);
    fp=fopen("matrix_a.txt","r"); //開檔並讀取.
    for(i=0;!feof(fp);i++){
        fscanf(fp,"%d\t%d\t%d\n",&a[i].row,&a[i].col,&a[i].value);
    }
    fclose(fp);

    fp=fopen("matrix_b.txt","r"); //開檔並讀取.
    for(i=0;!feof(fp);i++){
        fscanf(fp,"%d\t%d\t%d\n",&b[i].row,&b[i].col,&b[i].value);
    }
    fclose(fp);

    printf("Matrix A:\n\n");
    print_matrix(a); //印出矩陣 A.
    printf("\n\nMatrix B:\n\n");
    print_matrix(b); //印出矩陣 B.
printf("\n\nsadasdasB:\n\n");
    mmult(a,b,c); //將A矩陣乘上B矩陣 在放入C矩陣中.
    printf("\n\n");

    printf("Matrix A * B:\n\n");
    print_matrix(c);


}
