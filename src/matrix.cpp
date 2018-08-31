/// Implementation of matrix.h
/** \file matrix.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "matrix.h"

CMatrix::CMatrix(const string& name, int rows, int columns) : m_name(name), m_rows(rows), m_columns(columns) {

}

CMatrix::~CMatrix() {

}

void CMatrix::print(ostream& os) const {
    os << "Matrix " << m_name << " (" << m_rows << " x " << m_columns << "):" << endl;
    return;
}

ostream& operator<<(ostream& os, const CMatrix& matrix) {
    matrix.print(os);
    return os;
}

string CMatrix::getName() const {
    return m_name;
}

int CMatrix::getRows() const {
    return m_rows;
}

int CMatrix::getColumns() const {
    return m_columns;
}

void CMatrix::setName(const string& newName) {
    m_name = newName;
    return;
}

bool CMatrix::equalsZero(double value) const {
    if (fabs(value) < 1e-10)
        return true;
    else return false;
}



