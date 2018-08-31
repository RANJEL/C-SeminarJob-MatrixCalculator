/// Main class of the project.
/** \file main.cpp
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz>
 *  \mainpage
 *  The matrix calculator is capable of these operations: 
 *  * add matrices
 *  * subtract matrices
 *  * multiply matrices
 *  * transpose matrices
 *  * calculate inversion of matrices
 *  * calculate determinant of matrices
 *  * calculate rank of matrices
 *  * Gaussian elimination method
 *  \image html matrix.gif
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#include "textUserInterface.h"

int main() {
    CTui TUI;
    TUI.serve();
    return 0;
}
