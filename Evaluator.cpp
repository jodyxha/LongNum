
#include <string>
#include "TokenScanner.h"
#include "LongNum.h"
#include "BaseManager.h"
#include "Evaluator.h"



//----------------------------------------------------------------------------
// constructor
//
Evaluator::Evaluator(uchar iBase, uint iPrec)
    : m_iBase(iBase),
      m_iPrec(iPrec),
      m_pTS(NULL) {
    zero = LongNum("0", m_iBase);
}
      


//----------------------------------------------------------------------------
// destructor
//
Evaluator::~Evaluator() {
    BaseManager::freeInstance();
}

    
//----------------------------------------------------------------------------
// setBase
//
void Evaluator::setBase(uchar iBase) {
    // change representation for all variables
    varmap::iterator it;
    for (it = mVars.begin(); it != mVars.end(); ++it) {
        it->second = LongNum::changeBase(it->second, iBase);
    }
    m_iBase = iBase;
}

//----------------------------------------------------------------------------
// processString
//
LongNum Evaluator::processString(std::string sInput) {
    TokenScanner ts{sInput};
    ts.getNextToken();
    m_pTS = &ts;
    LongNum NRes = parseExpression();
    if  (m_pTS->current().kind != kind_t::end) {
        char s[1024];
        sprintf(s,  "leftover token : [%s] str[%s] num[%s]", msNames[m_pTS->current().kind].c_str(), m_pTS->current().stringValue.c_str(), m_pTS->current().numberValue.c_str());
 
        std::string sErr = s;
        throw(sErr);

    }
    return NRes;
}


//----------------------------------------------------------------------------
// parsePrimary
//
LongNum Evaluator::parsePrimary() {
    //    printf("parsePrimary start: token [%s]\n", msNames[m_pTS->current().kind].c_str());
    LongNum NRes("0", m_iBase);
    switch (m_pTS->current().kind) {

    case kind_t::number: {
        // number: just copy over number value
        std::string sNumber = m_pTS->current().numberValue;
        // function in basemanager
        bool bOK = BaseManager::checkBaseCompatibility(sNumber, m_iBase);
        if (bOK) {
            //        printf("parsePrimary doing number [%s]\n", m_pTS->current().numberValue.c_str());
            NRes = LongNum(sNumber, m_iBase);
            m_pTS->getNextToken();
        } else {
            char s[1024];
            sprintf(s,  "nut a number in base %u: [%s]",  m_iBase, sNumber.c_str());
            std::string sErr = s;
            throw(sErr);
        }}
        break;

    case kind_t::minus:
        // minus prefix operator: negate the next primary expression
 
        //        printf("parsePrimary doing minus\n");
        m_pTS->getNextToken();
        //        printf("  now token is [%s]\n", msNames[m_pTS->current().kind].c_str());
        NRes = -parsePrimary();
        //        m_pTS->getNextToken();
        break;

    case kind_t::lparen:
        // left parenthesis: parse next expression and expect a ")" as next token

        //        printf("parsePrimary doing lparen\n"); 
        m_pTS->getNextToken();
        NRes = parseExpression();
        if (m_pTS->current().kind == kind_t::rparen) {
            m_pTS->getNextToken();
        } else {
            char s[1024];
            sprintf(s,  "expected ')': [%s]", m_pTS->getString().c_str());
            std::string sErr = s;
            throw(sErr);
        }
        break;

    case kind_t::func: {
        // string expression: operator or variable

        std::string sName = m_pTS->current().stringValue;
        if (sName == "_sqrt") {
            // handle sqrt
            m_pTS->getNextToken();
            if (m_pTS->current().kind == kind_t::lparen) {
                m_pTS->getNextToken();
                NRes = LongNum::sqrt(parseExpression(), m_iPrec);
                if  (m_pTS->current().kind == kind_t::rparen) {
                    m_pTS->getNextToken();
                } else {
                     char s[1024];
                     sprintf(s,  "expected ')' for 'sqrt': [%s]", m_pTS->getString().c_str());
                     std::string sErr = s;
                     throw(sErr);
                }
            } else {
                char s[1024];
                sprintf(s,  "expected '(' after 'sqrt': [%s]", m_pTS->getString().c_str());
                std::string sErr = s;
                throw(sErr);
            }
        } else  if (sName == "_round") {
            // handle round
            m_pTS->getNextToken();
            if (m_pTS->current().kind == kind_t::lparen) {
                m_pTS->getNextToken();
                NRes = LongNum::round(parseExpression(), 0 /*m_iPrec*/);
                if  (m_pTS->current().kind == kind_t::rparen) {
                    m_pTS->getNextToken();
                } else {
                    char s[1024];
                    sprintf(s,  "expected ')' for 'round': [%s]", m_pTS->getString().c_str());
                    std::string sErr = s;
                    throw(sErr);
                }
            } else {
                char s[1024];
                sprintf(s,  "expected '(' after 'round': [%s]", m_pTS->getString().c_str());
                std::string sErr = s;
                throw(sErr);
            }
        } else  if (sName == "_trunc") {
            // handle trunc
            m_pTS->getNextToken();
            if (m_pTS->current().kind == kind_t::lparen) {
                m_pTS->getNextToken();
                NRes = LongNum::truncate(parseExpression);
                if  (m_pTS->current().kind == kind_t::rparen) {
                    m_pTS->getNextToken();
                } else {
                    char s[1024];
                    sprintf(s,  "expected ')' for 'trunc': [%s]", m_pTS->getString().c_str());
                    std::string sErr = s;
                    throw(sErr);
                }
            } else {
                char s[1024];
                sprintf(s,  "expected '(' after 'trunc': [%s]", m_pTS->getString().c_str());
                std::string sErr = s;
                throw(sErr);
            }

        }
        break;
    }
    case kind_t::name:{
  
        std::string sName = m_pTS->current().stringValue;
        
        varmap::iterator it = mVars.find(sName);
        
        m_pTS->getNextToken();
        if (m_pTS->current().kind == kind_t::assign) {
            // assignment: expect value
            m_pTS->getNextToken();
            LongNum v =  parseExpression();
            mVars[sName] = v;
            NRes = v;
        } else {
            if (it != mVars.end()) {
                // variable value
                NRes = it->second;
            } else {
                char s[1024];
                sprintf(s,  "unknown variable [%s]",  sName.c_str());
                std::string sErr = s;
                throw(sErr);  
            }
        }
    
        break;
    }
    default:
        char s[1024];
        sprintf(s,  "invalid token [%s] in [%s]",  msNames[m_pTS->current().kind].c_str(), m_pTS->getString().c_str());
        std::string sErr = s;
        throw(sErr);
    }
    //    printf("parsePrimary end: token [%s]\n", msNames[m_pTS->current().kind].c_str());
    return NRes;

}


//----------------------------------------------------------------------------
// parseTerm
//
LongNum Evaluator::parseTerm() {
    //    printf("parseTerm start: token [%s]\n", msNames[m_pTS->current().kind].c_str());
    
    bool bGoOn = true;

    LongNum NRes = parsePrimary();

    while (bGoOn) {
        switch (m_pTS->current().kind) {
        case kind_t::mul:
            //            printf("parseTerm does mul\n");
            m_pTS->getNextToken();
            NRes = NRes * parsePrimary();
            break;
        case kind_t::div: {
            //            printf("parseTerm does mdiv\n");
            m_pTS->getNextToken();
            LongNum NDiv = parsePrimary();
            if (!(NDiv == zero)) {
                NRes = NRes.div(NDiv, m_iPrec);
            } else {
                // error
                char s[1024];
                sprintf(s,  "parseTerm found bad divisor [%s] in [%s]",  NDiv.toString().c_str(), m_pTS->getString().c_str());
                std::string sErr = s;
                throw(sErr);
            }
            break;
        }
        default:
            //            printf("parseTerm does default\n");
            bGoOn = false;
        }
    }
    //    printf("parseTerm end: token [%s]\n", msNames[m_pTS->current().kind].c_str());
    return NRes;
}


//----------------------------------------------------------------------------
// parseExpression
//
LongNum Evaluator::parseExpression() {
    bool bGoOn = true;
    //    printf("parseExpression start: token [%s]\n", msNames[m_pTS->current().kind].c_str());
    
    LongNum NRes = parseTerm();


    while (bGoOn) {
        switch (m_pTS->current().kind) {
        case kind_t::plus:
            //            printf("parseExpression does plus\n");
            m_pTS->getNextToken();
            NRes = NRes + parseTerm();
            break;
        case kind_t::minus:
            //            printf("parseExpression does minus\n");
            m_pTS->getNextToken();
            NRes = NRes - parseTerm();
            break;
        default:
            //printf("parseExpression does default\n");
            bGoOn = false;
        }
    }
    
    //    printf("parseExpression end: token [%s]\n", msNames[m_pTS->current().kind].c_str());
    return NRes;
    
}


//----------------------------------------------------------------------------
// listVars
//
std::string Evaluator::listVars() {
    std::string s = "";
    s= "Base: "+std::to_string(m_iBase)+"\nPrec: "+std::to_string(m_iPrec)+"\n-------------\n";
    varmap::iterator it;
    for (it = mVars.begin(); it != mVars.end(); ++it) {
        s += it->first + " : "+it->second.toString()+"\n";
    }
    return s;
}
