/// Header for abstract class CMatrix
/** \file matrix.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef MATRIX_H
#define	MATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

/// Abstract matrix

/** An abstract matrix (M*N), where M is the number of rows and N is the number of columns. **/
class CMatrix {
public:
    CMatrix(const string& name, int rows, int columns); ///< Constructor \param name Name of the matrix. \param rows Number of the rows of the matrix. \param columns Number of the columns of the matrix. 
    // rule of three
    virtual ~CMatrix(); ///< Destructor
    /** Instead of copy constructor CMatrix(const CMatrix& matrix) => virtual CMatrix* clone() const = 0; to be able to create copy regardless of a real subclass. */
    virtual CMatrix* clone() const = 0; ///< Clone function
    /** If getDenseMatrix is called in CDenseMatrix, it will just call clone, but if it is called in CSparseMatrix, then CDenseMatrix(CSparseMatrix) will be called. And oppositely. */
    virtual CMatrix* getDenseMatrix() const = 0; ///< Make copy and I want it to be CDenseMatrix
    virtual CMatrix* getSparseMatrix() const = 0; ///< Make copy and I want it to be CSparseMatrix
    string getName() const; ///< Getter to m_name
    int getRows() const; ///< Getter to m_rows
    int getColumns() const; ///< Getter to m_columns
    void setName(const string& newName); ///< Setter to m_name

    /** \param matrix CDenseMatrix or CSparseMatrix, which you want to add to "this" \return New object and keep old matrices without changes => const */
    virtual CMatrix* add(const CMatrix& matrix) const = 0; ///< Add two matrices
    virtual CMatrix* subtract(const CMatrix& matrix) const = 0; ///< Subtract two matrices
    virtual CMatrix* multiply(const CMatrix& matrix) const = 0; ///< Multiply "this" matrix by "matrix"
    virtual CMatrix* transpose() const = 0; ///< Calculate transposed matrix ("this")
    virtual CMatrix* inversion() const = 0; ///< Calculate inversion of "this" matrix
    virtual double determinant() const = 0; ///< Calculate determinant of "this" matrix
    virtual int rank() const = 0; ///< Calculate rank of "this" matrix"
    virtual CMatrix* gem() const = 0; ///< Apply Gaussian elimination method to "this" matrix
    virtual CMatrix* commentedGem() const = 0; ///< Apply GEM and comment every step

    virtual void fillMatrix() = 0; ///< Call TUI and fill "this" matrix
    friend ostream& operator<<(ostream& os, const CMatrix& matrix); ///< Write out the content of "this" matrix

protected:
    virtual void print(ostream& os) const; ///< virtual print used in operator<<
    /** \param value Double value to compare \return True if value is almost zero */
    bool equalsZero(double value) const; ///< Compare double with zero

    string m_name; ///< Name of the matrix.
    int m_rows; ///< Number of the rows of the matrix.
    int m_columns; ///< Number of the columns of the matrix.
};


#endif	/* MATRIX_H */

