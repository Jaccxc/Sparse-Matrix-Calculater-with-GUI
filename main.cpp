#include "sparse.h"
#include "fragment_calculator_qt.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

term a[MAX_TERMS],b[MAX_TERMS],c[MAX_TERMS];

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "SparseMatrix_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    /*
    int i=0;
    ifstream fp;

    fp.open("C:\\Users\\Jacc\\Desktop\\ntpu_sparse_array_cpp\\SparseMatrix\\matrix_a.txt"); //開檔並讀取.
    i=0;

    while (fp >> a[i].row){
        fp >> a[i].col >> a[i].value;
        i++;
    } ;
    qDebug() << "Matrix A:\n\n" << i << "\n\n";
    print_matrix(a); //印出矩陣 A.

    fp.close();

    fp.open("C:\\Users\\Jacc\\Desktop\\ntpu_sparse_array_cpp\\SparseMatrix\\matrix_b.txt"); //開檔並讀取.
    i=0;
    while (fp >> b[i].row){
        fp >> b[i].col >> b[i].value;
        i++;
    } ;


    qDebug() << "\n\nMatrix B:\n\n";
    print_matrix(b); //印出矩陣 B.

    mmult(a,b,c); //將A矩陣乘上B矩陣 在放入C矩陣中.
    qDebug() << "\n\n";

    qDebug() << "Matrix A * B:\n\n";
    print_matrix(c);
    */

    Sparse w;
    w.show();
    return app.exec();
}



