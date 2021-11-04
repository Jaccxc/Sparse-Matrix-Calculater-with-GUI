#include "sparse.h"
#include "ui_sparse.h"
#include "fragment_calculator_qt.h"

Sparse::Sparse(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sparse)
{

    ui->setupUi(this);

    qApp->installEventFilter(this);
    this->setWindowTitle("Sparse Matrix Calculator - NTPU");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //Create drop shadow for main window
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setColor(QColor(0,0,0,60));
    ui->frame->setGraphicsEffect(shadow);

    dragPos = new QPoint(0,0);

    QSizeGrip *sizegrip = new QSizeGrip(this->ui->frame_sizegrip);
    sizegrip->setStyleSheet("width: 20px; height: 20px; margin 0px; padding: 0px;");

    groupButton1 = new QButtonGroup(this);
    groupButton1->addButton(ui->radioButton_add);
    groupButton1->addButton(ui->radioButton_sub);
    groupButton1->addButton(ui->radioButton_mul);
    groupButton1->addButton(ui->radioButton_tran);
    ui->radioButton_add->setChecked(true);

    connect(ui->radioButton_add, SIGNAL(clicked(bool)), this, SLOT(math_mode_choose()));
    connect(ui->radioButton_sub, SIGNAL(clicked(bool)), this, SLOT(math_mode_choose()));
    connect(ui->radioButton_tran, SIGNAL(clicked(bool)), this, SLOT(math_mode_choose()));
    connect(ui->radioButton_mul, SIGNAL(clicked(bool)), this, SLOT(math_mode_choose()));

    grid_matrix_a = new QGridLayout();
    grid_matrix_b = new QGridLayout();
    grid_matrix_answer = new QGridLayout();


    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_a.append(new QLineEdit());
    }
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_a[i]->setValidator(new QIntValidator(0, 100, this));

        line_edit_matrix_a[i]->setAlignment(Qt::AlignCenter);
        line_edit_matrix_a[i]->setMaximumWidth(30);
        line_edit_matrix_a[i]->setMaximumHeight(20);
        line_edit_matrix_a[i]->setMinimumWidth(30);
        //line_edit_matrix_a[i]->setPlaceholderText(QString("0"));
        line_edit_matrix_a[i]->setFont(QFont("Open Sans SemiBold", 9));
        line_edit_matrix_a[i]->setStyleSheet(QString("background-color: rgb(66, 66, 66);\nborder-radius:5px;"));

        QPalette placeholder_color_palette(line_edit_matrix_a[i]->palette());
        placeholder_color_palette.setColor(QPalette::PlaceholderText, QColor(100, 100, 100, 150));
        placeholder_color_palette.setColor(QPalette::Text, QColor(255, 255, 255));
        line_edit_matrix_a[i]->setPalette(placeholder_color_palette);

        connect(line_edit_matrix_a[i], SIGNAL(textChanged(const QString)), this, SLOT(matrix_a_changed(const QString)));

        grid_matrix_a->addWidget(line_edit_matrix_a[i], i/MATRIX_N, i%MATRIX_N);
    }

    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_b.append(new QLineEdit());
    }
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_b[i]->setValidator(new QIntValidator(0, 100, this));

        line_edit_matrix_b[i]->setAlignment(Qt::AlignCenter);
        line_edit_matrix_b[i]->setMaximumWidth(30);
        line_edit_matrix_b[i]->setMaximumHeight(20);
        line_edit_matrix_b[i]->setMinimumWidth(30);
        //line_edit_matrix_b[i]->setPlaceholderText(QString("0"));
        line_edit_matrix_b[i]->setFont(QFont("Open Sans SemiBold", 9));
        line_edit_matrix_b[i]->setStyleSheet(QString("background-color: rgb(66, 66, 66);\nborder-radius:5px;"));

        QPalette placeholder_color_palette(line_edit_matrix_b[i]->palette());
        placeholder_color_palette.setColor(QPalette::PlaceholderText, QColor(100, 100, 100, 150));
        placeholder_color_palette.setColor(QPalette::Text, QColor(255, 255, 255));
        line_edit_matrix_b[i]->setPalette(placeholder_color_palette);

        connect(line_edit_matrix_b[i], SIGNAL(textChanged(QString)), this, SLOT(matrix_b_changed(QString)));

        grid_matrix_b->addWidget(line_edit_matrix_b[i], i/MATRIX_N, i%MATRIX_N);
    }

    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_answer.append(new QLineEdit());
    }
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_answer[i]->setValidator(new QIntValidator(0, 100, this));

        line_edit_matrix_answer[i]->setAlignment(Qt::AlignCenter);
        line_edit_matrix_answer[i]->setMaximumWidth(40);
        line_edit_matrix_answer[i]->setMaximumHeight(20);
        line_edit_matrix_answer[i]->setMinimumWidth(30);
        line_edit_matrix_answer[i]->setFont(QFont("Open Sans SemiBold", 9));
        line_edit_matrix_answer[i]->setStyleSheet(QString("background-color: rgb(66, 66, 66);\nborder-radius:5px;"));

        QPalette placeholder_color_palette(line_edit_matrix_answer[i]->palette());
        placeholder_color_palette.setColor(QPalette::PlaceholderText, QColor(100, 100, 100, 150));
        placeholder_color_palette.setColor(QPalette::Text, QColor(255, 255, 255));
        line_edit_matrix_answer[i]->setPalette(placeholder_color_palette);

        line_edit_matrix_answer[i]->setDisabled(true);

        connect(line_edit_matrix_answer[i], SIGNAL(textChanged(QString)), this, SLOT(matrix_answer_changed(QString)));

        grid_matrix_answer->addWidget(line_edit_matrix_answer[i], i/MATRIX_N, i%MATRIX_N);
    }

    ui->frame_matrix_a->setLayout(grid_matrix_a);
    ui->frame_matrix_b->setLayout(grid_matrix_b);
    ui->frame_matrix_answer->setLayout(grid_matrix_answer);

}

Sparse::~Sparse()
{
    delete ui;
}


void Sparse::on_pushButton_close_clicked()
{
    this->close();
}


void Sparse::on_pushButton_minimize_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

bool Sparse::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);
        if (ev->buttons() & Qt::LeftButton)
        {
            this->dragPos->setX(ev->globalPosition().x());
            this->dragPos->setY(ev->globalPosition().y());
            ev->accept();
        }
    }
    if (obj == ui->toolbar_frame && event->type() == QEvent::MouseMove)
    {
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);
        if (ev->buttons() & Qt::LeftButton)
        {
            QPoint move_to = (this->pos() + ev->globalPosition() - *this->dragPos).toPoint();
            this->move(move_to);
            qDebug() << this->pos() << " " <<dragPos << "\n";
            this->dragPos->setX(ev->globalPosition().x());
            this->dragPos->setY(ev->globalPosition().y());
            qDebug() << "set :: " << ev->globalPosition() << "\n\n";
            ev->accept();
        }
    }

    return QObject::eventFilter(obj, event);
}

void Sparse::math_mode_choose()
{
        switch(groupButton1->checkedId())
           {
           case -2:
               qDebug() <<"add Operation"<< "\n";
               curr_mode = ADD;
               break;
           case -3:
               qDebug() <<"minus Operation"<< "\n";
               curr_mode = SUB;
               break;
           case -4:
               qDebug() <<"transpose Operation"<< "\n";
               curr_mode = TRN;
               break;
           case -5:
               qDebug() <<"multiply Operation"<< "\n";
               curr_mode = MUL;
               break;
           }
}


void Sparse::on_pushButton_a_to_b_clicked()
{
    QString temp;
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        temp = line_edit_matrix_a[i]->text();
        line_edit_matrix_a[i]->setText(line_edit_matrix_b[i]->text());
        line_edit_matrix_b[i]->setText(temp);
    }
}


void Sparse::on_pushButton_b_to_a_clicked()
{
    QString temp;
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        temp = line_edit_matrix_a[i]->text();
        line_edit_matrix_a[i]->setText(line_edit_matrix_b[i]->text());
        line_edit_matrix_b[i]->setText(temp);
    }
}


void Sparse::on_pushButton_answer_to_a_clicked()
{
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_a[i]->setText(line_edit_matrix_answer[i]->text());
    }
}


void Sparse::on_pushButton_answer_to_b_clicked()
{
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_b[i]->setText(line_edit_matrix_answer[i]->text());
    }
}


void Sparse::on_pushButton_a_clear_clicked()
{
    matrix_a_col = 0;
    matrix_a_row = 0;
    matrix_a_elements = 0;

    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_a[i]->setText(QString(""));
        line_edit_matrix_a[i]->setPlaceholderText(QString(""));
    }
}


void Sparse::on_pushButton_b_clear_clicked()
{
    matrix_b_col = 0;
    matrix_b_row = 0;
    matrix_b_elements = 0;

    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_b[i]->setText(QString(""));
        line_edit_matrix_b[i]->setPlaceholderText(QString(""));
    }
}


void Sparse::on_pushButton_answer_clear_clicked()
{
    matrix_answer_col = 0;
    matrix_answer_row = 0;
    matrix_answer_elements = 0;

    for(int i=MATRIX_N*MATRIX_N-1; i>=0; i--)
    {
        line_edit_matrix_answer[i]->setText(QString(""));
    }
}

void Sparse::matrix_a_changed(const QString &arg1)
{
    arg1.isNull();
    int current_col, current_row, max_col=-1, max_row=-1, element_count = 0;
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        if(line_edit_matrix_a[i]->text()!="")
        {
            current_col = i%MATRIX_N, current_row = i/MATRIX_N;
            if(current_col>max_col) max_col = current_col;
            if(current_row>max_row) max_row = current_row;
            if(line_edit_matrix_a[i]->text().toInt()!=0) ++element_count;
        }
        matrix_a_col = max_col;
        matrix_a_row = max_row;
        matrix_a_elements = element_count;
    }
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_a[i]->setPlaceholderText(QString(""));
        current_col = i%MATRIX_N, current_row = i/MATRIX_N;
        if(current_col<=matrix_a_col && current_row<=matrix_a_row)
        {
            line_edit_matrix_a[i]->setPlaceholderText(QString("0"));
        }
    }
}

void Sparse::matrix_b_changed(const QString &arg1)
{
    arg1.isNull();
    int current_col, current_row, max_col=-1, max_row=-1, element_count = 0;
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        if(line_edit_matrix_b[i]->text()!="")
        {
            current_col = i%MATRIX_N, current_row = i/MATRIX_N;
            if(current_col>max_col) max_col = current_col;
            if(current_row>max_row) max_row = current_row;
            if(line_edit_matrix_b[i]->text().toInt()!=0) ++element_count;
        }
        matrix_b_col = max_col;
        matrix_b_row = max_row;
        matrix_b_elements = element_count;
    }
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_b[i]->setPlaceholderText(QString(""));
        current_col = i%MATRIX_N, current_row = i/MATRIX_N;
        if(current_col<=matrix_b_col && current_row<=matrix_b_row)
        {
            line_edit_matrix_b[i]->setPlaceholderText(QString("0"));
        }
    }
}

void Sparse::matrix_answer_changed(const QString &arg1)
{
    arg1.isNull();
    int current_col, current_row;
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        line_edit_matrix_answer[i]->setPlaceholderText(QString(""));
        current_col = i%MATRIX_N, current_row = i/MATRIX_N;
        if(current_col<matrix_answer_col && current_row<matrix_answer_row)
        {
            if(line_edit_matrix_answer[i]->text() == "")
                line_edit_matrix_answer[i]->setText(QString("0"));
        }
    }
}

void Sparse::matrix_to_sparse(QList<QLineEdit*> matrix, term a[])
{
    int current_col, current_row, element_count = 0, max_col=-1, max_row=-1;
    for(int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
        if(matrix[i]->text()!="")
        {
            current_col = i%MATRIX_N, current_row = i/MATRIX_N;
            if(current_col>max_col) max_col = current_col;
            if(current_row>max_row) max_row = current_row;
            if(matrix[i]->text().toInt()!=0)
            {
                ++element_count;
                a[element_count].row = current_row;
                a[element_count].col = current_col;
                a[element_count].value = matrix[i]->text().toInt();
            }
        }
    }
    a[0].row = max_row+1;
    a[0].col = max_col+1;
    a[0].value = element_count;
}

void Sparse::sparse_to_matrix(QList<QLineEdit*> matrix, term a[])
{
    qDebug() << a[0].row << a[0].col << "\n";
    for(int i=MATRIX_N*MATRIX_N-1; i>=0; i--)
    {
        matrix[i]->setText(QString(""));
    }
    for(int i=1; i<a[0].value+1; i++)
    {
        matrix[a[i].row*5+a[i].col]->setText(QString::number(a[i].value));
    }
    for(int i=1; i<a->row*a->col+1; i++)
    {
        if(matrix[a[i].row*5+a[i].col]->text() == "")
            matrix[a[i].row*5+a[i].col]->setText("0");
    }
}

void Sparse::sparse_to_matrix_answer(QList<QLineEdit*> matrix, term a[])
{
    matrix_answer_row = a[0].row;
    matrix_answer_col = a[0].col;
    matrix_answer_elements = a[0].value;
    for(int i=MATRIX_N*MATRIX_N-1; i>=0; i--)
    {
        matrix[i]->setText(QString(""));
    }
    for(int i=1; i<a[0].value+1; i++)
    {
        matrix[a[i].row*5+a[i].col]->setText(QString::number(a[i].value));
    }
    for(int i=1; i<a->row*a->col+1; i++)
    {
        if(matrix[a[i].row*5+a[i].col]->text() == "")
            matrix[a[i].row*5+a[i].col]->setText("0");
    }

}

void Sparse::print_matrix(term a[]) //印出非零項矩陣.
{
    int i;
    qDebug() << "Row   Col   Value\n";
    for(i=0;i<=a[0].value;i++){ //印出C矩陣
        qDebug() << a[i].row << "    " << a[i].col << "    " << a[i].value;
    }
    qDebug() << "\n";
    int j,k = 1;
    for(i=0;i < a[0].row;i++){
        for(j=0;j < a[0].col;j++){
            if(a[k].row == i && a[k].col == j){
                qDebug() << "    " << a[k].value;
                k++;
            }else qDebug() << "     " << 0;
        }
        qDebug() << "\n";
    }
}





void Sparse::on_pushButton_calculate_clicked()
{
    ui->label_error_msg->setText(QString(""));
    switch(curr_mode)
       {
       case ADD:
           qDebug() <<"add Operation"<< "\n";
           add_operation();
           break;
       case SUB:
           qDebug() <<"minus Operation"<< "\n";
           sub_operation();
           break;
       case TRN:
           qDebug() <<"transpose Operation"<< "\n";
           trn_operation();
           break;
       case MUL:
           qDebug() <<"multiply Operation"<< "\n";
           qDebug() << "OK\n";
           mul_operation();
           break;
       }
}

void Sparse::add_operation()
{
    term a[MAX_TERMS], b[MAX_TERMS], c[MAX_TERMS];
    for(int i=0; i<MAX_TERMS; i++)
    {
        a[i].col = 0, a[i].row=0, a[i].value=0;
        b[i].col = 0, b[i].row=0, b[i].value=0;
        c[i].col = 0, c[i].row=0, c[i].value=0;
    }
    matrix_to_sparse(line_edit_matrix_a, a);
    matrix_to_sparse(line_edit_matrix_b, b);

    if((a[0].col != b[0].col) || (a[0].row != b[0].row))
    {
        ui->label_error_msg->setText(QString("Error: To add, A and B MUST have same size"));
        return;
    }

    add_matrix(a, b, c);
    print_matrix(c);
    sparse_to_matrix_answer(line_edit_matrix_answer,c);
}

void Sparse::sub_operation()
{
    term a[MAX_TERMS], b[MAX_TERMS], c[MAX_TERMS];
    for(int i=0; i<MAX_TERMS; i++)
    {
        a[i].col = 0, a[i].row=0, a[i].value=0;
        b[i].col = 0, b[i].row=0, b[i].value=0;
        c[i].col = 0, c[i].row=0, c[i].value=0;
    }
    matrix_to_sparse(line_edit_matrix_a, a);
    matrix_to_sparse(line_edit_matrix_b, b);

    if((a[0].col != b[0].col) || (a[0].row != b[0].row))
    {
        ui->label_error_msg->setText(QString("Error: To subtract, A and B MUST have same size"));
        return;
    }

    minus_matrix(a, b, c);
    print_matrix(c);
    sparse_to_matrix_answer(line_edit_matrix_answer,c);
}

void Sparse::trn_operation()
{
    term a[MAX_TERMS], b[MAX_TERMS];
    for(int i=0; i<MAX_TERMS; i++)
    {
        a[i].col = 0, a[i].row=0, a[i].value=0;
        b[i].col = 0, b[i].row=0, b[i].value=0;
    }
    matrix_to_sparse(line_edit_matrix_a, a);
    fastTranspose(a, b);
    sparse_to_matrix_answer(line_edit_matrix_answer, b);
}

void Sparse::mul_operation()
{
    term a[MAX_TERMS], b[MAX_TERMS], c[MAX_TERMS];
    for(int i=0; i<MAX_TERMS; i++)
    {
        a[i].col = 0, a[i].row=0, a[i].value=0;
        b[i].col = 0, b[i].row=0, b[i].value=0;
        c[i].col = 0, c[i].row=0, c[i].value=0;
    }

    matrix_to_sparse(line_edit_matrix_a, a);
    matrix_to_sparse(line_edit_matrix_b, b);

    qDebug() << a[0].col << " " << a[0].row << "\n";
    qDebug() << b[0].col << " " << b[0].row << "\n";
    print_matrix(a);
    print_matrix(b);

    if(a[0].col != b[0].row)
    {
        ui->label_error_msg->setText(QString("Error: Column of A not qual to Row of B"));
        return;
    }

    mmult(a, b, c);
    print_matrix(c);
    sparse_to_matrix_answer(line_edit_matrix_answer, c);

}

void Sparse::on_pushButton_a_random_clicked()
{
    randomize_matrix(line_edit_matrix_a);
}

void Sparse::on_pushButton_b_random_clicked()
{
    randomize_matrix(line_edit_matrix_b);
}

void Sparse::randomize_matrix(QList<QLineEdit *> matrix)
{
    srand(time(NULL));

    for (int i=0; i<MATRIX_N*MATRIX_N; i++)
    {
            if(rand()%10<3)
            {
                matrix[i]->setText(QString::number(rand()%21));
            }
    }
}

