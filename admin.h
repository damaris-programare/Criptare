#ifndef ADMIN_H
#define ADMIN_H

#include "dbtestgrila.h"
#include <QDialog>

namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = 0);
    ~Admin();

private slots:
    void on_pushButton_Back_clicked();

    void on_pushButton_Exit_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_search2_clicked();

    void on_pushButton_search1_clicked();

    void on_pushButtonDelete1_clicked();

    void on_pushButtonDelete2_clicked();

private:
    Ui::Admin *ui;
    DBTestGrila db;
};

#endif // ADMIN_H
