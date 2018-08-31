/// Header for class CTui
/** \file textUserInterface.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef TEXTUSERINTERFACE_H
#define	TEXTUSERINTERFACE_H

#include "matrixDatabase.h"

#include <climits>

using namespace std;

/// Text user interface
//* This class will create object, that only serves the user */

class CTui {
public:
    /** This method basically consist of two endless loops, which allows the user to use different types of options of the program. */
    void serve(); ///< Single method of CTui, called after CTui's default constructor in main.cpp

private:
    void displayHeaderAndDatabase() const;
    void displayListOfAction() const;
    void displayListOfOperations(const string& matrixName) const;
    string selectMatrix() const;
    string enterMatrixName() const;
    string enterFile() const;
    void pressKeyToContinue() const;
    int getAction();
    int getOperation(const string& matrixName);

    void action0();
    void action1();
    void action3();
    void operation1(const string& matrixName);
    void operation2(const string& matrixName);
    void operation3(const string& matrixName);
    void operation4(const string& matrixName);
    void operation5(const string& matrixName);
    void operation6(const string& matrixName);
    void operation7(const string& matrixName);
    void operation8(const string& matrixName);
    void operation9(const string& matrixName);
    /* I am sorry for duplicities in operations. */

    CMatrixDatabase m_dtb;
    int m_action;
    int m_operation;
};


#endif	/* TEXTUSERINTERFACE_H */

