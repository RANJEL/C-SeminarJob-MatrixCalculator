/// Header for class CSparseMatrix
/** \file sparseMatrix.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef SPARSEMATRIX_H
#define	SPARSEMATRIX_H

#include "matrix.h"
#include "exceptionInOperation.h"
#include "denseMatrix.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;
class CDenseMatrix;

/// Reprezentation of sparse matrix

/** This is not abstract class anymore. Sparse matrix is stored in vector of TElem, which consists of coordinates of non-zero value and value itself. */
class CSparseMatrix : public CMatrix {
public:
    CSparseMatrix(const string& name, int rows, int columns); ///< Constructor \param name Name of the matrix. \param rows Number of the rows of the matrix. \param columns Number of the columns of the matrix.
    CSparseMatrix(const CSparseMatrix& matrix); ///< Copy constructor
    virtual CMatrix* clone() const;
    virtual CSparseMatrix& operator=(const CSparseMatrix& matrix); ///< Overloaded operator= because of the Rule of three
    virtual ~CSparseMatrix();

    /** This constructor will be called when you get into CDenseMatrix::getSparseMatrix */
    CSparseMatrix(const CDenseMatrix& matrix); ///< Important type of constructor that allows CSparseMatrix to create itself by CDenseMatrix
    virtual CMatrix* getDenseMatrix() const;
    virtual CMatrix* getSparseMatrix() const;
    /** This method will be called when you get into  CDenseMatrix(const CSparseMatrix&) */
    double** convertFromSparseToDense() const;  ///< Method to convert CSparseMatrix data to CDenseMatrix data
    /** \return From "this" matrix(M x N) generates new matrix(M-1 x N-1) without row "skipRow" and column "skipColumn" \param skipRow Row to skip in submatrix \param skipColumn Column to skip in submatrix */
    CMatrix* getSubmatrix(int skipRow, int skipColumn) const; ///< Auxiliary method used in calculation of inversion

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

    /// One non-zero value of CSparceMatrix
    /** One element of CSparceMatrix, which consists of coordinates of non-zero value and value itself. */
    struct TElem {
        TElem(int rowIndex, int columnIndex, double value); ///< Constructor \param rowIndex First index of the element \param columnIndex Second index of the element \param value Non-zero value, that is stored in this coordinates
        bool operator<(const TElem& second) const; ///< Compare method used while lower_bounding vector

        int m_rowIndex; ///< First index of the element
        int m_columnIndex; ///< Second index of the element
        double m_value; ///< Non-zero value, that is stored in this coordinates
    };

    virtual void print(ostream& os) const; 
    void printCoordinates() const; ///< Debug alternative of CSparseMatrix::print() that writes out elements in TElem concept
    /** \param where Vector, where to search \param elemRows  First index of searched element \param elemColumns Second index of searched element \return iterator to possition of element in vector or vector.end() if vector doesn't contain that element */
    vector<TElem>::const_iterator searchElement(const vector<TElem>& where, int elemRows, int elemColumns) const; ///< Auxiliary method to search using lower_bound in vector

    vector<TElem> m_matrix; ///< Sparse matrix representation, that is vector of coordinates (TElem)
};

#endif	/* SPARSEMATRIX_H */