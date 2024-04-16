#include <math.h>
#include <map>
#include <vector>
#include <cstring>
#include "DigitOperationTables.h"

// the possible digits limit the bases we can use
// char dig_list[] = "0123456789abcdefghijklmnopqrstuvwxyz";


 
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
void DigitOperationTables::showTable(op_table t) {
    op_table::const_iterator itt;
    for (itt = t.begin(); itt != t.end(); ++itt) {
        printf("[%c]: ", itt->first);
        op_table_row::const_iterator itr;
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
    uint iNumBases = strlen(dig_list);
    for (uchar i = 2; i < iNumBases; i++) {
        printf("%c: ", dig_list[i]);
        for (uchar j = 0; j < iNumBases-1; j++) {
            printf("  %s", m_conv[dig_list[i]][dig_list[j]].c_str());
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
    return dig_list[c];
}

//----------------------------------------------------------------------------
// getDigitVal
//  get the numeric value for the symbol c
//
//  Here we could do:
//    if (is_digit(c)) {
//        uVal = c - '0';
//    else if (is_lower(c)) {
//        uVal = c - 'a';
//    } 
//  But the version below is more general
//
uchar DigitOperationTables::getDigitVal(char c) {
    uchar uVal = 0;
    while ((uVal < strlen(dig_list)) && (c != dig_list[uVal])) {
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
                sResult = sResult + DigitOperationTables::dig_list[x];
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
    //uint iNumBases = strlen(dig_list);
    //printf("num bases: %u\n", iNumBases);
    if ((iBase > 1) && (iBase < max_base)) {
        m_iBase = iBase;
        for (uchar i = 0; i < m_iBase; i++) {
             op_table_row tr_add;
             op_table_row tr_sub;
             op_table_row tr_mul;
             op_table_row tr_div;
             for (uchar j = 0; j < m_iBase; j++) {

                 // elementary additions 
                 tr_add[dig_list[j]] = {dig_list[(i+j)/m_iBase], dig_list[(i+j)%m_iBase]};

                 // elementary multiplications        
                 tr_mul[dig_list[j]] = {dig_list[(i*j)/m_iBase], dig_list[(i*j)%m_iBase]};

                 // elementary subtractions
                 if (j <= i) {
                     tr_sub[dig_list[j]] = {'0', dig_list[(i-j)]};
                 } else{
                     tr_sub[dig_list[j]] = {'1', dig_list[(i+m_iBase-j)%m_iBase]};
                 }

                 // elementary divisions (i think we don't even use them)
                 if (j == 0) {
                     tr_div[dig_list[j]] = {0,0};
                 } else {
                 tr_div[dig_list[j]] = {dig_list[i/j], dig_list[i%j]};
                 }
             }
             // add the rows tto the tables
             m_addition[dig_list[i]]       = tr_add;
             m_subtraction[dig_list[i]]    = tr_sub;
             m_multiplication[dig_list[i]] = tr_mul;
             m_division[dig_list[i]]       = tr_div;
        }

        // integer square roots
        std::string sq("00");
        for (uchar i = 0; i < m_iBase; i++) {
            sq[1] = dig_list[i];
            for (uchar j = 0; j < m_iBase; j++) {
                sq[0] = dig_list[j];
                float f = i*m_iBase + j;
                uint iS = floor(sqrtf(f));
                if (iS < max_base) {
                    m_sqrt[sq] = dig_list[iS];
                } else {
                    throw std::string("error in sqrt (shouldn't happen)");
                }
            }
        }
        /*
        // integer square roots
        char sq[3];
        sq[2] = '\0';
        for (uchar i = 0; i < m_iBase; i++) {
            sq[1] = dig_list[i];
            for (uchar j = 0; j < m_iBase; j++) {
                sq[0] = dig_list[j];
                float f = i*m_iBase + j;
                uint iS = floor(sqrtf(f));
                if (iS < iNumBases) {
                    m_sqrt[sq] = dig_list[iS];
                } else {
                    throw std::string("error in sqrt (shoulddquare less than a given numbe)");
                }
            }
        }
        */
        // base change conversions
        for (uchar i = 2; i < max_base; i++) {
            conv_row rconv;
            rconv.clear();
            for (uchar j = 0; j < max_base-1; j++) {
                rconv[dig_list[j]] = convert(j, i);
                
            }
            m_conv[dig_list[i]]       = rconv;
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
        if (dig_list[i] == c) {
            bFound = true;
        }
    }
    return bFound;
}


//----------------------------------------------------------------------------
// ascii_table
//  print a nice ascii version of the table
//  sTableName must be 'add', 'sub', 'mul', or 'div'
//
void DigitOperationTables::ascii_table(std::string sTableName) {
    op_table *pTable = NULL;
    std::string sOp = "";
    if (sTableName == "add") {
        pTable = &m_addition;
        sOp = "+";
    } else if (sTableName == "sub"){
        pTable = &m_subtraction;
        sOp = "-";
    } else if (sTableName == "mul"){
        pTable = &m_multiplication;
        sOp = "*";
    } else if (sTableName == "div"){
        pTable = &m_division;
        sOp = "/";
    } else {
        printf("Unknown table name [%s]. Should be 'add', 'sub', 'mul', 'div'\n", sTableName.c_str());
    }

    if (pTable != NULL) {
        std::string s0 = "+-----+";
        std::string s1 = "------------+";


        std::string sSep = s0;
        for (uchar j = 0; j < m_iBase; j++) {
            sSep = sSep +  s1;
        }        

        std::string sH0 = "|  %c  |";
        std::string sH1 = "     '%c'    |";


        std::string sL0 = "| '%c' |";
        std::string sL1 = " ('%c', '%c') |";

        printf("%s\n", sSep.c_str());

        char s[100];
        sprintf(s, sH0.c_str(), sOp[0]);
        std::string sHead = s;

        for (uchar j = 0; j < m_iBase; j++) {
            char s[100];
            sprintf(s, sH1.c_str(), getDigitSym(j));
            sHead += std::string(s);
        }
        printf("%s\n", sHead.c_str());

        printf("%s\n", sSep.c_str());

        for (uchar i = 0; i < m_iBase; i++) {
            char s[100];
            sprintf(s, sL0.c_str(), getDigitSym(i));
            std::string sLine = s;
            for (uchar j = 0; j < m_iBase; j++) {
                cresult cr = (*pTable)[getDigitSym(i)][getDigitSym(j)];
                char cc = (cr.carry != '\0')?cr.carry:'-';
                char cv = (cr.value != '\0')?cr.value:'-';
                sprintf(s, sL1.c_str(), cc, cv);
                sLine +=std::string(s);
            }
            printf("%s\n", sLine.c_str());
            printf("%s\n", sSep.c_str());
            

        }
            
    }

}


//----------------------------------------------------------------------------
// show_table
//  display table.
//  sTableName must be 'add', 'sub', 'mul', or 'div'
//
void DigitOperationTables::show_table(std::string sTableName) {
    op_table *pTable = NULL;
    std::string sOp = "";
    if (sTableName == "add") {
        pTable = &m_addition;
        sOp = "+";
    } else if (sTableName == "sub"){
        pTable = &m_subtraction;
        sOp = "-";
    } else if (sTableName == "mul"){
        pTable = &m_multiplication;
        sOp = "*";
    } else if (sTableName == "div"){
        pTable = &m_division;
        sOp = "/";
    } else {
        printf("Unknown table name [%s]. Should be 'add', 'sub', 'mul', 'div'\n", sTableName.c_str());
    }

    if (pTable != NULL) {
        showTable(*pTable);
    }
}
