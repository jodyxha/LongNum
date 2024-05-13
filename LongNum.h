#ifndef __LONGNUM_H__
#define __LONGNUM_H__

#include <string>
#include <vector>

#include "DigitOperationTables.h"

typedef std::vector<std::string> stringvec;
class BaseManager;

class LongNum {

public:
    
    LongNum(){};
    LongNum(uchar iBase);
    LongNum(const LongNum &rLongNum, uchar iBase);
    LongNum(const std::string sHumanReadable, uchar iBase);
    LongNum(const std::string sDigits, uint iComma, uchar iBase, char iSign);
    LongNum(char cDigit, uchar iBase);
    
    

    void shift(uint iNum);
    void unshift(uint iNum);
    static void zeropadBack(std::string &sDigits, uint iNum);
    static void zeropadFront(std::string &sDigits, uint iNum);
    static uint makeCompatibleStrings( LongNum &lN1,  LongNum &lN2, std::string &s1, std::string &s2);
    void reset();

    static LongNum addPositives(LongNum lN1, LongNum lN2);
    LongNum add(LongNum lN);

    static LongNum subPositives(LongNum lN1, LongNum lN2);
    LongNum sub(LongNum lN);

    static LongNum mulPositives(LongNum lN1, LongNum lN2);
    LongNum mul(LongNum &lN);

    static LongNum divPositives(LongNum lN1, LongNum lN2, uint iPrecision);
    LongNum div(LongNum &lN, uint iPrecision);

    static LongNum sqrt(LongNum lN, uint iPrecision);
    static LongNum pow(LongNum lN, LongNum lNPow);
    static LongNum round(LongNum lN, uint iPrecision);

    std::string toString();
    std::string toDebug();
    const std::string &getDigits() const { return m_sDigits;};
    uint getPostDigits() const {return m_iPostDigits;};
    void setPostDigits(uint iPostDigits) {m_iPostDigits=iPostDigits;};
    bool isNegative() { return m_iSign < 0;}
    uchar getBase() {return m_iBase;};
    DigitOperationTables *getOperationTables() {return m_pDOT;};

    void negate(){m_iSign = -m_iSign;};
    LongNum operator-() {return LongNum(m_sDigits, m_iPostDigits, m_pDOT->getBase(), -m_iSign);};
    bool operator==(LongNum lN);
    bool operator<(LongNum lN);
    bool operator<=(LongNum lN);
    bool operator>(LongNum lN);
    bool operator>=(LongNum lN);

    LongNum operator+(LongNum lN) {return add(lN);};
    LongNum operator-(LongNum lN) {return sub(lN);};
    LongNum operator*(LongNum lN) {return mul(lN);};
    
     LongNum &normalize();

    LongNum appendDigits(std::string sNewDigits) {m_sDigits = sNewDigits+m_sDigits; return *this;};

    static LongNum changeBase(LongNum N, uchar toBase, uint iPrecision);

    const std::string &getDigits() {return m_sDigits;};
    int getSign() {return m_iSign;};

    static LongNum truncate(LongNum N);
protected:
    bool lessThan(LongNum lN1, LongNum lN2);
    void setConsts();

    static LongNum mulSingle(LongNum lN, char c);

    static std::string getFirstDigits(std::string &sDigits, LongNum &lN2, uint *piPostDigits);
    static std::string getNextDigit(std::string &sDigits, LongNum lNRest, bool *pbDotSet);
    static uchar simpleDiv(LongNum lN1, LongNum lN2, LongNum &lNRest);

    static std::string getFirstPair(std::string &sDigits, uint iPostDigits);
    static std::string getNextPair(std::string &sDigits, bool *pbAfterDot);
    
    static stringvec splitToPairs(std::string &sDigits, uint iPostDigits);

    static LongNum findLargestSubtractor(LongNum NRem, std::string &sRes, uint iPrecision);

    std::string            m_sDigits;      // the digits of the number
    char                   m_iSign;        // the sign of the numbre (-1/+1)
    uint                   m_iPostDigits;  // number of digits after decimal point

    uchar                  m_iBase;
    DigitOperationTables  *m_pDOT;

    static LongNum one;
    static LongNum two;
};

#endif
