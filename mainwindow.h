#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "criptare.h"
#include "struct_questions.h"
#include "dbtestgrila.h"

#include <QMainWindow>
#include <QCheckBox>
#include <QGridLayout>
#include <QString>
#include <algorithm>
#include <QList>

using namespace std;

/*struct questionBlock {
   // int id;
    QString q1;
    QString r1;
    QString r2;
    QString r3;
    QString r4;
    QString r_corect;
};*/


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_dateEdit_userDateChanged();

    void on_pushButton_START_clicked();

    void startCountdownTimer();

    void update();

    void processFile(const std::string& fileName);

    void displayQuestionBlock(const questionBlock& qBlock);

    void displayQuestionFromVector(int index);

    void on_pushButton_NEXT_clicked();

    void on_pushButton_Back_clicked();

    void on_progressBar_valueChanged();

    void functCompar();

    void on_pushButton_FINISH_clicked();

    void randomQuestions();

    void saveToFileTxt(const vector<questionBlock>& questions, const QList<QList<QString> > selectedAnswers);

    void saveToPdf(const vector<questionBlock>& questions, const QList<QList<QString>> selectedAnswers);


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString name;

    criptare cript;
    DBTestGrila dbtest;

    int punctaj = 0;
    int contor_question;

    int remainingTime;
    QTimer *timer;

    vector<questionBlock> questions;
    QList<QList<QString>> selectedAnswers;


    questionBlock currentBlok;
};



#endif // MAINWINDOW_H
