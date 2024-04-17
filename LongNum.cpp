
#include <string>

#include "DigitOperationTables.h"
#include "BaseManager.h"
#include "LongNum.h"


//----------------------------------------------------------------------------
// lessThan
//  helper function for operator<()
//
bool LongNum::lessThan(LongNum lN1, LongNum lN2) {

    std::string s1;
    std::string s2;
    makeCompatibleStrings(lN1, lN2, s1, s2);

    //    printf("operator< working with [%s] and [%s], post digits: %u\n", s1.c_str(), s2.c_str(), lN1.getPostDigits());

    bool bLess = true;
    if (s1 == s2) {
        bLess = false;
    } else {
        bool bGoOn = true;
        for (uint i = s1.length();  bGoOn && (i > 0); i--) {
            if (s1.at(i-1) > s2.at(i-1)) {
                bLess = false;
                bGoOn = false;
            } else if  (s1.at(i-1) < s2.at(i-1)) {
                bLess = true;
                bGoOn = false;
            }
        }
    }
    return bLess;
}


//----------------------------------------------------------------------------
// operator==
//
bool LongNum::operator==(LongNum lN) { 
    normalize();
    lN.normalize();
    return (m_sDigits == lN.getDigits()) && (m_iPostDigits == lN.getPostDigits()) && (isNegative() == lN.isNegative());
}


//----------------------------------------------------------------------------
// operator<
//
bool LongNum::operator<(LongNum lN) {
    bool bLessThan = false;
    if (*this == lN) {
        bLessThan = false;
    } else if (this->isNegative() && !lN.isNegative()) {
        bLessThan = true;
    } else if  (!this->isNegative() && lN.isNegative()) {
        bLessThan = false;
    } else if (this->isNegative() && lN.isNegative()) {
        bLessThan = !lessThan(*this, lN);
    } else if (!this->isNegative() && !lN.isNegative()) {
        bLessThan = lessThan(*this, lN);
    }
    return bLessThan;
}

//----------------------------------------------------------------------------
// operator<=
//
bool LongNum::operator<=(LongNum lN) {
    bool bLessThan = operator<(lN);
    return bLessThan | (*this == lN);
}

//----------------------------------------------------------------------------
// operator>
//
bool LongNum::operator>(LongNum lN) {
    return !operator<=(lN);
}

//----------------------------------------------------------------------------
// operator>=
//
bool LongNum::operator>=(LongNum lN) {
    return !operator<(lN);
}

//----------------------------------------------------------------------------
// constructor
//  empty constructor
//
LongNum::LongNum(uchar iBase) 
    : m_sDigits(""),
      m_iSign(1),
      m_iPostDigits(0),
      m_pDOT(BaseManager::getInstance()->getOperationTables(iBase)) {
    
}


//----------------------------------------------------------------------------
// constructor
//   copy constructor
//
LongNum::LongNum(const LongNum &rLongNum, uchar iBase) 
    : m_sDigits(rLongNum.m_sDigits),
      m_iSign(rLongNum.m_iSign),
      m_iPostDigits(rLongNum.m_iPostDigits),
      m_pDOT(BaseManager::getInstance()->getOperationTables(iBase)) {

}
          

//----------------------------------------------------------------------------
// constructor
//   from string
//
LongNum::LongNum(const std::string sHumanReadable, uchar iBase)
    : m_sDigits(""),
      m_iSign(1),
      m_iPostDigits(0),
      m_iBase(iBase),
      m_pDOT(BaseManager::getInstance()->getOperationTables(iBase)) {
    

    bool bAfterDot = true;
    uint iPostDigits = 0;
    std::string sHumanReadable1(sHumanReadable);
    if (sHumanReadable1.substr(0, 2) == "0.") {
        sHumanReadable1 = sHumanReadable1.substr(1);
    } else if (sHumanReadable1.substr(0,3) == "-0.") {
        sHumanReadable1 = "-"+sHumanReadable1.substr(2);
    }
    for (uint i = sHumanReadable1.length(); i > 0; i--) {
        char c = sHumanReadable1.at(i-1);
        if (c == '.') {
            bAfterDot = false;
        } else if (m_pDOT->isDigit(c)) {
            m_sDigits = m_sDigits + c;
            if (bAfterDot) {
                iPostDigits++;
            }
        } else if (c == '-') {
            if (i == 1) {
                m_iSign  = -1;
                
            } else {
                std::string sErr = "ERROR: can't have a '-' in the middle of the number: ["+sHumanReadable+"]";
                throw(sErr);
            }
        } else {
            char s[1024];
            sprintf(s,  "ERROR: '%c' is not a valid digit in base %d: [%s]", c, iBase, sHumanReadable.c_str());
            std::string sErr = s;
                throw(sErr);
        }
    }
    m_iPostDigits = bAfterDot?0:iPostDigits;
    //printf("in:[%s] digs:[%s], postdigit %u, sign %d\n", sHumanReadable.c_str(), m_sDigits.c_str(), m_iPostDigits, m_iSign);
}


//----------------------------------------------------------------------------
// constructor
//  from elements
// 
LongNum::LongNum(const std::string sDigits, uint iPostDigits, uchar iBase, char iSign)
    : m_sDigits(sDigits),
      m_iSign(iSign),
      m_iPostDigits(iPostDigits),
      m_iBase(iBase),
      m_pDOT(BaseManager::getInstance()->getOperationTables(iBase)) {

}


//----------------------------------------------------------------------------
// constructor
//  with single digit
// 
LongNum::LongNum(char cDigit, uchar iBase) 
    : m_sDigits(""),
      m_iSign(1),
      m_iPostDigits(0),
      m_iBase(iBase),
      m_pDOT(BaseManager::getInstance()->getOperationTables(iBase)) {
    m_sDigits = cDigit;
}
   
//----------------------------------------------------------------------------
// zeropadBack
//   add trailing zeros
//
void  LongNum::zeropadBack(std::string &sDigits, uint iNum) {
    sDigits = std::string(iNum, '0')+sDigits;
}


//----------------------------------------------------------------------------
// zeropadFront
//   add leading zeros 
//
void  LongNum::zeropadFront(std::string &sDigits, uint iNum) {
    sDigits += std::string(iNum, '0');
}


//----------------------------------------------------------------------------
// shift
//  if there are digits after the decimal point, change iPostDigits,
//  otherwise, add trailing zeros (multiplication by base)
//
void LongNum::shift(uint iNum) {
    if (m_iPostDigits >= iNum) {
        m_iPostDigits -= iNum;
    } else { 
        zeropadBack(m_sDigits, iNum - m_iPostDigits);
        m_iPostDigits = 0;
    }
}


//----------------------------------------------------------------------------
// unshift
//
void LongNum::unshift(uint iNum) {
    m_iPostDigits += iNum;
    normalize();
}



//----------------------------------------------------------------------------
// normalize
//  remove leading and trailing zeros
//
LongNum &LongNum::normalize() {
    
    uint iPre = 0;
    while ((iPre < m_sDigits.length()) && (m_sDigits.at(iPre) == '0') && (iPre < m_iPostDigits) ) {
        iPre++;
    }
    
    uint iPost = m_sDigits.length()-1;
    while ((iPost > iPre) && (m_sDigits.at(iPost) == '0')) {
        iPost--;
    }

    if (iPost < iPre) {
        m_sDigits    = "0";
        m_iPostDigits = 0;
        m_iSign      = 1;
    } else {
        m_sDigits =  m_sDigits.substr(iPre, iPost - iPre + 1);
        m_iPostDigits -= iPre;
    }

    return *this;
}

//----------------------------------------------------------------------------
// truncate
//
LongNum LongNum::truncate(LongNum N) {
    std::string sDigits = std::string(N.m_sDigits.c_str());

    uint iPostDigits = N.getPostDigits();
    std::string s = "";
    if (iPostDigits >= sDigits.length()) {
        s = "0";
    }

    return LongNum(sDigits.substr(iPostDigits, std::string::npos)+s, 0, N.getBase(), 1);
}


//----------------------------------------------------------------------------
// makeCompatibleStrings
//  use padding to make the strings have equal length and 
//  equal number of post decimal point digits
// 
uint LongNum::makeCompatibleStrings( LongNum &lN1,  LongNum &lN2, std::string &s1, std::string &s2) {
    s1 = lN1.getDigits();
    s2 = lN2.getDigits();

    // ensure same number of digits after the decimal point
    if (lN2.getPostDigits() > lN1.getPostDigits()) {
        zeropadBack(s1, lN2.getPostDigits() - lN1.getPostDigits());
    } else if (lN1.getPostDigits() > lN2.getPostDigits()) { 
        zeropadBack(s2, lN1.getPostDigits() - lN2.getPostDigits());
    }

    // ensure same number of digits before the decimal point
    if (s1.length() < s2.length()) {
        zeropadFront(s1, s2.length()-s1.length());
    } else if (s2.length() < s1.length()) {
        zeropadFront(s2, s1.length()-s2.length());
    }
    return ( lN1.getPostDigits() > lN2.getPostDigits())?lN1.getPostDigits():lN2.getPostDigits();
}


//----------------------------------------------------------------------------
// addPositives
//  adding non-negative numbers
//
LongNum LongNum::addPositives(LongNum lN1, LongNum lN2)  {
    std::string s1;
    std::string s2;
    uint iNewPostDigits = makeCompatibleStrings(lN1, lN2, s1, s2);
    //    printf("add working with [%s] and [%s], post digits: %u\n", s1.c_str(), s2.c_str(), m_iPostDigits);
  
    std::string sResult = "";
    char cCarry = '0';
    for (uint i = 0; i < s1.length(); i++) {
        char c1 = s1.at(i);
        const cresult cr1 = lN1.getOperationTables()->add(cCarry, c1);
        //        printf("cur1 %c + carry %c: (%c,%c)\n", c1, cCarry, cr1.first, cr1.second);
        const cresult cr = lN1.getOperationTables()->add(cr1.value, s2.at(i));
        //        printf("res + cur2: (%c,%c)\n", cr.first, cr.second);
        sResult += cr.value;
        cCarry =  lN1.getOperationTables()->add(cr.carry,cr1.carry).value;
    }
    if (cCarry != '0') {
        sResult += cCarry;
    }
    //    printf("Result  [%s], post digits: %u\n", sResult.c_str(), iNewPostDigits);
    
    return LongNum(sResult, iNewPostDigits, lN1.getBase(), 1);
}


//----------------------------------------------------------------------------
// add
//  adding positive or negative numbers: reduce to operations with natural numbers
//
LongNum LongNum::add(LongNum lN)  {
    LongNum ns(lN.getBase());
    if (isNegative() && lN.isNegative()) {
        //    x + y = -((-x) + -y))
        ns = -addPositives(-*this, -lN);
    } else if  (!isNegative() && !lN.isNegative()) {
        //    x + y = x + y
        ns = addPositives(*this, lN);
    } else if  (!isNegative() && lN.isNegative()) {
        //    x + y = x - (-y)
        ns = subPositives(*this, -lN);
    } else if  (isNegative() && !lN.isNegative()) {
        //    x + y = y - (-x)
        ns = subPositives(lN, -*this);
    }
    return LongNum(ns);
}


//----------------------------------------------------------------------------
// subPositives
//  subtract non-negative numbers
//
LongNum LongNum::subPositives(LongNum lN1, LongNum lN2) {
    
    std::string s1;
    std::string s2;
    uint iNewPostDigits = makeCompatibleStrings(lN1, lN2, s1, s2);

    bool bSwapped = false;

    // make sure the minuend is larger than the subtrahend and remember the swap)

    if (lN1 < lN2) {
        s1.swap(s2);
        bSwapped = true;
    }
   
    // now element-wise subtraction using the subtraction table
    std::string sResult = "";
    char cCarry = '0';
    for (uint i = 0; i < s1.length(); i++) {
        char c1 = s1.at(i);
        char c2 = s2.at(i);
        //        printf("digit %d: %c <-> %c\n", i, c1, c2);
        const cresult cr1 = lN1.getOperationTables()->add(cCarry, c2);
        //        printf("cur2 %c + carry %c: (%c,%c)\n", c2, cCarry, cr1.first, cr1.second);
        const cresult cr = lN1.getOperationTables()->sub(c1, cr1.value);
        //        printf("c1(%c) - res(%c): (%c,%c)\n", c1, cr1.second, cr.first, cr.second);
        sResult += cr.value;
        cCarry =  lN1.getOperationTables()->add(cr.carry,cr1.carry).value;
        //        printf("added %c to resut: %s, carry %c\n", cr.second, sResult.c_str(), cCarry);
    }
    return LongNum(sResult,iNewPostDigits,lN1.getBase(), bSwapped?-1:1);
}


//----------------------------------------------------------------------------
// sub
//  subtracting positive or negative numbers: reduce to operations with natural numbers
//
LongNum LongNum::sub(LongNum lN)  {
    LongNum ns(lN.getBase());
    
    if (isNegative() && lN.isNegative()) {
        //   x - y = (-y) - (-x)
        ns = subPositives(-lN, -*this);
    } else if  (!isNegative() && !lN.isNegative()) {
        //   x - y- = x -y
        ns = subPositives(*this, lN);
    } else if  (!isNegative() && lN.isNegative()) {
        //   x - y =  x + (-y)
        ns = addPositives(*this, -lN);
    } else if  (isNegative() && !lN.isNegative()) {
        //   x - y = (-x) + y
        ns = -addPositives(-*this, lN);
    }
    
    return LongNum(ns);
}


//----------------------------------------------------------------------------
// mulPositives
//  multiply non-negative numbers
//
LongNum LongNum::mulPositives(LongNum lN1, LongNum lN2) {
    // because lN1 will be shifted, we must do this now
    int iFinalPostDigits =  lN1.getPostDigits() + lN2.getPostDigits(); 

    LongNum lNResult("", lN1.getPostDigits(), lN1.getBase(), lN1.isNegative()?-1:1);
    std::string s2 = lN2.getDigits();
    for (uint i = 0; i < s2.length(); i++) {
        
        LongNum lNTemp = mulSingle(lN1, s2.at(i));
        
        lNResult = lNResult.add(lNTemp);
        lN1.shift(1);
    } 
    return LongNum(lNResult.getDigits(), iFinalPostDigits, lNResult.getBase(), lNResult.isNegative()?-1:1);
}


//----------------------------------------------------------------------------
// mulSingle
//  multiply a number with a singe digit
//
LongNum LongNum::mulSingle(LongNum lN, char c) {

    std::string sResult("");
    std::string s1 = lN.getDigits();
    char cCarry = '0';
    for (uint i = 0; i < s1.length(); i++){
        char c1 = s1.at(i);
        const cresult crp = lN.getOperationTables()->mul(c1, c);
        const cresult crc = lN.getOperationTables()->add(cCarry, crp.value);
        sResult += crc.value;
        cCarry = lN.getOperationTables()->add(crc.carry, crp.carry).value;
    }
    if (cCarry != '0') {
        sResult += cCarry;
    }
    return LongNum(sResult, lN.getPostDigits(), lN.getBase(), lN.isNegative()?-1:1);
}


//----------------------------------------------------------------------------
// mul
//  multiplying positive or negative numbers: reduce to operations with natural numbers
//
LongNum LongNum::mul(LongNum &lN) {
    LongNum ns(lN.getBase());
    
    if (isNegative() && lN.isNegative()) {
        //   x * y = (-x) * (-y)
        ns = mulPositives(-lN, -*this);
    } else if  (!isNegative() && !lN.isNegative()) {
        //   x * y = x * y
        ns = mulPositives(*this, lN);
    } else if  (!isNegative() && lN.isNegative()) {
        //   x * y = -(x * (-y))
        ns = -mulPositives(*this, -lN);
    } else if  (isNegative() && !lN.isNegative()) {
        //   x * y = -((-x) * y)
        ns = -mulPositives(-*this, lN);
    }
    
    return LongNum(ns);
}


//----------------------------------------------------------------------------
// collectLeadingDigits
//   collect leading digits from sDigits into sTest, until value(sTest) is
//   larger than lN2; remove selected digits from s1
//
//
std::string LongNum::collectLeadingDigits(std::string &sDigits, LongNum &lN2, uint *piPostDigits) {
    LongNum lN2b(lN2.getDigits(), 0, lN2.getBase(), 1);
    
    uint iCount = sDigits.length()-1;
    std::string sTest("");
    bool bInString = (!sDigits.empty());
    while(true) {
        if (bInString) {
            sTest = sDigits[iCount] + sTest;
        } else {
            sTest = "0" + sTest; // first 0? if yes: remember position
            (*piPostDigits)++;
        }
        LongNum nTest = LongNum(sTest, 0, lN2.getBase(), 1);
        
        if (nTest >= lN2b) {
            //printf("-> ok; we're to large now, so take previous\n");
            iCount++;
            break;
        }
        if (iCount > 0) {
            iCount--;
        } else {
            bInString = false;
        }
    }
   
    sDigits = (bInString)?sDigits.substr(0, iCount-1):"";
  
    return sTest;
}


//----------------------------------------------------------------------------
// collectNextDigit
//
std::string LongNum::collectNextDigit(std::string &sDigits, LongNum lNRemainder, bool *pbDotSet) {
  
    std::string sRemainder = lNRemainder.normalize().getDigits();
    if (sDigits.length() > 0) {
        sRemainder =  sDigits.back() +  sRemainder;
        sDigits = sDigits.substr(0, sDigits.length()-1);
    } else {
        sRemainder =  "0" + sRemainder; // first 0? if yes: notify remember position
        *pbDotSet |= true;
    }
 
    return sRemainder;
}        


//----------------------------------------------------------------------------
// simpleDiv
//   subtract lN2 from lN1 as often as possible. Return number of
//   subtractions (and the remainder in lNRest)
//
uchar LongNum::simpleDiv(LongNum lN1, LongNum lN2, LongNum &lNRest) {
    uchar iC = 0;
 
    while ((lN2 <= lN1)) {
        lN1 = lN1 - lN2;
        iC++;
 
    }
    lNRest = lN1;
    return iC;

}


//----------------------------------------------------------------------------
// divPositives
//
LongNum LongNum::divPositives(LongNum lN1, LongNum lN2, uint iPrecision) {
    LongNum NResult(lN1.getBase());
    

    if (lN2 == LongNum("0", lN2.getBase())) {
        std::string sErr = "ERROR: 0-division: ["+lN1.toString()+"] / ["+lN2.toString()+"]";
        throw(sErr);
    } else {
        if (lN1 == LongNum("0", lN1.getBase())) {
            // dividing zero always yields zero
            NResult = LongNum("0",lN1.getBase());
        } else {
            // we shift both numbers by the same amount such that there are no digits after the decimal point in bith numbers
            uint c1 = lN1.getPostDigits();
            uint c2 = lN2.getPostDigits();
            uint iS = (c1 > c2)?c1:c2;
            
            lN1.shift(iS);
            lN2.shift(iS);
         
            std::string sResult = "";
            uint iDigCount = 0;
            std::string s1 = lN1.getDigits();
            bool bDotSet = false;
            uint iPostDigits = 0;
            std::string sSelected = collectLeadingDigits(s1, lN2, &iPostDigits);

            // have we already used up all digits of s1?
            if (iPostDigits > 0) {
                iPostDigits--;
                bDotSet = true;
            }
      
            while (iDigCount < iPrecision) {
        
                LongNum lNSelected(sSelected, 0, lN1.getBase(), 1);
                LongNum lNRemainder(lN1.getBase());
                
                // remove lN2 as often as possible from selected: count->result, rest->sSelected 
                uchar u = simpleDiv(lNSelected, lN2, lNRemainder);
                          
                if (bDotSet) {
                    iPostDigits++;
                }
                sResult =  DigitOperationTables::getDigitSym(u) + sResult;
              
                sSelected = collectNextDigit(s1, lNRemainder, &bDotSet);

                iDigCount++;
            }
            if (!bDotSet) {
                if (sSelected.empty()) {
                    iPostDigits = sResult.length();
                }
            }

            while (sResult.length() < iPostDigits) {
                sResult = sResult + "0";
            }
          
            NResult = LongNum(sResult, iPostDigits, lN1.getBase(), lN1.isNegative()?-1:1);
        }
    }
    return NResult;
}    


//----------------------------------------------------------------------------
// div
//
LongNum LongNum::div(LongNum &lN, uint iPrecision) {
    LongNum ns(lN.getBase());
    // exclude 0-divisor
    if (lN == LongNum("0", lN.getBase())) {
        throw(std::string("Errur: division by 0\n"));
    } else {
        if (isNegative() && lN.isNegative()) {
            //   x / y = (-x) / (-y)
            ns = divPositives(-lN, -*this, iPrecision);
        } else if  (!isNegative() && !lN.isNegative()) {
            //   x / y = x / y
            ns = divPositives(*this, lN, iPrecision);
        } else if  (!isNegative() && lN.isNegative()) {
            //   x / y = -(x / (-y))
            ns = -divPositives(*this, -lN, iPrecision);
        } else if  (isNegative() && !lN.isNegative()) {
            //   x / y = -((-x) / y)
            ns = -divPositives(-*this, lN, iPrecision);
        }
    }
    return LongNum(ns);
}


//----------------------------------------------------------------------------
// findHead
//   sqrt-helper
//   find the head of the number: first two digits if length is even,
//   "0"+highest digit if length is odd
//
std::string LongNum::findHead(std::string &sDigits, uint iPostDigits,  bool *pbAfterDot) {
    std::string s("");
    
    if (((sDigits.length()-iPostDigits)%2) == 0) {
        s = sDigits.substr(sDigits.length()-2, 2);
        sDigits = sDigits.substr(0, sDigits.length()-2);
    } else {
        s = sDigits.substr(sDigits.length()-1, 1)+"0";
        sDigits = sDigits.substr(0, sDigits.length()-1);
    
    }
    return s;
}


//----------------------------------------------------------------------------
// getNextTwoDigits
//   sqrt-helper
//   get the next two digit os sDigits, or "00" if empty
//
std::string LongNum::getNextTwoDigits(std::string &sDigits, bool *pbAfterDot) {
    std::string s("");
    // after the decimal point, there may be an odd number of digits!
    if (!sDigits.empty()) {
        if (sDigits.length() > 1) {
            s = sDigits.substr(sDigits.length()-2, 2);
            sDigits = sDigits.substr(0, sDigits.length()-2);
        } else {
            s = "0"+sDigits.substr(sDigits.length()-1, 1);
            sDigits = sDigits.substr(0, sDigits.length()-1);
        }
    } else {
        s = "00";
        (*pbAfterDot) |= true;
    }
    return s;
}

//----------------------------------------------------------------------------
// findLargestSubtractor
//   sqrt-helper
//   we have to find a value x such that x*(20*Res+x) is less or equal to NRem
//
LongNum LongNum::findLargestSubtractor(LongNum NRem, std::string &sResult, uint iPrecision) {
    uchar iBase = NRem.getBase();
    LongNum NSub(iBase);
    LongNum zero("0", iBase);
    LongNum one("1", iBase);
    LongNum N10("10", iBase);
    // we must find largest q with q*(tester+q) <= reminder

    LongNum NRes(sResult, 0,  iBase, 1);
    LongNum NTester(iBase);
    if (iBase == 2) {
        NTester = NRes;
        NTester.shift(1);
    } else {
        NTester = mulSingle(NRes, '2');
    }
    NTester.shift(1);
    // NTester is now equal to 2*base*res

    //printf("NTester [%s]\n", NTester.getDigits().c_str());

    
    LongNum NQ(iBase);
    if (NTester == zero){
        NQ   = zero;
        NSub = zero;
    } else {
        // trial division to check: NRem/NTester is an upper limit for our q, because then q*(NTester+q) = NRem + (NRem/NTester)^2 >= NRem
        NQ = NRem.div(NTester, iPrecision);
        if (NQ >= N10) {
            //printf("NQ is [%s, %d] >= 10, must set it to highest single digit\n", NQ.getDigits().c_str(), NQ.getPostDigits());
            NQ = N10 - one;
        }
        // NQ should now be in [0, N10-1] (i.e. a single digit before the decimal point)

        // NQ < N10 might not be an integer, therefore, we must truncate it
        NQ =  LongNum::truncate(NQ.normalize());
       
        // build the subtractor for successively smaller values of NQ until you get the largest subtractor less than NRem
        NSub = NQ * (NQ + NTester);
        while (NRem < NSub) {
            NQ = NQ - one;
            NSub = NQ * (NQ + NTester);
        }
    }
    sResult = NQ.getDigits().back() + sResult; 

    return NSub; 
}

//----------------------------------------------------------------------------
// sqrt
//  using an algorithm for square roots
//  I1. find leading pair cur of input digits D:
//    I1a  if number of digits before the decimal point is even, use the frontmost 2 digits
//    I1b  if number of digits before the decimal point is odd, use "0"+the frontmost digit
//  I2. find integer square root sub of cur
//  I3. first digit of result = sub
//  L1. remainder cur = cur - sub
//  L2. find next pair P of digits
//    L2a  if there are digits in D. use the frontmost 2 digits
//    L2b  if D is empty, use "00"
//  L3. append digits to remainder: remainder = remainder . P
//  L4. tester = (2*result) << 1
//  L5. find largest q such that q*(tester+q) <= remainder
//  L6. q is the next digit of the result: result = result . q
//  L7. sub =  q*(tester+q)
//  L8. go to L1
//
// remember that in out implementation the numbers are written backwards
//
LongNum LongNum::sqrt(LongNum lN, uint iPrecision) {
    std::string sResult("");
    //printf("sqrt([%s], ipostdigits[%u])\n", lN.getDigits().c_str(), lN.getPostDigits());
    LongNum one("1", lN.getBase());
    LongNum zero("0", lN.getBase());
    bool bAfterDot = false;
    int iPostDigits = 0;
    
    if (lN.isNegative()) {
        std::string sErr = "ERROR: can't take square root of negative number: ["+lN.toString()+"]";
        throw(sErr);

    } else if (lN == zero) {
        sResult = "0";

    } else {
        //printf("--- calculating sqrt(%s) ---\n", lN.toString().c_str());
        LongNum hun("100", lN.getBase());
        int iShifts = 0;
        // if we have numbers > 100, we have to shift by an even number of digits until <100
        while (!(lN < hun)) {
            iShifts--;
            iPostDigits -= 2;
            lN.m_iPostDigits +=2;
        }
        // if we have numbers < 1, we have to shift by an even number of digits until > 1
        while (lN < one) {
            lN.shift(2);
            iShifts++;
            iPostDigits += 2;
        }
        lN.normalize();

        // I1:
        std::string sDigits = lN.getDigits();

        // find first (odd length) or first two digits (even length); 
        // make two-char string (zeropad on the right,  e.g. '7' -> "70")
        std::string s1 = findHead(sDigits, lN.getPostDigits(), &bAfterDot);
       
        if (bAfterDot) {
            iPostDigits++;
        }

        // I2:
        char q = lN.getOperationTables()->sqrt(s1); // q is the first value of the result
        sResult += q; 

        // I3:
        LongNum Nr(q, lN.getBase());
        LongNum NSub = Nr*Nr;

        LongNum NRem = LongNum(s1, 0, lN.getBase(), 1);
  
        uint i = 0;
        while (i < iPrecision) {

            // L1:
            NRem = NRem - NSub;
  
            // L2:
            std::string s2 = getNextTwoDigits(sDigits, &bAfterDot);
            if (bAfterDot) {
                iPostDigits++;
            }
      
            // L3:
            // add next two digits from input (or "00") to remainder
            NRem.appendDigits(s2);
            NRem.normalize();
            
            // L4 - L7
            NSub = findLargestSubtractor(NRem, sResult, iPrecision);
           
            i++;
        }
        
        iPostDigits = sResult.length() -1 +iShifts;
    }

    //   subtract r from lN -> remainder x;
    //   if there are digits, get the next two digits, else get two 0s (notify dot)
    //   tester = mulSingle(result, 2).shift(1)
    //   q = x / tester
    //   if (q*(tester + q) < x)
    //     if (x - q*(tester+q)) < tester+q)) 
    //        r =  q*(tester+q)
    //     else
    //        r = (q-1)*(tester+q-1)
    //   fi
    // done
    return LongNum(sResult, iPostDigits, lN.getBase(), 1);
}



//----------------------------------------------------------------------------
// changeBase
//
LongNum LongNum::changeBase(LongNum N, uchar toBase) {
    LongNum one("1", toBase);
    LongNum zero("0", toBase);
    char tb = N.getOperationTables()->getDigitSym(toBase);
    char fb = N.getOperationTables()->getDigitSym(N.getBase());
  
    //    LongNum NToBase(N.getOperationTables()->conv(tb, tb), toBase);
    LongNum NFromBase(N.getOperationTables()->conv(fb, tb), 0, toBase, 1);
    LongNum lNRes(zero);
    std::string sDigits = N.getDigits();
    for (uint i = sDigits.size(); i > 0; i--) {
  
        lNRes = (lNRes*NFromBase).normalize() + LongNum(N.getOperationTables()->conv(sDigits[i-1], tb), 0, toBase, 1);
 
    }
    
    size_t n = snprintf(NULL, 0, "%d", N.getPostDigits());
    char sBuf[n+1];
    sprintf(sBuf, "%d", N.getPostDigits());
    LongNum NDivisor = LongNum::pow(NFromBase, LongNum(sBuf, 10));
    lNRes = lNRes.div(NDivisor, 20); 
    return LongNum(lNRes.normalize());
}


//----------------------------------------------------------------------------
// pow
//
LongNum LongNum::pow(LongNum lN, LongNum lNPow) {
    LongNum one("1", lN.getBase());
    LongNum zero("0", lN.getBase());
    LongNum lNRes(one);
    while (zero < lNPow) {
        lNRes = lNRes * lN;
        lNPow = lNPow - one;
    }
    return lNRes;
}


//----------------------------------------------------------------------------
// round
//
LongNum LongNum::round(LongNum lN, uint iPrecision) {
    LongNum lNRes(lN);
    std::string sDigits = lN.getDigits();
  
    if (iPrecision < lN.getPostDigits()) {
         
        char c = sDigits.at(lN.getPostDigits()-iPrecision-1);
        printf("char at %u: %c\n", lN.getPostDigits()-iPrecision-1, c);
        uchar v = DigitOperationTables::getDigitVal(c);
        if (lN.getBase()/2 < v) {
            LongNum lNRounder("1", iPrecision, lN.getBase(), 1);
            printf("Orig:    %s\n", lN.toString().c_str());
            printf("Rounder: %s\n", lNRounder.toString().c_str());
            lNRes = lN + lNRounder;
            printf("Res:     %s\n", lNRes.toString().c_str());
            sDigits = lNRes.getDigits();
        }
        // truncate to iPrecision 
        sDigits = sDigits.substr(lN.getPostDigits() - iPrecision);
    } else {
        // add 0s?
        zeropadBack(sDigits, iPrecision -  lN.getPostDigits());
        
    }
    return LongNum(sDigits, iPrecision, lN.getBase(), 1);
}


//----------------------------------------------------------------------------
// toString
//
std::string LongNum::toString() {
    std::string sOut;
    std::string s0;
    //    printf("m_iPostDigits: %u, digits.lem %zd\n", m_iPostDigits, m_sDigits.length());
    if (m_iPostDigits >= m_sDigits.length()) {
        zeropadFront(m_sDigits, m_iPostDigits-m_sDigits.length());
        s0 = "0.";
    }
    for (uint i = 0; i < m_sDigits.length(); i++) {
        if ((i == m_iPostDigits) && (m_iPostDigits > 0)) {
            sOut = "." + sOut;
        } 
        sOut = m_sDigits.at(i) + sOut;
    }
    sOut = s0 + sOut;
    if (m_iSign < 0) {
        sOut = "-" +  sOut;
    }

    return std::string(sOut);
}
            

//----------------------------------------------------------------------------
// toDebug
//
std::string LongNum::toDebug() {
    return std::string("[" + m_sDigits + "]("+std::to_string(m_iPostDigits)+")");
}
