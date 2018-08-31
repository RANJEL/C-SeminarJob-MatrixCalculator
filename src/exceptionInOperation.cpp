/// Implementation of exceptionInOperation.h
/** \file exceptionInOperation.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "exceptionInOperation.h"

CExceptionInOperation::CExceptionInOperation(const string& where) : m_operation(where) {
}

ostream& operator<<(ostream& os, const CExceptionInOperation& exception) {
    os << endl;
    if (exception.m_operation == "add")
        os << "Add error: Matrices must have the same number of rows and columns!" << endl;
    else if (exception.m_operation == "subtract")
        os << "Subtract error: Matrices must have the same number of rows and columns!" << endl;
    else if (exception.m_operation == "multiply")
        os << "Multiply error: The first matrix must have the same number of columns as the second matrix rows!" << endl;
    else if (exception.m_operation == "inversion")
        os << "Inversion error: If you want to do an inversion, the matrix must be square (matrix N x N) and the determinant of the matrix can not equal zero!" << endl;
    else if (exception.m_operation == "determinant")
        os << "Determinant error: A determinant can be calculated only from the square matrix (N x N)!" << endl;
    else os << "Unexpected error!" << endl;
    return os;
}

