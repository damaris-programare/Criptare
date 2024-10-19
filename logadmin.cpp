#include "logadmin.h"
#include "ui_logadmin.h"
#include "admin.h"
#include "dbtestgrila.h"

#include <QMessageBox>

LogAdmin::LogAdmin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogAdmin)
{
    ui->setupUi(this);
}

LogAdmin::~LogAdmin()
{
    delete ui;
}

void LogAdmin::on_pushButton_clicked()
{
    QString Name = ui->lineEdit->text();
    QString Password = ui->lineEdit_2->text();
    if(Name.isEmpty() || Password .isEmpty()){
        QMessageBox::warning(this, "Error", "Introduceti numele si parola!");
        return;
    }

    QString name = "admin";
    QString pass = "pass123";
    if((Name == name) && (Password == pass)){
        QMessageBox::information(this, "login", "Logare cu succes!");
        Admin *admin = new Admin();
        this->hide();
        admin->show();
    }
}
