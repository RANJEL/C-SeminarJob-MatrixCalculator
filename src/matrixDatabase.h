/// Header for class CMatrixDatabase
/** \file matrixDatabase.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef MATRIXDATABASE_H
#define	MATRIXDATABASE_H

#include "denseMatrix.h"
#include "sparseMatrix.h"

#include <map>
#include <fstream>
#include <sstream>

using namespace std;

/// Matrix database
//* Matrix database for easier management of matrices */

class CMatrixDatabase {
public:
    CMatrixDatabase(); ///< Constructor that initializes map<string, CMatrix*>
    ~CMatrixDatabase(); ///< Destructor
    /** \param isDense True if the new matrix should be dense. False if sparse. \param name The name of the new matrix \param rows The number of rows of the new matrix \param columns The number of columns of the new matrix \return Whether the creation was successful */
    bool createMatrix(bool isDense, const string& name, int rows, int columns); ///< Create a new matrix and store it in the database
    bool addMatrix(CMatrix* matrix, const string& name); ///< Add a new matrix to the database (internaly used by CTui) \param matrix Pointer to CMatrix = matrix to add \param name Name of the matrix you want to add
    bool removeMatrix(const string& name); ///< Remove the matrix from the database
    CMatrix* chooseMatrix(const string& name) const; ///< Choose the matrix with specific name from the database \param name Name of the matrix to choose \return Pointer to found matrix in database, or NULL if matrix with that name was not found.
    bool displayMatrix(const string& name) const; ///< Display the matrix from the database
    bool renameMatrix(const string& oldName, const string& newName); ///< Rename the matrix \param oldName Old name of matrix that is in the database now \param newName New name of matrix, that will be copied and stored in the database (oldName will be deleted) 

    bool containMatrix(const string& name) const; ///< Indicate whether the database contains the matrix
    void displayDatabase() const; ///< Display all matrices stored in the database
    bool isEmpty() const; ///< Indicate whether the database is empty 
    void displayNamesOfMatrices() const; ///< Go throw all matrices in database and display only their names
    void backUpDatabase(const string& toFile = "./backup/backupFile.txt") const; ///< Take all matrices just now stored in the database and store them into a file \param toFile File, where to store the actual database. It's defaultly ./backup/backupFile.txt. It will allways try to create this file.
    bool addFromBackupToDatabase(const string& fromFile = "./backup/backupFile.txt"); ///< Delete the actual database and replace it with the database stored in a file \param fromFile File, where data should be stored. It's defaultly ./backup/backupFile.txt. If the file doesn't exist, loading will fail.

private:
    map<string, CMatrix*> m_database;
};

#endif	/* MATRIXDATABASE_H */

