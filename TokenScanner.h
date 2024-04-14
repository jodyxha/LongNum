#ifndef __TOKEN_SCANNER_H__
#define __TOKEN_SCANNER_H__

#include <iostream>
#include <fstream>
#include <map>
#include <string>

#include "LongNum.h"

// the token flavors
enum class kind_t: char {
    none, name, number, end, error,
    plus='+', minus='-', mul='*', div='/', assign='=', lparen='(', rparen=')', comma=','
};


// string representation of the token flavors
static std::map<kind_t, std::string> msNames = {
    {kind_t::none,   "none"},
    {kind_t::name,   "name"},
    {kind_t::number, "number"},
    {kind_t::end,    "end"},
    {kind_t::error,  "error"},
    {kind_t::plus,   "plus"},
    {kind_t::minus,  "minus"},
    {kind_t::mul,    "mul"},
    {kind_t::div,    "div"},
    {kind_t::assign, "assign"},
    {kind_t::lparen, "lparen"},
    {kind_t::rparen, "rparen"},
    {kind_t::comma,  "comma"},
};


// the token
struct Token {
    kind_t kind;
    std::string stringValue;
    std::string numberValue;
};


class TokenScanner {
public:
    TokenScanner(std::string s);

    ~TokenScanner() {};

    Token getNextToken();
    Token &current();

    const std::string &getString() { return m_sInput;};
private:
    char getNextChar();

    size_t m_iCurPos;
    std::string m_sInput;
    char  m_cCurChar;
    Token m_curTok;
};  
#endif
