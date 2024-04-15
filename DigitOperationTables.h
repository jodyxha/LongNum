#ifndef __DIGITOPERATORTABLES_H__
#define __DIGITOPERATORTABLES_H__

#include <string>
#include <map>
#include <vector>

typedef unsigned char uchar;
typedef unsigned int  uint;

// data structure for elementary digit operation results
typedef struct {
    char carry;
    char value;
} cresult;

// look-up table: op_table[c1][c2] = (carry, (c1 op c2)% base)
typedef std::map<char, cresult>   table_row;
// look-up table: first operand => table_row
typedef std::map<char, table_row> table;


// lookup for elementary square roots 
typedef std::map<std::string, char>   sqrt_table;

// look-up row for base changes
typedef std::map<char, std::string>   conv_row;
// look-up table for base changes
typedef std::map<char, conv_row>      conv_table;



class DigitOperationTables {
public:

    static DigitOperationTables *createInstance(uchar iBase);
    virtual ~DigitOperationTables();
    bool isDigit(const char c) const;

    uchar getBase() {return m_iBase;};

    static char  getDigitSym(uchar c);
    static uchar  getDigitVal(char c);
    // addition table
    const cresult add(const char c1, const char c2) { return m_addition[c1][c2];};
    // subtraction table
    const cresult sub(const char c1, const char c2) { return m_subtraction[c1][c2];};
    // multiplication table
    const cresult mul(const char c1, const char c2) { return m_multiplication[c1][c2];};
    // division table
    const cresult div(const char c1, const char c2) { return m_division[c1][c2];};

    char    sqrt(const std::string s) { return m_sqrt[s];};

    std::string conv(const char num, const char base) { return m_conv[base][num];};
    void showTable(table t);
    void showSqrtTable(sqrt_table t);
    void showConvTable(conv_table t);

protected:
    DigitOperationTables();

    int init(uchar iBase);

    uchar m_iBase;
    std::vector<char> m_vDigits;
    table m_addition;
    table m_subtraction;
    table m_multiplication;
    table m_division;
    sqrt_table m_sqrt;
    conv_table m_conv;
};



#endif
