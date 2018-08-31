/// Implementation of sparseMatrix.h
/** \file sparseMatrix.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "sparseMatrix.h"

CSparseMatrix::TElem::TElem(int rowIndex, int columnIndex, double value) : m_rowIndex(rowIndex), m_columnIndex(columnIndex), m_value(value) {
}

bool CSparseMatrix::TElem::operator<(const TElem& second) const {
    if (m_rowIndex == second.m_rowIndex)
        return m_columnIndex < second.m_columnIndex;
    else return m_rowIndex < second.m_rowIndex;
}

CSparseMatrix::CSparseMatrix(const string& name, int rows, int columns) : CMatrix(name, rows, columns) {
    /* the vector will be initialized to the size 0, I suppose the matrix to have only zeros now */
}

CSparseMatrix::CSparseMatrix(const CSparseMatrix& matrix) : CMatrix(matrix.m_name, matrix.m_rows, matrix.m_columns), m_matrix(matrix.m_matrix) {
}

CMatrix* CSparseMatrix::clone() const {
    return new CSparseMatrix(*this);
}

CSparseMatrix& CSparseMatrix::operator=(const CSparseMatrix& matrix) {
    if (this == &matrix)
        return *this;

    m_name = matrix.m_name;
    m_rows = matrix.m_rows;
    m_columns = matrix.m_columns;
    m_matrix = matrix.m_matrix;

    return *this;
}

CSparseMatrix::~CSparseMatrix() {
    /* the vector will do his job on his own */
}

CSparseMatrix::CSparseMatrix(const CDenseMatrix& matrix) : CMatrix(matrix.getName(), matrix.getRows(), matrix.getColumns()) {
    vector<TElem2> tmp = matrix.convertFromDenseToSparse();
    for (auto it = tmp.begin(); it != tmp.end(); it++) {
        m_matrix.push_back(TElem(it->m_rowIndex, it->m_columnIndex, it->m_value));
    }
}

CMatrix* CSparseMatrix::getDenseMatrix() const {
    return new CDenseMatrix(*this);
}

CMatrix* CSparseMatrix::getSparseMatrix() const {
    return clone();
}

vector<CSparseMatrix::TElem>::const_iterator CSparseMatrix::searchElement(const vector<TElem>& where, int elemRows, int elemColumns) const {
    const double WHATEVER = 13;
    TElem searchedElem(elemRows, elemColumns, WHATEVER);
    // the vector is sorted, so let's search searchedElem
    return lower_bound(where.begin(), where.end(), searchedElem);
}

double** CSparseMatrix::convertFromSparseToDense() const {
    double** newMatrix = NULL; // without NULL the valgrind announced the conditionul jump depends on uninitialised value
    int l_rows = m_rows;
    int l_columns = m_columns;


    newMatrix = new double*[l_rows];
    for (int i = 0; i < l_rows; i++)
        newMatrix[i] = new double[l_columns];

    for (int i = 0; i < l_rows; i++) {
        for (int j = 0; j < l_columns; j++) {
            vector<TElem>::const_iterator it = searchElement(m_matrix, i, j);
            if (it != m_matrix.end() && it->m_rowIndex == i && it->m_columnIndex == j)
                newMatrix[i][j] = it->m_value;
            else newMatrix[i][j] = 0;
        }
    }

    return newMatrix;
}

void CSparseMatrix::printCoordinates() const {
    cout << "Matrix " << m_name << " (" << m_rows << " x " << m_columns << "):" << endl;
    cout << "(i, j, value)" << endl;
    for (auto it = m_matrix.begin(); it != m_matrix.end(); it++)
        cout << fixed << setprecision(3) << "(" << it->m_rowIndex << ", " << it->m_columnIndex << ", " << it->m_value << ")" << endl;

    return;
}

void CSparseMatrix::print(ostream& os) const {
    CMatrix::print(os);
    cout << fixed << setprecision(3);
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_columns; j++) {
            vector<TElem>::const_iterator it = searchElement(m_matrix, i, j);
            if (it != m_matrix.end() && it->m_rowIndex == i && it->m_columnIndex == j) {
                if (fabs(it->m_value) < 5e-4) // then I don't want to see -0.000
                    os << setw(7) << 0.000 << "  ";
                else
                    os << setw(7) << it->m_value << "  ";
            } else os << setw(7) << 0.000 << "  ";
        }
        os << endl;
    }
    return;
}

void CSparseMatrix::fillMatrix() {
    cout << "Enter a sparse matrix " << m_name << ": (" << m_rows << " x " << m_columns << ")" << endl;

    string inputString;
    istringstream iss;
    bool error;
    double value;
    string trash;
    int addedNonZeros;

    for (int i = 0; i < m_rows; i++) {
        do {
            error = false;
            addedNonZeros = 0;
            cout << "Enter the " << i + 1 << ". row divided by spaces: ";
            getline(cin, inputString);
            iss.clear();
            iss.str(inputString);

            for (int j = 0; j < m_columns && !error; j++) {
                if (iss >> value) {
                    if (!equalsZero(value)) {
                        m_matrix.push_back(TElem(i, j, value));
                        addedNonZeros++;
                    }
                } else {
                    // I maybe added a bad row, so I have to regress
                    for (int tmp = 0; tmp < addedNonZeros; tmp++)
                        m_matrix.pop_back();
                    error = true;
                    addedNonZeros = 0;
                }
            }
            if (iss >> trash && trash != "") {
                // I maybe added a bad row, so I have to regress
                for (int tmp = 0; tmp < addedNonZeros; tmp++)
                    m_matrix.pop_back();
                error = true;
            }

        } while (error);

    }
    return;
}

CMatrix* CSparseMatrix::add(const CMatrix& matrix) const {
    if (m_rows != matrix.getRows() || m_columns != matrix.getColumns())
        throw CExceptionInOperation("add");

    CSparseMatrix* newMatrix = (CSparseMatrix*) matrix.getSparseMatrix();
    vector<TElem> tmp;
    /* I will go throw this matrix using iterator it. If I find new coords (that aren't in newMatrix), I will add them
     *  and if I don't find new coords (= same coords in both matrices), I will add their values and save the result to newMatrix. */
    for (auto it = m_matrix.begin(); it != m_matrix.end(); it++) {
        auto pos = searchElement(newMatrix->m_matrix, it->m_rowIndex, it->m_columnIndex);
        if (pos != newMatrix->m_matrix.end() && pos->m_rowIndex == it->m_rowIndex && pos->m_columnIndex == it->m_columnIndex) {
            /* but I don't want a const_iterator, because I can't change where pos points now.. I will use this trick only working for random access containers  */
            vector<TElem>::iterator posNonConst = newMatrix->m_matrix.begin() + (pos - newMatrix->m_matrix.begin());
            posNonConst->m_value += it->m_value; // I found this coordinates in the other matrix
        } else tmp.push_back(*it); // I have to add this coord.
    }

    newMatrix->m_matrix.insert(newMatrix->m_matrix.end(), tmp.begin(), tmp.end());
    sort(newMatrix->m_matrix.begin(), newMatrix->m_matrix.end());

    return newMatrix;
}

CMatrix* CSparseMatrix::subtract(const CMatrix & matrix) const {
    if (m_rows != matrix.getRows() || m_columns != matrix.getColumns())
        throw CExceptionInOperation("subtract");

    CSparseMatrix* copiedMatrix = (CSparseMatrix*) matrix.getSparseMatrix();
    CSparseMatrix* newMatrix = (CSparseMatrix*) clone();
    vector<TElem> tmp;

    for (auto it = copiedMatrix->m_matrix.begin(); it != copiedMatrix->m_matrix.end(); it++) {
        auto pos = searchElement(newMatrix->m_matrix, it->m_rowIndex, it->m_columnIndex);

        if (pos != newMatrix->m_matrix.end() && pos->m_rowIndex == it->m_rowIndex && pos->m_columnIndex == it->m_columnIndex) {
            vector<TElem>::iterator posNonConst = newMatrix->m_matrix.begin() + (pos - newMatrix->m_matrix.begin());
            posNonConst->m_value -= it->m_value;
        } else tmp.push_back(TElem(it->m_rowIndex, it->m_columnIndex, -it->m_value));
    }

    newMatrix->m_matrix.insert(newMatrix->m_matrix.end(), tmp.begin(), tmp.end());
    sort(newMatrix->m_matrix.begin(), newMatrix->m_matrix.end());

    delete copiedMatrix;
    return newMatrix;
}

CMatrix* CSparseMatrix::multiply(const CMatrix& matrix) const {
    if (m_columns != matrix.getRows())
        throw CExceptionInOperation("multiply");

    /* I will just convert this matrix and "matrix" aswell to CDenseMatrix, there I will count result and I will convert it back to SCparseMatrix */
    CDenseMatrix* matrix1 = (CDenseMatrix*) getDenseMatrix();
    CDenseMatrix* result = (CDenseMatrix*) matrix1->multiply(matrix); // That I already can do.
    CSparseMatrix* convertedResult = (CSparseMatrix*) result->getSparseMatrix();

    delete matrix1;
    delete result;
    return convertedResult;
}

CMatrix* CSparseMatrix::transpose() const {
    CSparseMatrix* newMatrix = new CSparseMatrix("RES", m_columns, m_rows);
    for (auto it = m_matrix.begin(); it != m_matrix.end(); it++)
        newMatrix->m_matrix.push_back(TElem(it->m_columnIndex, it->m_rowIndex, it->m_value));
    sort(newMatrix->m_matrix.begin(), newMatrix->m_matrix.end());

    return newMatrix;
}

CMatrix* CSparseMatrix::inversion() const {
    if (m_rows != m_columns || equalsZero(determinant()))
        throw CExceptionInOperation("inversion");

    CDenseMatrix* tmpMatrix = (CDenseMatrix*) getDenseMatrix();
    try {
        CDenseMatrix* result = (CDenseMatrix*) tmpMatrix->inversion();
        CSparseMatrix* convertedResult = (CSparseMatrix*) result->getSparseMatrix();

        delete tmpMatrix;
        delete result;
        return convertedResult;
    } catch (CExceptionInOperation e) {
        delete tmpMatrix;
        throw;
    }
}

double CSparseMatrix::determinant() const {
    if (m_rows != m_columns)
        throw CExceptionInOperation("determinant");

    CDenseMatrix* tmpMatrix = (CDenseMatrix*) getDenseMatrix();
    double determinant = tmpMatrix->determinant();

    delete tmpMatrix;
    return determinant;
}

int CSparseMatrix::rank() const {
    CDenseMatrix* tmpMatrix = (CDenseMatrix*) getDenseMatrix();
    int rank = tmpMatrix->rank();

    delete tmpMatrix;
    return rank;
}

CMatrix* CSparseMatrix::gem() const {
    CDenseMatrix* tmpMatrix = (CDenseMatrix*) getDenseMatrix();
    CDenseMatrix* result = (CDenseMatrix*) tmpMatrix->gem();
    CSparseMatrix* convertedResult = (CSparseMatrix*) result->getSparseMatrix();

    delete tmpMatrix;
    delete result;
    return convertedResult;
}

CMatrix* CSparseMatrix::commentedGem() const {
    CDenseMatrix* tmpMatrix = (CDenseMatrix*) getDenseMatrix();
    CDenseMatrix* result = (CDenseMatrix*) tmpMatrix->commentedGem();
    CSparseMatrix* convertedResult = (CSparseMatrix*) result->getSparseMatrix();

    delete tmpMatrix;
    delete result;
    return convertedResult;
}

CMatrix* CSparseMatrix::getSubmatrix(int skipRow, int skipColumn) const {
    if (skipRow >= m_rows || skipColumn >= m_columns)
        throw CExceptionInOperation("getSubmatrix");

    CSparseMatrix* tmpMatrix = (CSparseMatrix*) clone();

    for (auto it = tmpMatrix->m_matrix.begin(); it != tmpMatrix->m_matrix.end();)
        if (it->m_rowIndex == skipRow || it->m_columnIndex == skipColumn)
            it = tmpMatrix->m_matrix.erase(it); // now it points to the next element after deleted one
        else it++;

    for (auto it = tmpMatrix->m_matrix.begin(); it != tmpMatrix->m_matrix.end(); it++) {
        if (it->m_rowIndex > skipRow)
            it->m_rowIndex--;
        if (it->m_columnIndex > skipColumn)
            it->m_columnIndex--;
    }
    CSparseMatrix* result = new CSparseMatrix("RES", m_rows - 1, m_columns - 1);
    result->m_matrix = tmpMatrix->m_matrix;

    delete tmpMatrix;
    return result;
}
