
#include <string>

#include "TokenScanner.h"

//----------------------------------------------------------------------------
// constructor
//
TokenScanner::TokenScanner(std::string s)
    : m_iCurPos(0), 
      m_sInput(s), 
      m_cCurChar('\0'), 
      m_curTok({kind_t::none}) {
    // get the firstcharacter
    m_cCurChar = getNextChar();
};


//----------------------------------------------------------------------------
// current
//
Token &TokenScanner::current() {
    return m_curTok;
}


//----------------------------------------------------------------------------
// getNextChar
//
char TokenScanner::getNextChar() {
    m_cCurChar = '\0';
    if (m_iCurPos < m_sInput.length()) {
        m_cCurChar = m_sInput.at(m_iCurPos++);
        while (isspace(m_cCurChar) && (m_iCurPos < m_sInput.length())) {
            m_cCurChar = m_sInput.at(m_iCurPos++);
        }
    } else {
        m_cCurChar = '\0';
    }

    
    //    printf("--- have [%c](%zd)\n", m_cCurChar, m_iCurPos);
    return m_cCurChar;
}


//----------------------------------------------------------------------------
// getNextToken
//
Token TokenScanner::getNextToken() {
    m_curTok = {kind_t::none};

    switch(m_cCurChar) {
    case 0:
        getNextChar();
        return m_curTok = {kind_t::end};

    case ';':
    case '*':
    case '/':
    case '+':
    case '-':
    case '(':
    case ')':
    case '=':
    case ',':
        m_curTok =  {static_cast<kind_t>(m_cCurChar)};//tt
        getNextChar();
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case '.':
        m_curTok.numberValue = "";
        //while (isdigit(m_cCurChar) || (m_cCurChar == '.')) {
        while (isalnum(m_cCurChar) || (m_cCurChar == '.')) {
            m_curTok.numberValue += m_cCurChar;
            getNextChar();
        }
        m_curTok.kind = kind_t::number;
        break;
        
    default:
        //if (isalpha(m_cCurChar)) {
        if (m_cCurChar == '$') {
            while ((m_cCurChar == '$') || isalnum(m_cCurChar) || (m_cCurChar == '_')) {
                m_curTok.stringValue += m_cCurChar;
                getNextChar();
            }
            m_curTok.kind = kind_t::name;
        } else if (m_cCurChar == '_') {
            while ((m_cCurChar == '_') || isalnum(m_cCurChar)) {
                m_curTok.stringValue += m_cCurChar;
                getNextChar();
            }
            m_curTok.kind = kind_t::func;
        } else {
            m_curTok.kind = kind_t::error;
            m_curTok.stringValue = "Bad character: [";
            m_curTok.stringValue += m_cCurChar;
            m_curTok.stringValue += "]";
        }
    }
    //    printf("tok:[%s] str:[%s] num:[%s]\n", msNames[m_curTok.kind].c_str(), m_curTok.stringValue.c_str(), m_curTok.numberValue.c_str());
    return m_curTok;
}
