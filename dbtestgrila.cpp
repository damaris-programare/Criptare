#include "dbtestgrila.h"
#include "struct_questions.h"
#include "mainwindow.h"

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDate>
#include <QSqlQueryModel>


DBTestGrila::DBTestGrila()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydatabase.db");
    if(!db.open()){
        qDebug() << "Eroare la deschiderea bazei de date!";
    }

    createTable();
}

void DBTestGrila::createTable()
{
    QSqlQuery qry;
    qry.prepare("CREATE TABLE IF NOT EXISTS testGrila("
                "Name varchar(20),"
                "Question varchar(100),"
                "Answer1 varchar(100),"
                "Answer2 varchar(100),"
                "Answer3 varchar(100),"
                "Answer4 varchar(100),"
                "AnswerCorrect varchar(100),"
                "AnswerSelected varchar(100))");
    if (!qry.exec()) {
        qDebug() << "Eroare creare tabel testGrila!" << qry.lastError().text();
    }

    qry.prepare("CREATE TABLE IF NOT EXISTS score("
                "Nume varchar(20),"
                "Scor integer,"
                "Data date)");
    if (!qry.exec()) {
        qDebug() << "Eroare creare tabel score!" << qry.lastError().text();
    }
}


void DBTestGrila::dropTable()
{
    QSqlQuery qry;
   // qry.exec("DROP table testGrila");
   // qDebug() << "stergere tabel cu succes";

  //  qry.exec("DELETE FROM testGrila WHERE Name = 'xtyjgbh';");

    qry.exec("DELETE FROM score WHERE Name = 'andrei';");
}


void DBTestGrila::insertInto(int punctaj, vector<questionBlock>& questions, QString name, QList<QList<QString>> selectedAnswers)
{
    QDate currentDate = QDate::currentDate();
    QList<QList<QString>> adjustList;
    QSqlQuery qry;
    qry.prepare("INSERT INTO score(Nume, Scor, Data) VALUES (:Nume, :Scor, :Data)");

    qry.bindValue(":Nume", name);
    qry.bindValue(":Scor", punctaj);
    qry.bindValue(":Data", currentDate); // Convertim QDate la QString în formatul dorit.

    if (!qry.exec()) {
        qDebug() << "Error inserting data in score:" << qry.lastError().text();
    }

    qry.prepare("INSERT INTO testGrila (Name, Question, Answer1, Answer2, Answer3, Answer4, AnswerCorrect, AnswerSelected) "
                " VALUES (:Name, :Question, :Answer1, :Answer2, :Answer3, :Answer4, :AnswerCorrect, :AnswerSelected)");

    for (size_t i = 0; i < questions.size(); ++i) {
        const questionBlock& q = questions[i];

        qry.bindValue(":Name", name);
        qry.bindValue(":Question", q.q1);
        qry.bindValue(":Answer1", q.r1);
        qry.bindValue(":Answer2", q.r2);
        qry.bindValue(":Answer3", q.r3);
        qry.bindValue(":Answer4", q.r4);
        qry.bindValue(":AnswerCorrect", q.r_corect);
        adjustList.append(selectedAnswers[i]);
        QString stringAnswer = convertListToString(adjustList); // Convertim QList<QList<QString>> la QString
        qry.bindValue(":AnswerSelected", stringAnswer);

        if (!qry.exec()) {
            qDebug() << "Error inserting data in testGrila:" << qry.lastError().text();
        }
    }
}

void DBTestGrila::viewTable_testGrila(QTableView *tableView)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT * from testGrila");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    qDebug() << (model->rowCount());

    if(!qry->exec()){
        qDebug() << "Eroare afisare tabel" << qry->lastError();
        QMessageBox::warning(nullptr,"Error", "Eroare afisare tabel testGrila!");
    }else{
        qDebug() << "Afisare cu succes tabel testGrila!";
    }
    delete qry;
}

void DBTestGrila::viewTable_score(QTableView *tableView)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT * FROM score ORDER BY Scor DESC");
    qry->exec();
    model->setQuery(*qry);
    tableView->setModel(model);
    qDebug() << (model->rowCount());

    if(!qry->exec()){
        qDebug() << "Eroare afisare tabel" << qry->lastError();
        QMessageBox::warning(nullptr,"Error", "Eroare afisare tabel score!");
    }else{
        qDebug() << "Afisare cu succes tabel score!";
    }
    delete qry;
}

void DBTestGrila::search_score(const QString name, const QString date, QTableView *tableView)
{
    QSqlQueryModel *modelScore = new QSqlQueryModel();
    QSqlQuery *qryScore = new QSqlQuery(db);
    // interogarea SQL dinamic
    QString query = "SELECT * FROM score WHERE 1=1";  // 1=1 este o conditie care va ajuta la adăugarea dinamică a condițiilor
    if (!name.isEmpty()) {
        query += " AND Nume = :name";
    }
    if (!date.isEmpty()) {
        query += " AND Data = :date";
    }
    qryScore->prepare(query);
    qryScore->bindValue(":name", name);
    qryScore->bindValue(":date", date);

    if (!qryScore->exec()) {
        qDebug() << "Eroare la selectarea din tabela score!" << qryScore->lastError().text();
        return;
    }
    modelScore->setQuery(*qryScore);
    tableView->setModel(modelScore);
    if (modelScore->rowCount() == 0) {
        QMessageBox::information(nullptr, "Informație", "Nu au fost găsite rezultate pentru această căutare.");
    }
    delete qryScore;
}

void DBTestGrila::search_testGrila(const QString name, QTableView *tableView){
    QSqlQueryModel *modelTestGrila = new QSqlQueryModel();
    QSqlQuery *qryTestGrila = new QSqlQuery(db);
    qryTestGrila->prepare("SELECT * FROM testGrila WHERE Name = :name");
    qryTestGrila->bindValue(":name", name);
    qryTestGrila->exec();
    modelTestGrila->setQuery(*qryTestGrila);
    tableView->setModel(modelTestGrila);
    if (modelTestGrila->rowCount() == 0) {
        QMessageBox::information(nullptr, "Informație", "Nu au fost găsite rezultate pentru această căutare.");
    }
    if (!qryTestGrila->exec()) {
        qDebug() << "Eroare la selectarea din tabela testGrila!" << qryTestGrila->lastError().text();
        return;
    }
    delete qryTestGrila;
}

QString DBTestGrila::convertListToString(const QList<QList<QString>>& list)
{
    QList<QString> concatenatedLists; // Listă pentru a stoca sublistele concatenate

    for (const auto& sublist : list) {
        concatenatedLists.append(sublist.join(", "));
    }
    return concatenatedLists.join("\n");
}

void DBTestGrila::deleteElemTestGrila(const QString name, QTableView *tableView)
{
    QSqlQueryModel *modelTestGrila = new QSqlQueryModel();
    QSqlQuery *qryTestGrila = new QSqlQuery(db);
    qryTestGrila->prepare("DELETE FROM testGrila WHERE Name = :name");
    qryTestGrila->bindValue(":name", name);
    qryTestGrila->exec();
    modelTestGrila->setQuery(*qryTestGrila);
    tableView->setModel(modelTestGrila);
    if (!qryTestGrila->exec()) {
        qDebug() << "Eroare la stergerea din tabela testGrila!" << qryTestGrila->lastError().text();
        return;
    }
    delete qryTestGrila;
}

void DBTestGrila::deleteElemScore(const QString name, QTableView *tableView)
{
    QSqlQueryModel *modelScore = new QSqlQueryModel();
    QSqlQuery *qryScore = new QSqlQuery(db);
    qryScore->prepare("DELETE FROM score WHERE Nume = :name");
    qryScore->bindValue(":name", name);
    qryScore->exec();
    modelScore->setQuery(*qryScore);
    tableView->setModel(modelScore);
    if (!qryScore->exec()) {
        qDebug() << "Eroare la stergerea din tabela score!" << qryScore->lastError().text();
        return;
    }
    delete qryScore;
}

void DBTestGrila::joinTable(QTableView *tableView)
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT testGrila.Name, testGrila.Question, testGrila.Answer1, testGrila.Answer2, "
                 "testGrila.Answer3, testGrila.Answer4, testGrila.AnswerCorrect, "
                 "testGrila.AnswerSelected, score.Scor, score.Data "
                 "FROM testGrila "
                 "INNER JOIN score ON testGrila.Name = score.Nume");
    if (!qry->exec()) {
        qDebug() << "Eroare join: " << qry->lastError().text();
        return;
    }
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(*qry);
    tableView->setModel(model);
    delete qry;  // Curățăm resursele alocate pentru interogare.
}

