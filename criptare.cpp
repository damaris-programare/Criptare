#include "criptare.h"
#include "struct_questions.h"

#include <string>
#include <iostream>
#include <fstream>

#include <cryptopp/files.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <QFileInfo>


using namespace std;

criptare::criptare()
{
    ///string numeFisier = "Chestionar.txt";
    ///string cheie = "cheie";
    //// criptareFisier(numeFisier, cheie);
}

criptare::~criptare()
{

}

void criptare::criptareFisier(const QFileInfo &fileInfo, const QString& cheie)
{
    string fileInfoStr = fileInfo.filePath().toStdString(); // Convertim QString in string
    string cheieStr = cheie.toStdString(); // Convertim QString in string

    ifstream fisierIn(fileInfoStr);
    ofstream fisierOut("Criptat_" + fileInfo.fileName().toStdString());

    if (!fisierIn.is_open()) {
        cerr << "Eroare la deschiderea fisierului normal \n";
        return;
    }

    if (!fisierOut.is_open()) {
        cerr << "Eroare la deschiderea fisierului de iesire Criptat...\n";
        return;
    }

    char caracter;
    int index = 0;

    while (fisierIn.get(caracter)) {
        caracter ^= cheieStr[index % cheieStr.length()]; //Criptare XOR
        fisierOut.put(caracter);

        index++;
    }

    fisierIn.close();
    fisierOut.close();
    cout << "Fisierul " << fileInfoStr << " a fost criptat cu succes.\n";
}

///const QString& cheie;
///QString& cheie;
///QString  *cheie;               ////questionBlock *qBlock   ///const questionBlock& qBlock

void criptare::decriptareFisier(const QFileInfo &fileInfo, vector <questionBlock> *qBlock, const QString cheie)
{
    string fileInfoStr = fileInfo.filePath().toStdString(); // Convertim QString in std::string
    string cheieStr = cheie.toStdString(); // Convertim QString in std::string

    ifstream fisierIn("Criptat_" + fileInfoStr);

    if (!fisierIn.is_open()) {
        cerr << "Eroare la deschiderea fisierului criptat \n";
        return;
    }

    char caracter;
    int index = 0;
    questionBlock *q;

    while (fisierIn.get(caracter)) {
        caracter ^= cheieStr[index % cheieStr.length()]; //Decriptare XOR
        while(caracter != '\n'){
            q += caracter;   ////citire caracter si decriptare caracter
        }
        switch (index) {
        case 0:
            q->q1 += caracter;
            index++;
            break;
        case 1:
            q->r1 += caracter;
            index++;
            break;
        case 2:
            q->r2 += caracter;
            index++;
            break;
        case 3:
            q->r3 += caracter;
            index++;
            break;
        case 4:
            q->r4 += caracter;
            index++;
            break;
        case 5:
            q->r_corect += caracter;
            qBlock->push_back(*q);
            q = {};
            index = 0;
            break;
        default:
            break;
        }
    }
    fisierIn.close();
    cout << "Fisierul " << fileInfoStr << " a fost decriptat cu succes.\n";
}
