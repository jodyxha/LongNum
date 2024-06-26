#include <iostream>
#include <string>
#include "TokenScanner.h"
#include "BaseManager.h"
#include "LongNum.h"
#include "Evaluator.h"

#define BASE 10
#define PREC 12



//----------------------------------------------------------------------------
// trim
//   return left and right trimmed copy of string
// 
std::string trim(const std::string& str) {
    std::string sOut = "";
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (std::string::npos == first) {
        if (std::string::npos == last) {
            sOut = "";
        } else {
            sOut = str.substr(0, last);
        }
    } else {
        if (std::string::npos == last) {
            sOut = str.substr(first);
        } else {
            sOut =  str.substr(first, (last - first + 1));
        }

        size_t last = str.find_last_not_of(' ');
        sOut = str.substr(first, (last - first + 1));
    }
    return sOut;
}


//----------------------------------------------------------------------------
// scan
//    testing the token scanner
//
void scan(TokenScanner &ts) {
    for (;;) {
        ts.getNextToken();
        std::cout << msNames[ts.current().kind];
        if (ts.current().kind == kind_t::number) {
            std::cout << " (" << ts.current().numberValue << ")";
        } else if (ts.current().kind == kind_t::name) {
            std::cout << " ('" << ts.current().stringValue << "')";
        }
        std::cout << "\n";
        if (ts.current().kind == kind_t::error) {
            std::cout <<   "Error: " << ts.current().stringValue << "\n";
            break;
        }
        if (ts.current().kind == kind_t::none) break;
        if (ts.current().kind == kind_t::end) break;
        
    }
    
}

//----------------------------------------------------------------------------
// help
//
void help() {
     std::cout <<"\n----------------------------------------------------------------------------\n";
     std::cout << "interactive calculator for arbitrary-length numbers in arbitrary bases\n";
     std::cout << "supported operations:\n";
     std::cout << "'+'   addition\n";
     std::cout << "'-'   subtraction\n";
     std::cout << "'*'   multiplication\n";
     std::cout << "'/'   division\n";
     std::cout << "_sqrt()  square-root function\n";
     std::cout << "_round() round to next integer\n";
     std::cout << "_trunc() truncate (remove fraction part of number)\n";
     std::cout << "\n";
     std::cout << "Variable names must start with '$':";
     std::cout << "To set variable, use '=', e.g. $a3=417.87\n";
     std::cout << "To display variable, type its name\n";

     std::cout << "If you place a '!' in the fromt of an expression,\n";
     std::cout << "the result will be followed by its internal representation (digits, number of postcomma digits, sign)\n";

     std::cout << "other commands:\n";
     std::cout << "setprec <prec>    set precision (i.e. number of digits)\n";
     std::cout << "setbase <prec>    set base (2 <= base < " << std::to_string(DigitOperationTables::max_base) << ")\n";
     std::cout << "                  (the currently used base is part of the input prompt)\n";
     std::cout << "show              display current precision, base and variable values\n";
     std::cout << "exit              exit program\n";
     std::cout << "\n";
     std::cout << "All numbers are internally represented by strings, and\n";
     std::cout << "all the operations are performed digit-wise using the\n";
     std::cout << "'normal' algorithms for calculations by hand\n";
     std::cout << "----------------------------------------------------------------------------\n";
     std::cout << "\n";
}
        


//----------------------------------------------------------------------------
// main
//
int main(int iArgC, char *apArgV[]) {
    int iResult = 0;
    uchar iBase = 10;
    if (iArgC > 1) {
        uint iB = atoi(apArgV[1]);
        if ((iB > 1) && (iB < DigitOperationTables::max_base)) {
            iBase = iB;
        } else {
            std::cout << "invalid base [" << apArgV[1]  << "] - ";
        }
    }
    std::cout << "using base " << uint(iBase) << "\n";
    Evaluator *pEv = new Evaluator(iBase);


    bool bGoOn = true;
    std::string s;

    std::cout << "Enter '?' for help, 'exit' or ctrl-D to exit\n";

    do {

        std::cout << "(" << uint(iBase) << ")> ";
        std::getline(std::cin, s);
        s.erase(0, s.find_first_not_of(' '));  

        if (s == "exit") {
            bGoOn = false;
        } else if (s == "show") {
            std::cout << pEv->listVars();
        } else if (s.find("setprec") == 0) {
            size_t pos = s.find_first_of("0123456789");
            if (pos != std::string::npos) {
                int iPrec = atoi(s.substr(pos).c_str());
                if (iPrec > 0) {
                    pEv->setPrec(iPrec);
                } else {
                    std::cout << "Inavlaid value for precision:[" << s.substr(pos) << "]\n";
                }
            } else {
                std::cout << "Expected integer after 'setprec':[" << s << "]\n";
            }
        } else if (s.find("setbase") == 0) {
            size_t pos = s.find_first_of("0123456789");
            if (pos != std::string::npos) {
                uint iTempBase = atoi(s.substr(pos).c_str());
                if ((iTempBase > 1) && (iTempBase < DigitOperationTables::max_base)) {
                    pEv->setBase(iTempBase);
                    iBase = iTempBase;
                } else {
                    std::cout << "Inavalid value for base:[" << s.substr(pos) << "]\n";
                }
            } else {
                    std::cout << "Expected integer after 'setbase':[" <<s << "]\n";
            }
        } else if (s == "?") {
            help();
        } else if (s == "") {
            bGoOn = false;
        } else { 
            // std::cout << "s:[%s](%zd), trim(s):[%s](%zd)\n", s.c_str(),s.length(), trim(s).c_str(), trim(s).length());
            if (trim(s).length() > 0) {
                bool bDebug = false;
                if (s[0] == '!') {
                    s = s.substr(1);
                    bDebug = true;
                }
                try{
                    LongNum res = pEv->processString(s);
                    std::cout << res.normalize().toString();
                    if (bDebug) {
                        std::cout << " : " + res.toDebug();
                    }
                    std::cout << "\n";
                } catch  (std::string sErr) {
                    std::cout << "Eception: [" << sErr << "]\n";
                    
                }
            }
        }
    } while(bGoOn);
    /*
    TokenScanner ts{apArgV[1]};
    ts.getNextToken();
    //scan(ts);

    LongNum zero("0", BASE);

    LongNum res = parseExpression(ts, BASE);
     std::cout << "result: [%s]\n", res.toString().c_str());
    */
    delete pEv;
    return iResult;
}
