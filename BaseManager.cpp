#include <map>
#include "DigitOperationTables.h"
#include "BaseManager.h"

BaseManager *BaseManager::s_pBM;
baselist     BaseManager::s_mBases;

//----------------------------------------------------------------------------
// getInstance
//
BaseManager *BaseManager::getInstance() {
    if (s_pBM == NULL) {
        s_pBM = new BaseManager();
    }
    return s_pBM;
}

//----------------------------------------------------------------------------
// freeInstance
//
void BaseManager::freeInstance() {
    if (s_pBM != NULL) {
        delete s_pBM;
    }
}

//----------------------------------------------------------------------------
// constructor
//
BaseManager::BaseManager() {}

//----------------------------------------------------------------------------
// destructor
//
BaseManager::~BaseManager() {
    baselist::iterator it;
    for (it = s_mBases.begin(); it != s_mBases.end(); ++it) {
        if (it->second != NULL) {
            delete it->second;
        }
    }
}
 
//----------------------------------------------------------------------------
// getBase
//
DigitOperationTables *BaseManager::getOperationTables(uchar iBase) {
    DigitOperationTables *pResult = NULL;
    baselist::const_iterator it = s_mBases.find(iBase);
    if (it != s_mBases.end()) {
        pResult = it->second;
    } else {
        pResult = DigitOperationTables::createInstance(iBase);
        if (pResult != NULL) {
            s_mBases[iBase] = pResult;
        }
    }
    return pResult;
}

//----------------------------------------------------------------------------
// checkBaseCompatibility
//
bool BaseManager::checkBaseCompatibility(std::string sNumber, uchar iBase) {
    return DigitOperationTables::checkBaseCompatibility(sNumber, iBase);
}

