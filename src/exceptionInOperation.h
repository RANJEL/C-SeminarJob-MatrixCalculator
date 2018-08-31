/// Header for class CExceptionInOperation
/** \file exceptionInOperation.h
 *  \author Jan Lejnar <lejnajan@fit.cvut.cz> */

#ifndef EXCEPTIONINOPERATION_H
#define	EXCEPTIONINOPERATION_H

#include <iostream>
using namespace std;

/// Error class
//* An error class, which creates error object when there aren't conditions for continuing in the calculation. */

class CExceptionInOperation {
public:
    CExceptionInOperation(const string& where); ///< Constructor \param where Name of method(operation), where error happened.
    friend ostream& operator<<(ostream& os, const CExceptionInOperation& exception); ///< Write specific information to user, why error occured.

private:
    string m_operation; ///< Name of method(operatoin), where error happened
};

#endif	/* EXCEPTIONINOPERATION_H */

