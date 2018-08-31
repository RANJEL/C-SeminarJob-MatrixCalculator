/// Implementation of textUserInterface.h
/** \file textUserInterface.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "textUserInterface.h"

void CTui::serve() {
    m_dtb.addFromBackupToDatabase(); // Load default backup file when program starts.

    while (true) {
        displayHeaderAndDatabase();
        m_action = getAction();
        switch (m_action) {
            case 0:
                action0();
                break;
            case 1:
                action1();
                break;
            case 2:
                system("clear");
                displayHeaderAndDatabase();
                m_dtb.displayMatrix(selectMatrix());
                pressKeyToContinue();
                break;
            case 3:
                action3();
                break;
            case 4:
                m_dtb.removeMatrix(selectMatrix());
                break;
            case 5:
                system("clear");
                displayHeaderAndDatabase();
                m_dtb.displayDatabase();
                pressKeyToContinue();
                break;
            case 6:
                if (m_dtb.addFromBackupToDatabase(enterFile()) == false) {
                    cout << endl << "Error: The backup file not found!" << endl;
                    pressKeyToContinue();
                }
                break;
            case 7:
                m_dtb.backUpDatabase(enterFile());
                break;
            case 8:
                cout << endl << "MATRIX CALCULATOR v.1.0 by Jan Lejnar" << endl;
                system("sleep 2");
                return;
        }
    }
}

void CTui::displayHeaderAndDatabase() const {
    cout << ">>> MATRIX CALCULATOR <<<" << endl;
    m_dtb.displayNamesOfMatrices();
    return;
}

void CTui::displayListOfAction() const {
    cout << "Options: " << endl;
    cout << "0) choose a matrix to work with" << endl;
    cout << "1) create a new matrix" << endl;
    cout << "2) display a matrix" << endl;
    cout << "3) rename a matrix" << endl;
    cout << "4) delete a matrix" << endl;
    cout << "5) display all matrices" << endl;
    cout << "6) load matrices from a file" << endl;
    cout << "7) save matrices to a file" << endl;
    cout << "8) exit the program" << endl << endl;

    return;
}

void CTui::displayListOfOperations(const string & name) const {
    CMatrix* selectedMatrix = m_dtb.chooseMatrix(name);
    if (!selectedMatrix)
        return;
    cout << "Options with the selected matrix \"" << selectedMatrix->getName() << "\" (" << selectedMatrix->getRows() << " x " << selectedMatrix->getColumns() << "): " << endl;
    cout << "0) back to previous options" << endl;
    cout << "1) add the selected matrix to" << endl;
    cout << "2) subtract the selected matrix from" << endl;
    cout << "3) multiply the selected matrix by" << endl;
    cout << "4) transpose the selected matrix" << endl;
    cout << "5) calculate the inversion of the selected matrix" << endl;
    cout << "6) calculate the determinant of the selected matrix" << endl;
    cout << "7) calculate the rank of the selected matrix" << endl;
    cout << "8) apply the Gaussian elimination method to the selected matrix" << endl;
    cout << "9) apply the Gaussian elimination method to the selected matrix (commented mod)" << endl << endl;

    return;
}

int CTui::getAction() {
    int selection;
    string line;
    string errorMessage = "";


    do {
        system("clear");
        displayHeaderAndDatabase();
        displayListOfAction();
        cout << errorMessage;
        cout << "Enter your selection (number): ";
        getline(cin, line);
        if (cin.eof())
            cin.clear();
        istringstream iss(line);
        iss >> selection;
        if (!iss)
            selection = -1;
        /* validations*/
        if ((selection == 0 || selection == 2 || selection == 3 || selection == 4) && m_dtb.isEmpty()) {
            selection = -1;
            errorMessage = "Error, there is no matrix to choose, the matrix database is empty! Try again.\n";
        }
    } while ((selection < 0 || selection > 8));

    return selection;
}

int CTui::getOperation(const string& matrixName) {
    int selection;
    string line;

    do {
        system("clear");
        displayHeaderAndDatabase();
        displayListOfOperations(matrixName);
        cout << "Enter your selection (number): ";
        getline(cin, line);
        if (cin.eof())
            cin.clear();
        istringstream iss(line);
        iss >> selection;
        if (!iss)
            selection = -1;
    } while ((selection < 0 || selection > 9));

    return selection;
}

string CTui::selectMatrix() const {
    string name = "";
    string errorMessage = "";

    do {
        system("clear");
        displayHeaderAndDatabase();
        cout << errorMessage;
        cout << "Select a matrix from the database: ";
        cin.clear();
        getline(cin, name);
        if (name != "" && m_dtb.chooseMatrix(name) == NULL) {
            name = "";
            errorMessage = "Error: The matrix with that name doesn't exist! Try again.\n";
        }
    } while (name == "");

    return name;
}

string CTui::enterMatrixName() const {
    string name = "";
    string errorMessage = "";

    do {
        system("clear");
        displayHeaderAndDatabase();
        cout << errorMessage;
        cout << "Enter a name of the new matrix: ";
        cin.clear();
        getline(cin, name);
        if (name != "" && m_dtb.containMatrix(name)) {
            name = "";
            errorMessage = "Error: The matrix with that name is already taken!\n";
        }
    } while (name == "");

    return name;
}

string CTui::enterFile() const {
    string line;
    char character;

    do {
        system("clear");
        displayHeaderAndDatabase();
        cout << "Do you want to use the default backup file \"./backup/backupFile.txt\"? (Y/N): ";
        cin.clear();
        getline(cin, line);
        istringstream iss(line);
        iss >> character;
        if (!iss)
            character = 'C';
    } while (character != 'Y' && character != 'y' && character != 'N' && character != 'n');

    if (character == 'Y' || character == 'y')
        return "./backup/backupFile.txt";

    string fileName = "";
    do {
        system("clear");
        displayHeaderAndDatabase();
        cout << "Enter a relative path to a file: ";
        cin.clear();
        getline(cin, fileName);
    } while (fileName == "");

    return fileName;
}

void CTui::pressKeyToContinue() const {
    string line;
    char character;

    do {
        cout << endl << "Do you want to continue? (Y/N): ";
        cin.clear();
        getline(cin, line);
        istringstream iss(line);
        iss >> character;
        if (!iss)
            character = 'C';
    } while (character != 'Y' && character != 'y');

    return;
}

void CTui::action0() {
    string selectedMatrix = selectMatrix();
    m_operation = getOperation(selectedMatrix);

    switch (m_operation) {
        case 0:
            return;
        case 1:
            operation1(selectedMatrix);
            break;
        case 2:
            operation2(selectedMatrix);
            break;
        case 3:
            operation3(selectedMatrix);
            break;
        case 4:
            operation4(selectedMatrix);
            break;
        case 5:
            operation5(selectedMatrix);
            break;
        case 6:
            operation6(selectedMatrix);
            break;
        case 7:
            operation7(selectedMatrix);
            break;
        case 8:
            operation8(selectedMatrix);
            break;
        case 9:
            operation9(selectedMatrix);
            break;
    }
}

void CTui::action1() {
    int rows;
    int columns;
    bool isDense;
    string name;

    string line;
    char character;
    string errorMessage = "";

    name = enterMatrixName();

    do {
        system("clear");
        displayHeaderAndDatabase();
        cout << "Enter a name of the new matrix: " << name << endl;
        cout << errorMessage;
        cout << "Enter a number of rows of the new matrix: ";
        cin.clear();
        getline(cin, line);
        istringstream iss(line);
        iss >> rows;
        if (!iss) {
            rows = -1;
            errorMessage = "Error: The number of rows must be positive and greater than 0! Try again.\n";
        }
    } while (rows < 1);

    errorMessage = "";
    do {
        system("clear");
        displayHeaderAndDatabase();
        cout << "Enter a name of the new matrix: " << name << endl;
        cout << "Enter a number of rows of the new matrix: " << rows << endl;
        cout << errorMessage;
        cout << "Enter a number of columns of the new matrix: ";
        cin.clear();
        getline(cin, line);
        istringstream iss(line);
        iss >> columns;
        if (!iss) {
            columns = -1;
            errorMessage = "Error: The number of columns must be positive and greater than 0! Try again.\n";
        }
    } while (columns < 1);

    errorMessage = "";
    do {
        system("clear");
        displayHeaderAndDatabase();
        cout << "Enter a name of the new matrix: " << name << endl;
        cout << "Enter a number of rows of the new matrix: " << rows << endl;
        cout << "Enter a number of columns of the new matrix: " << columns << endl;
        cout << "Enter if you want a dense(D) or a sparce(S) matrix (D/S): ";
        cin.clear();
        getline(cin, line);
        istringstream iss(line);
        iss >> character;
        if (!iss)
            character = 'C';
    } while (character != 'D' && character != 'd' && character != 'S' && character != 's');

    if (character == 'D' || character == 'd')
        isDense = true;
    else isDense = false;

    if (m_dtb.createMatrix(isDense, name, rows, columns) == false) {
        cout << endl << "Error: The matrix is too large!" << endl;
        system("sleep 3");
    }

    return;
}

void CTui::action3() {
    string oldName = selectMatrix();
    string newName = enterMatrixName();

    m_dtb.renameMatrix(oldName, newName);
    return;
}

void CTui::operation1(const string& matrix1Name) {
    system("clear");
    displayHeaderAndDatabase();
    cout << "Which matrix add to " << matrix1Name << "? " << endl;
    system("sleep 2");
    string matrix2Name = selectMatrix();

    cout << endl << "The result will be a new matrix." << endl;
    system("sleep 2");
    string matrixRESName = enterMatrixName();

    CMatrix* matrix1 = m_dtb.chooseMatrix(matrix1Name);
    CMatrix* matrix2 = m_dtb.chooseMatrix(matrix2Name);
    CMatrix* matrixRES;

    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrix1Name);
    cout << "+" << endl;
    m_dtb.displayMatrix(matrix2Name);

    try {
        matrixRES = matrix1->add(*matrix2);
        m_dtb.addMatrix(matrixRES, matrixRESName);
        cout << "=" << endl;
        m_dtb.displayMatrix(matrixRESName);
        pressKeyToContinue();
        return;
    } catch (CExceptionInOperation e) {
        cout << e;
        pressKeyToContinue();
        return;
    }
}

void CTui::operation2(const string& matrix1Name) {
    system("clear");
    displayHeaderAndDatabase();
    cout << "Which matrix subtract from " << matrix1Name << "? " << endl;
    system("sleep 2");
    string matrix2Name = selectMatrix();

    cout << endl << "The result will be a new matrix." << endl;
    system("sleep 2");
    string matrixRESName = enterMatrixName();

    CMatrix* matrix1 = m_dtb.chooseMatrix(matrix1Name);
    CMatrix* matrix2 = m_dtb.chooseMatrix(matrix2Name);
    CMatrix* matrixRES;

    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrix1Name);
    cout << "-" << endl;
    m_dtb.displayMatrix(matrix2Name);

    try {
        matrixRES = matrix1->subtract(*matrix2);
        m_dtb.addMatrix(matrixRES, matrixRESName);
        cout << "=" << endl;
        m_dtb.displayMatrix(matrixRESName);
        pressKeyToContinue();
        return;
    } catch (CExceptionInOperation e) {

        cout << e;
        pressKeyToContinue();
        return;
    }
}

void CTui::operation3(const string& matrix1Name) {
    system("clear");
    displayHeaderAndDatabase();
    cout << matrix1Name << " multiply by which matrix?" << endl;
    system("sleep 2");
    string matrix2Name = selectMatrix();

    cout << endl << "The result will be a new matrix." << endl;
    system("sleep 2");
    string matrixRESName = enterMatrixName();

    CMatrix* matrix1 = m_dtb.chooseMatrix(matrix1Name);
    CMatrix* matrix2 = m_dtb.chooseMatrix(matrix2Name);
    CMatrix* matrixRES;

    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrix1Name);
    cout << "*" << endl;
    m_dtb.displayMatrix(matrix2Name);

    try {
        matrixRES = matrix1->multiply(*matrix2);
        m_dtb.addMatrix(matrixRES, matrixRESName);
        cout << "=" << endl;
        m_dtb.displayMatrix(matrixRESName);
        pressKeyToContinue();
        return;
    } catch (CExceptionInOperation e) {
        cout << e;
        pressKeyToContinue();
        return;
    }
}

void CTui::operation4(const string& matrixName) {
    system("clear");
    displayHeaderAndDatabase();

    cout << endl << "The result will be a new matrix." << endl;
    system("sleep 2");
    string matrixRESName = enterMatrixName();

    CMatrix* matrix = m_dtb.chooseMatrix(matrixName);
    CMatrix* matrixRES;
    matrixRES = matrix->transpose();

    m_dtb.addMatrix(matrixRES, matrixRESName);
    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrixName);
    cout << "=> transposed" << endl;
    m_dtb.displayMatrix(matrixRESName);
    pressKeyToContinue();
    return;
}

void CTui::operation5(const string& matrixName) {
    system("clear");
    displayHeaderAndDatabase();

    cout << endl << "The result will be a new matrix." << endl;
    system("sleep 2");
    string matrixRESName = enterMatrixName();

    CMatrix* matrix = m_dtb.chooseMatrix(matrixName);
    CMatrix* matrixRES;

    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrixName);

    try {
        matrixRES = matrix->inversion();
        m_dtb.addMatrix(matrixRES, matrixRESName);
        cout << "=> the inversion" << endl;
        m_dtb.displayMatrix(matrixRESName);
        pressKeyToContinue();
        return;
    } catch (CExceptionInOperation e) {
        cout << e;
        pressKeyToContinue();
        return;
    }
}

void CTui::operation6(const string& matrixName) {
    system("clear");
    displayHeaderAndDatabase();

    CMatrix* matrix = m_dtb.chooseMatrix(matrixName);
    double result;

    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrixName);

    try {
        result = matrix->determinant();
        cout << fixed << setprecision(3);
        cout << "=> The determinant of matrix " << matrixName << " is " << result << endl;
        pressKeyToContinue();
        return;
    } catch (CExceptionInOperation e) {
        cout << e;
        pressKeyToContinue();
        return;
    }
}

void CTui::operation7(const string& matrixName) {
    system("clear");
    displayHeaderAndDatabase();

    CMatrix* matrix = m_dtb.chooseMatrix(matrixName);
    int result;

    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrixName);

    try {
        result = matrix->rank();
        cout << fixed << setprecision(3);
        cout << "=> The rank of matrix " << matrixName << " is " << result << endl;
        pressKeyToContinue();
        return;
    } catch (CExceptionInOperation e) {
        cout << e;
        pressKeyToContinue();
        return;
    }
}

void CTui::operation8(const string& matrixName) {
    system("clear");
    displayHeaderAndDatabase();

    cout << "The result will be a new matrix." << endl;
    system("sleep 2");
    string matrixRESName = enterMatrixName();

    CMatrix* matrix = m_dtb.chooseMatrix(matrixName);
    CMatrix* matrixRES;

    matrixRES = matrix->gem();
    m_dtb.addMatrix(matrixRES, matrixRESName);

    system("clear");
    displayHeaderAndDatabase();
    m_dtb.displayMatrix(matrixName);
    cout << "~" << endl;
    m_dtb.displayMatrix(matrixRESName);
    pressKeyToContinue();
    return;
}

void CTui::operation9(const string& matrixName) {
    system("clear");
    displayHeaderAndDatabase();

    cout << "The result will be a new matrix." << endl;
    system("sleep 2");
    string matrixRESName = enterMatrixName();

    CMatrix* matrix = m_dtb.chooseMatrix(matrixName);
    CMatrix* matrixRES;

    matrixRES = matrix->commentedGem();
    m_dtb.addMatrix(matrixRES, matrixRESName);

    pressKeyToContinue();
    return;
}




