#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "struct_questions.h"
#include "criptare.h"
#include "dbtestgrila.h"
#include "logadmin.h"

#include <QtPrintSupport>
#include <QPdfWriter>
#include <QPainter>

#include <iostream>
#include <QObject>
#include <QMessageBox>
#include <QTimer>
#include <QGridLayout>
#include <QCheckBox>
#include <QDebug>
#include <fstream>
#include <QString>
#include <algorithm>
#include <random>
#include <QFileInfo>
#include <QFile>
#include <QDateTime>
#include <QDir>


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString cheie = "cheie";
    QString filePath = "/home/damaris/test_grila/Chestionar.txt";
    QFileInfo fileInfo(filePath);
    QString filePathCript = "/home/damaris/build-test_grila-Desktop_Qt_5_6_3_GCC_64bit-Debug/Criptat_Chestionar.txt";
    QFileInfo fileInfoCript(filePathCript);

    if(!fileInfoCript.exists()){
        cript.criptareFisier(fileInfo, cheie);
    }
    if(fileInfoCript.exists()){
        cript.decriptareFisier(fileInfoCript, &questions, cheie);
    }

    on_dateEdit_userDateChanged();
    bool tab = false;
    ui->tabWidget->tabBar()->setVisible(tab);
    ui->tabWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processFile(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        QMessageBox::information(nullptr, "Error", "Unable to open file!");
        return;
    }
    questions.clear();

    questionBlock q;
    int contor = 0;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);

        QString data = QString::fromStdString(line);

        switch (contor) {
        case 0:
            q.q1 = data;
            contor++;
            break;
        case 1:
            q.r1 = data;
            contor++;
            break;
        case 2:
            q.r2 = data;
            contor++;
            break;
        case 3:
            q.r3 = data;
            contor++;
            break;
        case 4:
            q.r4 = data;
            contor++;
            break;
        case 5:
            q.r_corect = data;
            questions.push_back(q);
            q = {};
            contor = 0;
            break;
        default:
            break;
        }
    }
    file.close();
}


void MainWindow::displayQuestionBlock(const questionBlock& qBlock)
{
    currentBlok.q1 = qBlock.q1;
    currentBlok.r1 = qBlock.r1;
    currentBlok.r2 = qBlock.r2;
    currentBlok.r3 = qBlock.r3;
    currentBlok.r4 = qBlock.r4;
    currentBlok.r_corect = qBlock.r_corect;

    ui->questionLabel->setText(qBlock.q1);
    ui->answer1->setText(qBlock.r1);
    ui->answer2->setText(qBlock.r2);
    ui->answer3->setText(qBlock.r3);
    ui->answer4->setText(qBlock.r4);
}

void MainWindow::displayQuestionFromVector(int index)
{
    if (index >= 0 && index < questions.size())
    {
        const questionBlock& qBlock = questions.at(index);
        displayQuestionBlock(qBlock);
    }
    if(index == questions.size()){
        on_pushButton_FINISH_clicked();
        timer->stop();
    }
}

void MainWindow::on_pushButton_START_clicked()
{
    processFile("/home/damaris/test_grila/Chestionar.txt");
    startCountdownTimer();

    contor_question = 0;
    name = ui->lineEdit_name->text();

    if (!name.isEmpty()) {
        int nextTabIndex = ui->tabWidget->currentIndex() + 1;
        if (nextTabIndex < ui->tabWidget->count()) {
            /// ui->tabWidget->setTabEnabled(nextTabIndex,True/False);

            ui->tabWidget->setCurrentIndex(nextTabIndex);
            randomQuestions();
            displayQuestionFromVector(contor_question);
        } else {
            QMessageBox::critical(this, "Error tab!"," Nu exista un tab urmator!");
        }
    } else {
        QMessageBox::critical(this, "Error name!"," Te rog, completeaza campul pentru nume!");
    }
}

void MainWindow::startCountdownTimer()
{
    remainingTime = 300; //300 de secunde = 5 min
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

void MainWindow::update()
{
    if (remainingTime > 0)
    {
        QTime displayTime(0, remainingTime/60, remainingTime%60); // Convertim secundele rămase in QTime
        ui->timeEdit->setTime(displayTime); // Afișare
        remainingTime--;
    }
    else
    {
        timer->stop();
        // adăuga aici acțiune după expirarea timpului
    }
}

void MainWindow::on_progressBar_valueChanged()
{
    int totalQuestions = questions.size();
    int questionsAnswered = contor_question;

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(totalQuestions);
    ui->progressBar->setValue(questionsAnswered);
}

void MainWindow::on_dateEdit_userDateChanged()
{
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDate(currentDate);
}

void MainWindow::on_pushButton_NEXT_clicked()
{
    bool checkBoxChecked = false;
    QList<QCheckBox *> checkBoxes = ui->tabWidget->currentWidget()->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : checkBoxes) {
        if (checkBox->isChecked()) {
            checkBoxChecked = true;
            break;
        }
    }
    if (!checkBoxChecked) {
        QMessageBox::warning(this, "Error", "Te rog, selecteaza un raspuns pentru a trece mai departe.");
        return;
    }

    functCompar();

    for (QCheckBox *checkBox : checkBoxes) {
        checkBox->setChecked(false);
    }
   // randomQuestions();
    contor_question++;
    displayQuestionFromVector(contor_question);
    on_progressBar_valueChanged();
}

void MainWindow::functCompar()
{
    QList<QCheckBox *> checkboxes = ui->tabWidget->currentWidget()->findChildren<QCheckBox *>();
    QString corectAnswer = currentBlok.r_corect;

    selectedAnswers; // Salvează selecțiile pentru toate întrebările
    QList<QString> checkedAnswers; // Salveaza răspunsurile pentru întrebarea curentă

    for (QCheckBox* checkbox : checkboxes) {
        if (checkbox->isChecked()) {
            checkedAnswers.append(checkbox->text()); // Adaugă textul din checkbox
        }
    }

    // Adaugă răspunsurile curente în lista de liste
    selectedAnswers.append(checkedAnswers);

    bool isCorrect = false;

    // Verificăm acum răspunsul selectat
    if (!checkedAnswers.isEmpty()) { // Verificăm dacă există răspunsuri selectate
        for (const QString &selAnswer : checkedAnswers) {
            if (corectAnswer.contains(selAnswer)) {
                isCorrect = true;
                punctaj++;
                break;
            }
        }
    }

    if (isCorrect) {
        //QMessageBox::information(nullptr, "", "Răspuns corect!");
    } else {
        //QMessageBox::information(nullptr, "", "Răspuns incorect!");
    }
}

void MainWindow::on_pushButton_Back_clicked()
{ 
    contor_question--;
    displayQuestionFromVector(contor_question);
}

void MainWindow::on_pushButton_FINISH_clicked()
{
    name = ui->lineEdit_name->text();
    int score = punctaj;

    QMessageBox finishMessage;
    finishMessage.setText(QString("Felicitări, %1! Scorul tău este: %2").arg(name).arg(score));
    finishMessage.exec();

    // Blocam butoanele și checkbox-urile
    ui->pushButton_NEXT->setEnabled(false);
    ui->pushButton_Back->setEnabled(false);
    ui->answer1->setEnabled(false);
    ui->answer2->setEnabled(false);
    ui->answer3->setEnabled(false);
    ui->answer4->setEnabled(false);

    saveToFileTxt(questions, selectedAnswers);
    saveToPdf(questions, selectedAnswers);

    dbtest.insertInto(punctaj, questions, name, selectedAnswers);
}

void MainWindow::randomQuestions()
{
    random_device rd;
    mt19937 q(rd());
    shuffle(questions.begin(), questions.end(), q);
}

void MainWindow::saveToFileTxt(const vector<questionBlock>& questions, const QList<QList<QString>> selectedAnswers) {

    name = ui->lineEdit_name->text();
    QList<QList<QString>> adjustedList;

    // Obține data și ora curentă
    QString dateTimeString = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");

    // Definim calea către folderul 'rezultate'
    QDir resultsDir("rezultate");

    // Verificăm dacă folderul există, dacă nu, îl creăm
    if (!resultsDir.exists()) {
        resultsDir.mkdir("."); // Creează folderul în directorul curent
    }

    // Creare nume fișier
    QString fileName = QString("rezultate/%1_%2.txt").arg(name).arg(dateTimeString);

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "Fisierul nu se poate deschide pentru scriere");
        return;
    }

    QTextStream out(&file);
    out << "UserName: " << name << "\n";

    for (size_t i = 0; i < questions.size(); ++i) {
        const questionBlock& q = questions[i];
        out << "Question: " << q.q1 << "\n";
        out << "Answer 1: " << q.r1 << "\n";
        out << "Answer 2: " << q.r2 << "\n";
        out << "Answer 3: " << q.r3 << "\n";
        out << "Answer 4: " << q.r4 << "\n";
        out << "Correct Answer: " << q.r_corect << "\n";

        adjustedList.append(selectedAnswers[i]);
        QString stringAnswer = dbtest.convertListToString(adjustedList);
        out << "Selected Answer: " << stringAnswer << "\n";
    }
    file.close();
}

void MainWindow::saveToPdf(const vector<questionBlock>& questions, const QList<QList<QString>> selectedAnswers) {

    name = ui->lineEdit_name->text();
    QList<QList<QString>> adjustedList;

    // Obține data și ora curentă
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyyMMdd_hh-mm-ss");

    // Definim calea către folderul 'rezultate'
    QDir resultsDir("rezultate");

    // Verificăm dacă folderul există, dacă nu, îl creăm
    if (!resultsDir.exists()) {
        resultsDir.mkdir("."); // Creează folderul în directorul curent
    }

    QString outputFileName = QString("rezultate/%1_%2.pdf").arg(name).arg(currentDateTime);

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(outputFileName);

    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::warning(nullptr, "Error", "Nu este activ QPainter");
        return; // Exit
    }

    painter.setFont(QFont("Arial", 12));

    painter.drawText(100, 100, QString("UserName: %1").arg(name));

    // Marchează poziția curentă pentru text
    int yPos = 130;
    const int pageHeight = printer.pageRect().height();
    const int marginBottom = 100; //  spațiu pentru margini

    for (size_t i = 0; i < questions.size(); ++i) {
        const questionBlock& q = questions[i];
        if (yPos + 100 > pageHeight - marginBottom) { // 100 este un estimat pentru înălțimea totală a întrebării și răspunsurilor
            printer.newPage(); // Adaugă o pagină nouă
            yPos = 110; // Resetează poziția pe noua pagină
        }
        painter.drawText(100, yPos, QString("Question: %1").arg(q.q1));
        yPos += 20;
        painter.drawText(100, yPos, QString("Answer 1: %1").arg(q.r1));
        yPos += 20;
        painter.drawText(100, yPos, QString("Answer 2: %1").arg(q.r2));
        yPos += 20;
        painter.drawText(100, yPos, QString("Answer 3: %1").arg(q.r3));
        yPos += 20;
        painter.drawText(100, yPos, QString("Answer 4: %1").arg(q.r4));
        yPos += 20;
        painter.drawText(100, yPos, QString("Correct Answer: %1").arg(q.r_corect));
        yPos += 20;
        adjustedList.append(selectedAnswers[i]);
        QString stringAnswer = dbtest.convertListToString(adjustedList);
        painter.drawText(100, yPos, QString("Selected Answer: %1").arg(stringAnswer));
        yPos += 40; // Spațiu între întrebări
    }
    painter.end();
}


void MainWindow::on_pushButton_clicked()
{
    LogAdmin *logadmin = new LogAdmin();
    this->hide();
    logadmin->show();
}
















