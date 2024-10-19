#include "admin.h"
#include "ui_admin.h"
#include "logadmin.h"

Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_pushButton_Back_clicked()
{
    LogAdmin *logadmin = new LogAdmin();
    this->hide();
    logadmin->show();
}

void Admin::on_pushButton_Exit_clicked()
{
    this->close();
}

void Admin::on_pushButton_clicked()
{
    db.viewTable_testGrila(ui->tableView);
    db.joinTable(ui->tableView);
}

void Admin::on_pushButton_2_clicked()
{
    db.viewTable_score(ui->tableView);
    db.joinTable(ui->tableView);
}

void Admin::on_pushButton_search2_clicked()
{
    QString name = ui->lineEdit->text();
    QString date = ui->dateEdit->text();
    db.joinTable(ui->tableView);
    db.search_score(name, date, ui->tableView);
}

void Admin::on_pushButton_search1_clicked()
{
    QString name = ui->lineEdit_3->text();
    db.joinTable(ui->tableView);
    db.search_testGrila(name, ui->tableView);
}

void Admin::on_pushButtonDelete1_clicked()
{
    QString name = ui->lineEdit_3->text();
    db.deleteElemTestGrila(name, ui->tableView);
    db.viewTable_testGrila(ui->tableView);
}

void Admin::on_pushButtonDelete2_clicked()
{
    QString name = ui->lineEdit->text();
    db.deleteElemScore(name, ui->tableView);
    db.viewTable_score(ui->tableView);
}

