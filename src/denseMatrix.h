/// Header for class CDenseMatrix
/** \file denseMatrix.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef DENSEMATRIX_H
#define	DENSEMATRIX_H

#include "matrix.h"
#include "exceptionInOperation.h"
#include "sparseMatrix.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;
class CSparseMatrix; // I need to declare this class because of CDenseMatrix(const CSparseMatrix& matrix), even though I included "sparseMatrix.h" before.
struct TElem2; // Because I want to keep CSparseMatrix::Elem protected, I prefer to create this temporary equivalent of TElem

/// Reprezentation of dense matrix
/** This is not abstract class anymore. Dense matrix is stored in a two-dimensional array. */

class CDenseMatrix : public CMatrix {
public:
    CDenseMatrix(const string& name, int rows, int columns); ///< Constructor \param name Name of the matrix. \param rows Number of the rows of the matrix. \param columns Number of the columns of the matrix.
    CDenseMatrix(const string& name, int rows, int columns, double** data); ///< Constructor created for CMatrixDatabase \param name Name of the matrix. \param rows Number of the rows of the matrix. \param columns Number of the columns of the matrix. \param data Pointer to two-dimensional array
    CDenseMatrix(const CDenseMatrix& matrix); ///< Copy constructor
    virtual CMatrix* clone() const;
    virtual CDenseMatrix& operator=(const CDenseMatrix& matrix); ///< Overloaded operator= because of the Rule of three
    virtual ~CDenseMatrix();

    /** This constructor will be called when you get into CSparseMatrix::getDenseMatrix */
    CDenseMatrix(const CSparseMatrix& matrix); ///< Important type of constructor that allows CDenseMatrix to create itself by CSparseMatrix
    virtual CMatrix* getDenseMatrix() const;
    virtual CMatrix* getSparseMatrix() const;
    /** This method will be called when you get into  CSparseMatrix(const CDenseMatrix&) */
    vector<TElem2> convertFromDenseToSparse() const; ///< Method to convert CDenseMatrix data to CSparseMatrix data

    virtual CMatrix* add(const CMatrix& matrix) const;
    virtual CMatrix* subtract(const CMatrix& matrix) const;
    virtual CMatrix* multiply(const CMatrix& matrix) const;
    virtual CMatrix* transpose() const;
    virtual CMatrix* inversion() const;
    virtual double determinant() const;
    virtual int rank() const;
    virtual CMatrix* gem() const;
    virtual CMatrix* commentedGem() const;

    virtual void fillMatrix();

protected:
    virtual void print(ostream& os) const;

    void toRowEchelonForm(ostringstream& os) const; ///< Auxiliary method called by CDenseMatrix::gem() and CDenseMatrix::commentedGem() that converts matrix to the row echelon form \param os Output string stream, where comments are stored
    void makeThereZero(int whichRow, int whichColumn, ostringstream& os) const; ///< Set into the specific element of matrix zero, using a linear combination \param whichRow Number of element's first index \param whichColumn Numbers of elements's second index \param os Ostringstream for commented mode
    void swapRows(int row1, int row2, ostringstream& os) const; ///< Swap two rows of the matrix.
    bool isZeroLine(int row) const; ///< Indicate, whether is a row filled only with zeros \param row Tested row \return True, if there are only zeros and False if there is 1 or more non-zeros.
    bool whereIsPivot(int whichRow, int& result) const; ///< Find out where it pivot(first non-zero element in the row) \param whichRow Tested row \param result Possition of pivot = result \return Whether is pivot valid
    void makeBelowZero(int whichRow, int whichColumn, ostringstream& os) const; ///< Method used in third wave of GEM

    double** m_matrix; ///< Dense matrix representation, that is two-dimensional array
};

/// Auxiliary structure, equivalent of CSparseMatrix::TElem

/** I needed to get to TElem but I wanted to keep that protected. That's why I chosed way to create this temporary equivalent of CSparseMatrix::TElem. Right there look for documentation. */
struct TElem2 {
    TElem2(int rowIndex, int columnIndex, double value);
    int m_rowIndex;
    int m_columnIndex;
    double m_value;
};

#endif	/* DENSEMATRIX_H */