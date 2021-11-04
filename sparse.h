#ifndef SPARSE_H
#define SPARSE_H

#include "fragment_calculator_qt.h"

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QSizeGrip>
#include <QButtonGroup>
#include <QLineEdit>
#include <QIntValidator>
#include <QPalette>
#include <QGridLayout>
#include <stdlib.h>
#include <time.h>



QT_BEGIN_NAMESPACE
namespace Ui { class Sparse; }
QT_END_NAMESPACE

class Sparse : public QMainWindow
{
    Q_OBJECT

public:
    Sparse(QWidget *parent = nullptr);
    ~Sparse();
    enum calculate_mode{
        ADD=0,
        SUB,
        MUL,
        TRN
    };




private slots:
    void on_pushButton_close_clicked();
    void on_pushButton_minimize_clicked();
    void math_mode_choose();
    void on_pushButton_a_to_b_clicked();
    void on_pushButton_b_to_a_clicked();
    void on_pushButton_answer_to_a_clicked();
    void on_pushButton_answer_to_b_clicked();
    void on_pushButton_a_clear_clicked();
    void on_pushButton_b_clear_clicked();
    void on_pushButton_answer_clear_clicked();
    void matrix_a_changed(const QString &arg1);
    void matrix_b_changed(const QString &arg1);
    void matrix_answer_changed(const QString &arg1);
    void on_pushButton_calculate_clicked();
    void on_pushButton_a_random_clicked();
    void on_pushButton_b_random_clicked();

private:
    Ui::Sparse *ui;
    QPoint *dragPos;
    QButtonGroup *groupButton1;
    QList<QLineEdit*> line_edit_matrix_a;
    QList<QLineEdit*> line_edit_matrix_b;
    QList<QLineEdit*> line_edit_matrix_answer;
    QGridLayout *grid_matrix_a;
    QGridLayout *grid_matrix_b;
    QGridLayout *grid_matrix_answer;
    const int MATRIX_N = 5;
    int matrix_a_col = 0;
    int matrix_a_row = 0;
    int matrix_a_elements = 0;
    int matrix_b_col = 0;
    int matrix_b_row = 0;
    int matrix_b_elements = 0;
    int matrix_answer_col = 0;
    int matrix_answer_row = 0;
    int matrix_answer_elements = 0;
    int curr_mode = 0;

    void matrix_to_sparse(QList<QLineEdit*> matrix, term a[]);
    void sparse_to_matrix(QList<QLineEdit*> matrix, term a[]);
    void sparse_to_matrix_answer(QList<QLineEdit*> matrix, term a[]);
    void print_matrix(term a[]);
    void add_operation();
    void sub_operation();
    void trn_operation();
    void mul_operation();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;//in header
    void randomize_matrix(QList<QLineEdit*> matrix);
};
#endif // SPARSE_H
