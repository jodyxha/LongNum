#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include <string>
#include <map>
#include "TokenScanner.h"
#include "LongNum.h"


typedef std::map<std::string, LongNum> varmap;

class Evaluator {
public:
    Evaluator(uchar iBase, uint iPrec=10);

    virtual ~Evaluator();
    
    LongNum processString(std::string sInput);

    std::string listVars();

    void setPrec(uint iPrec) { m_iPrec=iPrec;};
    uint getPrec() { return m_iPrec;}
    void setBase(uchar iBase);
    uchar getBase() { return m_iBase;}
protected:
    uchar m_iBase;
    uint  m_iPrec;
    TokenScanner *m_pTS;
    LongNum zero;
    LongNum parseExpression();
    LongNum parseTerm();
    LongNum parsePrimary();

    varmap  mVars;
};


#endif
