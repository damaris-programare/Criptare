#ifndef CRIPTARE_H
#define CRIPTARE_H

#include "struct_questions.h"

#include <string>
#include <QFileInfo>


using namespace std;

class criptare
{
public:
    criptare();
    ~criptare();

    void criptareFisier(const QFileInfo &fileInfo, const QString &cheie);
    void decriptareFisier(const QFileInfo &numeFisier, vector <questionBlock> *qBlock, const QString cheie);

private:
   /// QString cheie = "cheie";
};

#endif // CRIPTARE_H
