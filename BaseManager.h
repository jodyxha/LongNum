#ifndef __BASEMANAGER_H__
#define __BASEMANAGER_H__

#include "DigitOperationTables.h"

typedef std::map<uchar, DigitOperationTables *> baselist;
class BaseManager {

public:
    static BaseManager *getInstance();
    static void         freeInstance();

    static DigitOperationTables    *getOperationTables(uchar m_iBase);
protected:

    BaseManager();
    virtual ~BaseManager();

    static BaseManager *s_pBM;
    static baselist s_mBases;
};


#endif
