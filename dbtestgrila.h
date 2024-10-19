#ifndef DBTESTGRILA_H
#define DBTESTGRILA_H

#include "struct_questions.h"

#include <QList>
#include <QSqlDatabase>
#include <QString>
#include <QTableView>

using namespace std;

class DBTestGrila
{
public:
    DBTestGrila();

    void createTable();
    void dropTable();
    void insertInto(int punctaj, vector<questionBlock>& questions, QString name, QList<QList<QString> > selectedAnswers);
    void viewTable_testGrila(QTableView *tableView);
    void viewTable_score(QTableView *tableView);
    void search_score(const QString name, const QString date, QTableView *tableView);
    void search_testGrila(const QString name, QTableView *tableView);
    QString convertListToString(const QList<QList<QString> > &list);
    void deleteElemTestGrila(const QString name, QTableView *tableView);
    void deleteElemScore(const QString name, QTableView *tableView);
    void joinTable(QTableView *tableView);
private:
    QSqlDatabase db;
    QTableView tableView;
};

#endif // DBTESTGRILA_H
