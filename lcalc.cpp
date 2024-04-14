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
    printf("\n----------------------------------------------------------------------------\n");
    printf("interactive calculator for arbitrary-length numbers in arbitrary bases\n");
    printf("supported operations:\n");
    printf("'+'   addition\n");
    printf("'-'   subtraction\n");
    printf("'*'   multiplication\n");
    printf("'/'   division\n");
    printf("sqrt  square-root function\n");
    printf("\n");
    printf("other commands:\n");
    printf("setprec <prec>    set precision (i.e. number of digits)\n");
    printf("setbase <prec>    set base (2 <= base < 36)\n");
    printf("show              display current precision, base and variable values\n");
    printf("exit              exit program\n");
    printf("\n");
    printf("All numbers are internally represented by strings, and\n");
    printf("all the operations are performed digit-wise using the\n");
    printf("'normal' algorithms for calculations by hand\n");
    printf("----------------------------------------------------------------------------\n");
    printf("\n");
}
        


//----------------------------------------------------------------------------
// main
//
int main(int iArgC, char *apArgV[]) {
    int iResult = 0;
    uchar iBase = 10;
    if (iArgC > 1) {
        int iB = atoi(apArgV[1]);
        if ((iB > 1) && (iB < 36)) {
            iBase = iB;
        } else {
            std::cout << "invalid base [" << apArgV[1]  << "] - ";
        }
    }
    std::cout << "using base " << int(iBase) << "\n";
    Evaluator *pEv = new Evaluator(iBase);


    bool bGoOn = true;
    std::string s;

    std::cout << "Enter '?' for help, 'exit' or ctrl-D to exit\n";

    do {

        std::cout << "(" << int(iBase) << ")> ";
        std::getline(std::cin, s);
      
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
                    printf("Inavlaid value for precision:[%s]\n", s.substr(pos).c_str());
                }
            } else {
                printf("Expected integer after 'setprec':[%s]\n", s.c_str());
            }
        } else if (s.find("setbase") == 0) {
            size_t pos = s.find_first_of("0123456789");
            if (pos != std::string::npos) {
                iBase = atoi(s.substr(pos).c_str());
                if ((iBase > 1) && (iBase < 36)) {
                    pEv->setBase(iBase);
                } else {
                    printf("Inavalid value for base:[%s]\n", s.substr(pos).c_str());
                }
            } else {
                printf("Expected integer after 'setbase':[%s]\n", s.c_str());
            }
        } else if (s == "?") {
            help();
        } else if (s == "") {
            bGoOn = false;
        } else { 
            //printf("s:[%s](%zd), trim(s):[%s](%zd)\n", s.c_str(),s.length(), trim(s).c_str(), trim(s).length());
            if (trim(s).length() > 0) {
                try{
                    LongNum res = pEv->processString(s);
                    std::cout << res.normalize().toString() << "\n";
                } catch  (std::string sErr) {
                    printf("Eception: [%s]\n", sErr.c_str());
                    
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
    printf("result: [%s]\n", res.toString().c_str());
    */
    delete pEv;
    return iResult;
}
