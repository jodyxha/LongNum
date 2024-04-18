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
typedef std::map<char, cresult>   op_table_row;
// look-up table: first operand => op_table_row
typedef std::map<char, op_table_row> op_table;


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
    void showTable(op_table t);
    void showSqrtTable(sqrt_table t);
    void showConvTable(conv_table t);

    void show_table(std::string sTableName);
    void ascii_table(std::string sTableName);

    static bool checkBaseCompatibility(std::string sNumber, uchar iBase);

protected:
    DigitOperationTables();

    int init(uchar iBase);

    uchar m_iBase;
    std::vector<char> m_vDigits;
    op_table m_addition;
    op_table m_subtraction;
    op_table m_multiplication;
    op_table m_division;
    sqrt_table m_sqrt;
    conv_table m_conv;

public:
    // the printable digits limit the bases we can use (we could also add '$','&','&','?' etc, but this would look strange)
    static constexpr const char dig_list[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    // the maximum base we can representnumbers in, is 36 (=len(dig_list))
    static const uint max_base = sizeof(dig_list)/sizeof(char);
};



#endif
