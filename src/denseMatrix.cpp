/// Implementation of denseMatrix.h
/** \file denseMatrix.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "denseMatrix.h"

CDenseMatrix::CDenseMatrix(const string& name, int rows, int columns) : CMatrix(name, rows, columns) {
    m_matrix = new double*[rows];
    for (int i = 0; i < rows; i++)
        m_matrix[i] = new double[columns];

    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            m_matrix[i][j] = 0;
}

CDenseMatrix::CDenseMatrix(const string& name, int rows, int columns, double** data) : CMatrix(name, rows, columns), m_matrix(data) {

}

CDenseMatrix::CDenseMatrix(const CDenseMatrix& matrix) : CMatrix(matrix.m_name, matrix.m_rows, matrix.m_columns) {
    m_matrix = new double*[m_rows];
    for (int i = 0; i < m_rows; i++)
        m_matrix[i] = new double[m_columns];

    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            m_matrix[i][j] = matrix.m_matrix[i][j];
}

CMatrix* CDenseMatrix::clone() const {
    return new CDenseMatrix(*this);
}

CDenseMatrix& CDenseMatrix::operator=(const CDenseMatrix& matrix) {
    if (this == &matrix)
        return *this;

    for (int i = 0; i < m_rows; i++)
        delete [] m_matrix[i];
    delete [] m_matrix;

    m_name = matrix.m_name;
    m_rows = matrix.m_rows;
    m_columns = matrix.m_columns;
    m_matrix = new double*[m_rows];
    for (int i = 0; i < m_rows; i++)
        m_matrix[i] = new double[m_columns];

    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            m_matrix[i][j] = matrix.m_matrix[i][j];

    return *this;
}

CDenseMatrix::~CDenseMatrix() {
    for (int i = 0; i < m_rows; i++)
        delete [] m_matrix[i];
    delete [] m_matrix;
}

CDenseMatrix::CDenseMatrix(const CSparseMatrix& matrix) : CMatrix(matrix.getName(), matrix.getRows(), matrix.getColumns()) {
    m_matrix = matrix.convertFromSparseToDense();
}

CMatrix* CDenseMatrix::getDenseMatrix() const {
    return clone();
}

CMatrix* CDenseMatrix::getSparseMatrix() const {
    return new CSparseMatrix(*this);
}

TElem2::TElem2(int rowIndex, int columnIndex, double value) : m_rowIndex(rowIndex), m_columnIndex(columnIndex), m_value(value) {

}

vector<TElem2> CDenseMatrix::convertFromDenseToSparse() const {
    vector<TElem2> tmp;

    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            if (!equalsZero(m_matrix[i][j]))
                tmp.push_back(TElem2(i, j, m_matrix[i][j]));

    return tmp;
}

void CDenseMatrix::print(ostream& os) const {
    CMatrix::print(os);
    os << fixed << setprecision(3);
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_columns; j++)
            if (fabs(m_matrix[i][j]) < 5e-4) // then I don't want to see -0.000
                os << setw(7) << 0.000 << "  ";
            else
                os << setw(7) << m_matrix[i][j] << "  ";

        os << endl;
    }
    return;
}

void CDenseMatrix::fillMatrix() {
    cout << "Enter a dense matrix " << m_name << ": (" << m_rows << " x " << m_columns << ")" << endl;

    string inputString;
    istringstream iss;
    bool error;
    double value;
    string trash;

    for (int i = 0; i < m_rows; i++) {
        do {
            error = false;
            cout << "Enter the " << i + 1 << ". row divided by spaces: ";
            getline(cin, inputString);
            iss.clear();
            iss.str(inputString);
            for (int j = 0; j < m_columns && !error; j++) {
                if (iss >> value)
                    m_matrix[i][j] = value;
                else error = true;
            }
            if (iss >> trash && trash != "")
                error = true;
        } while (error);

    }
    return;
}

CMatrix* CDenseMatrix::add(const CMatrix& matrix) const {
    if (m_rows != matrix.getRows() || m_columns != matrix.getColumns())
        throw CExceptionInOperation("add");

    // I don't know if in matrix is DenseMatrix or SparseMatrix
    // and I want to avoid RTTI => virtual getDenseMatrix  
    CDenseMatrix* newMatrix = (CDenseMatrix*) matrix.getDenseMatrix();

    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            newMatrix->m_matrix[i][j] += m_matrix[i][j];

    return newMatrix;
}

CMatrix* CDenseMatrix::subtract(const CMatrix& matrix) const {
    if (m_rows != matrix.getRows() || m_columns != matrix.getColumns())
        throw CExceptionInOperation("subtract");

    CDenseMatrix* copiedMatrix = (CDenseMatrix*) matrix.getDenseMatrix();
    CDenseMatrix* newMatrix = (CDenseMatrix*) clone();

    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            newMatrix->m_matrix[i][j] -= copiedMatrix->m_matrix[i][j];

    delete copiedMatrix;
    return newMatrix;
}

CMatrix* CDenseMatrix::multiply(const CMatrix& matrix) const {
    if (m_columns != matrix.getRows())
        throw CExceptionInOperation("multiply");

    CDenseMatrix* matrix2 = (CDenseMatrix*) matrix.getDenseMatrix();
    CDenseMatrix* result = new CDenseMatrix("RES", m_rows, matrix.getColumns());

    double sum;
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < matrix2->m_columns; j++) {
            sum = 0;
            for (int k = 0; k < matrix2->m_rows; k++)
                sum += m_matrix[i][k] * matrix2->m_matrix[k][j];
            result->m_matrix[i][j] = sum;
        }
    }

    delete matrix2;
    return result;
}

CMatrix* CDenseMatrix::transpose() const {
    CDenseMatrix* newMatrix = new CDenseMatrix("RES", m_columns, m_rows);
    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++)
            newMatrix->m_matrix[j][i] = m_matrix[i][j];

    return newMatrix;
}

CMatrix* CDenseMatrix::inversion() const {
    if (m_rows != m_columns || equalsZero(determinant()))
        throw CExceptionInOperation("inversion");

    CDenseMatrix* resultTransposed = new CDenseMatrix("RES", m_rows, m_columns);
    double det = determinant();

    for (int i = 0; i < m_rows; i++)
        for (int j = 0; j < m_columns; j++) {

            CSparseMatrix* tmpMatrix = (CSparseMatrix*) getSparseMatrix();
            CDenseMatrix* subMatrix = (CDenseMatrix*) tmpMatrix->getSubmatrix(i, j);

            resultTransposed->m_matrix[i][j] = pow(-1, i + j) * subMatrix->determinant() / det;

            delete tmpMatrix;
            delete subMatrix;
        }

    CDenseMatrix* resultFinal = (CDenseMatrix*) resultTransposed->transpose();
    delete resultTransposed;
    return resultFinal;
}

double CDenseMatrix::determinant() const {
    if (m_rows != m_columns)
        throw CExceptionInOperation("determinant");

    double determinant = 1;
    CDenseMatrix* tmpMatrix = (CDenseMatrix*) clone();
    CDenseMatrix* tmpGEMMatrix = (CDenseMatrix*) tmpMatrix->gem();

    for (int i = 0; i < m_rows; i++)
        determinant *= tmpGEMMatrix->m_matrix[i][i];

    delete tmpMatrix;
    delete tmpGEMMatrix;
    return determinant;
}

int CDenseMatrix::rank() const {
    int rank = 0;
    CDenseMatrix* tmpMatrix = (CDenseMatrix*) clone();
    CDenseMatrix* tmpGEMMatrix = (CDenseMatrix*) tmpMatrix->gem();

    for (int i = 0; i < m_rows; i++)
        if (!tmpGEMMatrix->isZeroLine(i))
            rank++;

    delete tmpMatrix;
    delete tmpGEMMatrix;
    return rank;
}

CMatrix* CDenseMatrix::gem() const {
    ostringstream os;
    CDenseMatrix* newMatrix = (CDenseMatrix*) clone();

    newMatrix->toRowEchelonForm(os);

    return newMatrix;
}

CMatrix* CDenseMatrix::commentedGem() const {
    ostringstream os;
    CDenseMatrix* newMatrix = (CDenseMatrix*) clone();

    newMatrix->toRowEchelonForm(os);
    cout << os.str();

    return newMatrix;
}

void CDenseMatrix::toRowEchelonForm(ostringstream& os) const {
    double** M = m_matrix;
    int rowCount = m_rows;
    int columnCount = m_columns;
    os << "================================================================================" << endl << *this
            << "================================================================================" << endl;
    // Let's start creating triangular shape
    for (int j = 0; j < columnCount; j++)
        for (int i = j + 1; i < rowCount; i++)
            makeThereZero(i, j, os);

    // Now I have to get all zero lines below non-zero lines
    for (int i = 0; i < rowCount; i++)
        if (isZeroLine(i)) {
            for (int j = i + 1; j < rowCount; j++)
                if (!isZeroLine(j))
                    swapRows(i, j, os);
        }
    // Finally it's necessary to fix this problem
    /* 1 2 3
     * 0 0 1
     * 0 0 3 */
    // If a pivot is the first non-zero number in a row, the number below him must be zero.
    int index = -1;
    for (int i = 0; i < rowCount - 1; i++)
        if (whereIsPivot(i, index) && !equalsZero(M[i + 1][index]))
            makeBelowZero(i, index, os);
    os << "================================================================================" << endl << "~ " << *this
            << "================================================================================" << endl;

    return;
}

void CDenseMatrix::makeThereZero(int i, int j, ostringstream& os) const {
    double** M = m_matrix;
    int columnCount = m_columns;

    if (equalsZero(M[i][j])) {
        // There is already zero.
        return;
    }

    if (equalsZero(M[j][j])) {
        // I can't divide by zero.
        swapRows(i, j, os);
        return;
    }

    double multiplyRowBy = -(M[i][j] / M[j][j]);
    os << "$ Add to the " << i + 1 << ". row the " << j + 1 << ". row multiplied by " << multiplyRowBy << endl;

    for (int column = 0; column < columnCount; column++)
        M[i][column] += M[j][column] * multiplyRowBy;

    os << "~ " << *this << "--------------------------------------------------------------------------------" << endl;
    return;
}

void CDenseMatrix::swapRows(int row1, int row2, ostringstream& os) const {
    double** M = m_matrix;
    int columnCount = m_columns;

    os << "$ Swap the " << row2 + 1 << ". and the " << row1 + 1 << ". row" << endl;

    for (int i = 0; i < columnCount; i++)
        swap(M[row1][i], M[row2][i]);

    os << "~ " << *this << "--------------------------------------------------------------------------------" << endl;
    return;
}

bool CDenseMatrix::isZeroLine(int row) const {
    bool result = true;
    double** M = m_matrix;
    int columnCount = m_columns;

    for (int col = 0; col < columnCount && result; col++)
        if (!equalsZero(M[row][col]))
            result = false;

    return result;
}

bool CDenseMatrix::whereIsPivot(int whichRow, int& result) const {
    bool isThere = false;
    double** M = m_matrix;
    int columnCount = m_columns;

    int i = 0;
    while (i < columnCount && isThere == false) {
        if (!equalsZero(M[whichRow][i])) {
            isThere = true;
            result = i;
        }
        i++;
    }
    return isThere;
}

void CDenseMatrix::makeBelowZero(int whichRow, int whichColumn, ostringstream& os) const {
    double** M = m_matrix;
    int columnCount = m_columns;

    double multiplyRowBy = -(M[whichRow + 1][whichColumn] / M[whichRow][whichColumn]);
    os << "$ Add to the " << whichRow + 2 << ". row the " << whichRow + 1 << ". row multiplied by " << multiplyRowBy << endl;

    for (int column = 0; column < columnCount; column++)
        M[whichRow + 1][column] += M[whichRow][column] * multiplyRowBy;

    os << "~ " << *this << "--------------------------------------------------------------------------------" << endl;
    return;
}
