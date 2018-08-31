/// Implementation of matrixDatabase.h
/** \file matrixDatabase.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "matrixDatabase.h"

CMatrixDatabase::CMatrixDatabase() {

}

CMatrixDatabase::~CMatrixDatabase() {
    for (auto it = m_database.begin(); it != m_database.end(); it++)
        delete it->second;
}

bool CMatrixDatabase::createMatrix(bool isDense, const string& name, int rows, int columns) {
    if (containMatrix(name))
        return false;

    CMatrix* newMatrix;

    try {
        if (isDense)
            newMatrix = new CDenseMatrix(name, rows, columns);
        else newMatrix = new CSparseMatrix(name, rows, columns);
    } catch (...) {
        // catch bad_aloc if matrix is too large
        return false;
    }

    newMatrix->fillMatrix();
    m_database[name] = newMatrix;

    return true;
}

bool CMatrixDatabase::addMatrix(CMatrix* matrix, const string& name) {
    if (matrix == NULL)
        return false;

    if (containMatrix(name))
        return false;

    matrix->setName(name);
    m_database[name] = matrix;
    return true;
}

bool CMatrixDatabase::removeMatrix(const string& name) {
    try {
        delete m_database.at(name);
        m_database.erase(name);
        return true;
    } catch (...) { // progtest don't know out_of_range
        return false;
    }
}

CMatrix* CMatrixDatabase::chooseMatrix(const string& name) const {
    try {
        return m_database.at(name);
    } catch (...) { // progtest don't know out_of_range
        return NULL;
    }
}

bool CMatrixDatabase::displayMatrix(const string& name) const {
    try {
        cout << *m_database.at(name);
        return true;
    } catch (...) { // progtest don't know out_of_range
        return false;
    }
}

bool CMatrixDatabase::renameMatrix(const string& oldName, const string& newName) {
    if (containMatrix(newName))
        return false; // New name is already taken.

    auto it = m_database.find(oldName);
    if (it == m_database.end()) // There isn't matrix with this name.
        return false;

    it->second->setName(newName); // Change name.
    m_database[newName] = it->second; // Insert renamed matrix.
    m_database.erase(oldName); // Delete old one from map.
    return true;
}

void CMatrixDatabase::displayDatabase() const {
    for (auto it = m_database.begin(); it != m_database.end(); it++)
        cout << *it->second << endl;
}

bool CMatrixDatabase::isEmpty() const {
    return m_database.empty();
}

void CMatrixDatabase::displayNamesOfMatrices() const {
    cout << "********************************************************************************" << endl;
    cout << "Matrix Database: ";
    for (auto it = m_database.begin(); it != m_database.end(); it++) {
        auto it2 = it;
        if (++it2 == m_database.end())
            cout << it->first << endl;
        else cout << it->first << ", ";
    }
    if (m_database.begin() == m_database.end())
        cout << endl;
    cout << "********************************************************************************" << endl;
    return;
}

void CMatrixDatabase::backUpDatabase(const string& toFile) const {
    system("mkdir backup 2>/dev/null");

    ofstream os(toFile); // If there is some file, ofstream will replace him with new empty one.
    os << fixed << setprecision(3);
    for (auto it = m_database.begin(); it != m_database.end(); it++)
        os << setw(7) << *it->second;
    os.close();
    return;
}

bool CMatrixDatabase::addFromBackupToDatabase(const string& fromFile) {
    system("mkdir backup 2>/dev/null");
    system("touch ./backup/backupFile.txt 2>/dev/null");

    ifstream is(fromFile);
    if (!is.good())
        return false;

    /* delete old matrices */
    while (!isEmpty())
        removeMatrix(m_database.begin()->first);

    string firstLine;
    string trashString;
    char trashChar;
    istringstream iss;

    string name;
    int rows;
    int columns;
    CDenseMatrix* newMatrix;
    double** data;


    while (is.good()) {
        try {
            is >> trashString;
        } catch (...) {
            is.close();
            return false;
        }
        getline(is, firstLine);
        if (firstLine == "") // the file is empty
            break;
        firstLine = firstLine.substr(1); // don't want to start with space
        try {
            size_t bracketPos = firstLine.rfind("(");
            if (bracketPos != string::npos && bracketPos > 1) {
                name = firstLine.substr(0, bracketPos - 1);
                iss.clear();
                iss.str(firstLine.substr(bracketPos));
                iss >> trashChar >> rows >> trashChar >> columns;
                if (!iss.good())
                    is.setstate(ios::failbit);
            } else is.setstate(ios::failbit);
        } catch (...) { // progtest don't know out_of_range
            is.setstate(ios::failbit);
        }

        if (is.good()) {
            /* create new data */
            data = new double*[rows];
            for (int i = 0; i < rows; i++)
                data[i] = new double[columns];
            /* try to fill them */
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < columns; j++)
                    is >> data[i][j];
            if (is.good()) {
                newMatrix = new CDenseMatrix(name, rows, columns, data);
                addMatrix(newMatrix, name);
            } else {
                /* dealocate tmp data */
                for (int i = 0; i < rows; i++)
                    delete [] data[i];
                delete [] data;
            }
        }
    }
    is.close();
    return true; // backup file was found
}

bool CMatrixDatabase::containMatrix(const string& name) const {
    if (m_database.find(name) == m_database.end())
        return false;
    else return true;
}




