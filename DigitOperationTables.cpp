#include <math.h>
#include <map>
#include <vector>
#include <cstring>
#include "DigitOperationTables.h"

// the possible digits limit the bases we can use
char all_digs[] = "0123456789abcdefghijklmnopqrstuvwxyz";


 
//----------------------------------------------------------------------------
// createInstance
//
DigitOperationTables *DigitOperationTables::createInstance(uchar iBase) {
    DigitOperationTables *pBO = new DigitOperationTables();
    int iResult = pBO->init(iBase);
    if (iResult != 0) {
        delete pBO;
        pBO = NULL;
    }
    
    return pBO;
}



//----------------------------------------------------------------------------
// constructor
//
DigitOperationTables::DigitOperationTables() {
}


//----------------------------------------------------------------------------
// destructor
//
DigitOperationTables::~DigitOperationTables() {
}


//----------------------------------------------------------------------------
// showTable
//   show the table of a particular elemrentary operation
//
void DigitOperationTables::showTable(table t) {
    table::const_iterator itt;
    for (itt = t.begin(); itt != t.end(); ++itt) {
        printf("[%c]: ", itt->first);
        table_row::const_iterator itr;
        for (itr = itt->second.begin(); itr != itt->second.end(); ++itr) {
            printf("  [%c] (%c,%c)", itr->first, itr->second.carry, itr->second.value);
        }
        printf("\n");
    }
}

//----------------------------------------------------------------------------
// showSqrtTable
//  show the table of elementary square roots
//
void DigitOperationTables::showSqrtTable(sqrt_table t) {
    sqrt_table::const_iterator it;
    for (it = t.begin(); it != t.end(); ++it) {
        printf(" %s: %c;", it->first.c_str(), it->second);
    }
    printf("\n");
}

//----------------------------------------------------------------------------
// showConvTable
//  show the table of elementary base change conversions
//
void DigitOperationTables::showConvTable(conv_table t) {
    uint iNumBases = strlen(all_digs);
    for (uchar i = 2; i < iNumBases; i++) {
        printf("%c: ", all_digs[i]);
        for (uchar j = 0; j < iNumBases-1; j++) {
            printf("  %s", m_conv[all_digs[i]][all_digs[j]].c_str());
        }
        printf("\n");
    }

    /*
    conv_table::const_iterator itt;
    for (itt = t.begin(); itt != t.end(); ++itt) {
        conv_row::const_iterator itr;
        printf("%c: ", itt->first);
        for (itr = itt->second.begin(); itr != itt->second.end(); ++itr) {
            printf(" %s", itr->second.c_str());
        }
        printf("\n");
    }
    */
}


//----------------------------------------------------------------------------
// getDigitSym
//  get the digit symbol corresponding to the value c
//
char DigitOperationTables::getDigitSym(uchar c) {
    return all_digs[c];
}

//----------------------------------------------------------------------------
// getDigitVal
//  get the numeric value for the symbol c
//
uchar DigitOperationTables::getDigitVal(char c) {
    uchar uVal = 0;
    while ((uVal < strlen(all_digs)) && (c != all_digs[uVal])) {
        uVal++;
    }
    return uVal;
}


//----------------------------------------------------------------------------
// convert
//  convert the value N to base iB
//
std::string convert(uchar iN, uchar iB) {
    std::string sResult;

    if (iB > 1) {
        if (iN > 0) {
            while (iN > 0) {
                uchar x = iN % iB;
                sResult = sResult + all_digs[x];
                iN /= iB;
            } 
        } else {
            sResult = "0";
        }
    } else {
        throw std::string("convert: base must be >= 2");
    }

    return sResult;
}


//----------------------------------------------------------------------------
// init
//   fill the tables for the elementary digit operations:
//   m_addition[i][j]        = ((i+j)/B,(i+j)%B)  (i.e. carry and sum)
//   m_multiplication[i][j]  = ((i*j)/B,(i*j)%B)  (i.e. carry and product)
//   m_subtraction[i][j]     = (0, i-j) if i >=j, (1, (i-j+B)%B) if (i < j)
//   m_division[i][j]        = (i/j, i%j) if j != 0
//
int DigitOperationTables::init(uchar iBase) {
    int iResult = -1;
    uint iNumBases = strlen(all_digs);
    //printf("num bases: %u\n", iNumBases);
    if ((iBase > 1) && (iBase < iNumBases)) {
        m_iBase = iBase;
        for (uchar i = 0; i < m_iBase; i++) {
             table_row tr_add;
             table_row tr_sub;
             table_row tr_mul;
             table_row tr_div;
             for (uchar j = 0; j < m_iBase; j++) {

                 // elementary additions 
                 tr_add[all_digs[j]] = {all_digs[(i+j)/m_iBase], all_digs[(i+j)%m_iBase]};

                 // elementary multiplications        
                 tr_mul[all_digs[j]] = {all_digs[(i*j)/m_iBase], all_digs[(i*j)%m_iBase]};

                 // elementary subtractions
                 if (j <= i) {
                     tr_sub[all_digs[j]] = {'0', all_digs[(i-j)]};
                 } else{
                     tr_sub[all_digs[j]] = {'1', all_digs[(i+m_iBase-j)%m_iBase]};
                 }

                 // elementary divisions (i think we don't even use them)
                 if (j == 0) {
                     tr_div[all_digs[j]] = {0,0};
                 } else {
                 tr_div[all_digs[j]] = {all_digs[i/j], all_digs[i%j]};
                 }
             }
             // add the rows tto the tables
             m_addition[all_digs[i]]       = tr_add;
             m_subtraction[all_digs[i]]    = tr_sub;
             m_multiplication[all_digs[i]] = tr_mul;
             m_division[all_digs[i]]       = tr_div;
        }

        // integer square roots
        char sq[3];
        sq[2] = '\0';
        for (uchar i = 0; i < m_iBase; i++) {
            sq[1] = all_digs[i];
            for (uchar j = 0; j < m_iBase; j++) {
                sq[0] = all_digs[j];
                float f = i*m_iBase + j;
                uint iS = floor(sqrtf(f));
                if (iS < iNumBases) {
                    m_sqrt[sq] = all_digs[iS];
                } else {
                    throw std::string("error in sqrt (shoulf not happen)");
                }
            }
        }

        // base change conversions
        for (uchar i = 2; i < iNumBases; i++) {
            conv_row rconv;
            rconv.clear();
            for (uchar j = 0; j < iNumBases-1; j++) {
                rconv[all_digs[j]] = convert(j, i);
                
            }
            m_conv[all_digs[i]]       = rconv;
        }


        iResult = 0;
    }
    /*

    printf("additions:\n");
    showTable(m_addition);
    printf("add['2']['6']: %c,%c\n", m_addition['2']['6'].first, m_addition['2']['6'].second);

    printf("subtractions:\n");
    showTable(m_subtraction);
    printf("sub['2']['6']: %c,%c\n", m_subtraction['2']['6'].first, m_subtraction['2']['6'].second);

    printf("sqrts:\n");
    showSqrtTable(m_sqrt);
    
    printf("convs:\n");
    showConvTable(m_conv);
    */
    return iResult;
}


//----------------------------------------------------------------------------
// isDigit
//
bool DigitOperationTables::isDigit(const char c) const {
    bool bFound = false;
    for (uchar i = 0; (!bFound) && (i < m_iBase); i++) {
        if (all_digs[i] == c) {
            bFound = true;
        }
    }
    return bFound;
}
