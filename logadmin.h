#ifndef LOGADMIN_H
#define LOGADMIN_H

#include <QDialog>

namespace Ui {
class LogAdmin;
}

class LogAdmin : public QDialog
{
    Q_OBJECT

public:
    explicit LogAdmin(QWidget *parent = 0);
    ~LogAdmin();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LogAdmin *ui;
};

#endif // LOGADMIN_H
