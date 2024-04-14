#include <string.h>
#include <stdio.h>
#include <math.h>

#include "BaseManager.h"
#include "LongNum.h"

void showNumber(std::string sInput, LongNum &lN, uchar iBase) {
    printf(" LongNum n1(\"%s\",%u): [%s](%d)\n", sInput.c_str(), iBase, lN.toString().c_str(), lN.getCurCommas());
}

void showResult(LongNum lN1, LongNum lN2, LongNum lNRes, std::string sOp) {
    printf(" LongNum [%s](%d) %s [%s](%d): [%s](%d)\n", lN1.normalize().toString().c_str(), lN1.getCurCommas(), sOp.c_str(), lN2.normalize().toString().c_str(), lN2.getCurCommas(), lNRes.normalize().toString().c_str(), lNRes.getCurCommas());
}




typedef std::vector<std::pair<std::string, LongNum>> testvars;

void additions(testvars &mVars, uchar iBase)  {
    printf("--- additions ---\n");
    LongNum one("1", iBase);
    LongNum zero("0", iBase);

    LongNum ns = mVars[0].second.add(mVars[1].second);
    showResult(mVars[0].second, mVars[1].second, ns, "+");
    ns = mVars[0].second.add(mVars[2].second);
    showResult(mVars[0].second, mVars[2].second, ns, "+");
    ns = mVars[3].second.add(mVars[1].second);
    showResult(mVars[3].second, mVars[1].second, ns, "+");
    ns = mVars[3].second.add(mVars[2].second);
    showResult(mVars[3].second, mVars[2].second, ns, "+");

    ns = mVars[1].second.add(zero);
    showResult(mVars[1].second, zero, ns, "+");
    ns = zero.add(mVars[1].second);
    showResult(zero, mVars[1].second, ns, "+");
    LongNum ns2 = mVars[0].second+mVars[1].second+mVars[2].second;
    printf("[%s] + [%s] + [%s] = [%s]\n",   mVars[0].first.c_str(), mVars[1].first.c_str(), mVars[2].first.c_str(), ns2.toString().c_str());

    LongNum n1(mVars[0].second);
    LongNum n2(mVars[1].second);
    printf("n1:[%s] n2:[%s]\n",   n1.toString().c_str(), n2.toString().c_str());
    LongNum n3 = n1 + n2;
    printf("n1:[%s] n2:[%s] n3:[%s]\n",   n1.toString().c_str(), n2.toString().c_str(), n3.toString().c_str());
}


void comparisons(testvars &mVars, uchar iBase)  {
    printf("--- comparisons ---\n");
    printf(" [%s] < [%s]: %s\n", mVars[0].second.toString().c_str(), mVars[0].second.toString().c_str(), (mVars[0].second.operator<( mVars[0].second))?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[0].second.toString().c_str(), mVars[1].second.toString().c_str(), (mVars[0].second < mVars[1].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[0].second.toString().c_str(), mVars[2].second.toString().c_str(), (mVars[0].second < mVars[2].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[0].second.toString().c_str(), mVars[3].second.toString().c_str(), (mVars[0].second < mVars[3].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[1].second.toString().c_str(), mVars[0].second.toString().c_str(), (mVars[1].second < mVars[0].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[1].second.toString().c_str(), mVars[1].second.toString().c_str(), (mVars[1].second < mVars[1].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[1].second.toString().c_str(), mVars[2].second.toString().c_str(), (mVars[1].second < mVars[2].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[1].second.toString().c_str(), mVars[3].second.toString().c_str(), (mVars[1].second < mVars[3].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[2].second.toString().c_str(), mVars[0].second.toString().c_str(), (mVars[2].second < mVars[0].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[2].second.toString().c_str(), mVars[1].second.toString().c_str(), (mVars[2].second < mVars[1].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[2].second.toString().c_str(), mVars[2].second.toString().c_str(), (mVars[2].second < mVars[2].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[2].second.toString().c_str(), mVars[3].second.toString().c_str(), (mVars[2].second < mVars[3].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[3].second.toString().c_str(), mVars[0].second.toString().c_str(), (mVars[3].second < mVars[0].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[3].second.toString().c_str(), mVars[1].second.toString().c_str(), (mVars[3].second < mVars[1].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[3].second.toString().c_str(), mVars[2].second.toString().c_str(), (mVars[3].second < mVars[2].second)?"yes":"no"); 
    printf(" [%s] < [%s]: %s\n", mVars[3].second.toString().c_str(), mVars[3].second.toString().c_str(), (mVars[3].second < mVars[3].second)?"yes":"no"); 
}


void subtractions(testvars &mVars, uchar iBase)  {
    printf("--- subtractions ---\n");
    LongNum one("1", iBase);
    LongNum zero("0", iBase);

    LongNum nd =mVars[0].second.sub(mVars[1].second).normalize();
    showResult(mVars[0].second, mVars[1].second, nd, "-");
    nd = mVars[0].second.sub(mVars[2].second).normalize();
    showResult(mVars[0].second, mVars[2].second, nd, "-");
    nd = mVars[3].second.sub(mVars[1].second).normalize();
    showResult(mVars[3].second, mVars[1].second, nd, "-");
    nd = mVars[3].second.sub(mVars[2].second).normalize();
    showResult(mVars[3].second, mVars[2].second, nd, "-");
    nd = mVars[0].second.sub(mVars[0].second).normalize();
    showResult(mVars[0].second, mVars[0].second, nd, "-");
    nd = mVars[0].second.sub(zero).normalize();
    showResult(mVars[0].second, zero, nd, "-");
    nd = zero.sub(mVars[0].second).normalize();
    showResult(zero, mVars[0].second, nd, "-");
}


void multiplications(testvars &mVars, uchar iBase)  {
    printf("--- multiplications ---\n");
    LongNum one("1", iBase);
    LongNum zero("0", iBase);

    /*
    LongNum md = LongNum::mulSingle(mVars[0].second, '5');
    printf(" LongNum [%s] * [%c]: [%s]\n",  mVars[0].second.toString().c_str(), '5', md.toString().c_str());

    md = LongNum::mulSingle(mVars[0].second, '7');
    printf(" LongNum [%s] * [%c]: [%s]\n",  mVars[0].second.toString().c_str(), '7', md.toString().c_str());
    */

    LongNum md = mVars[1].second.mul(mVars[2].second).normalize();
    showResult(mVars[1].second, mVars[2].second, md, "*");

    md = mVars[1].second.mul(zero).normalize();
    showResult(mVars[1].second, zero, md, "*");

    md = mVars[1].second.mul(one).normalize();
    showResult(one, mVars[1].second, md, "*");

}


int main1(int iArgC, char *apArgV[]) {
    int iResult = 0;
    uchar iBase = 10;
    if (iArgC > 1) {
        int iB = atoi(apArgV[1]);
        if (iB > 0) {
            iBase = iB;
        } else {
            printf("ionvauid bas (must be > 0): [%s]\n", apArgV[1]);
        }
    }

    try {
    testvars mVars;
    /*
    std::string s1("132.3144201");
    std::string s2("4520.316");
  
    std::string s1("777.777777");
    std::string s2("7777.777");
    std::string s3("777.7767");
    */
    
    
    std::string s1("777.77");
    std::string s2("77.777");
    std::string s3("-777.7767");
    std::string s4("-723.1");
    LongNum one("1", iBase);
    LongNum zero("0", iBase);

    LongNum n1(s1, iBase);
    LongNum n2(s2, iBase);
    LongNum n3(s3, iBase);
    LongNum n4(s4, iBase);

    
    showNumber( s1, n1, iBase);
    showNumber( s2, n2, iBase);
    showNumber( s3, n3, iBase);
    showNumber( s4, n4, iBase);
    mVars.push_back(std::pair<std::string, LongNum>(s1,n1));
    mVars.push_back(std::pair<std::string, LongNum>(s2,n2));
    mVars.push_back(std::pair<std::string, LongNum>(s3,n3));
    mVars.push_back(std::pair<std::string, LongNum>(s4,n4));
    
    printf("--- additions new ---\n");

    additions(mVars, iBase);

    
    comparisons(mVars, iBase);

    
    subtractions(mVars, iBase);
 
    // printf("--- multiplications ---\n");
    std::string t1("1234");
    LongNum m1(t1, iBase);
    showNumber(t1, m1, iBase);
    std::string t2("775.34");
    LongNum m2(t2, iBase);
    showNumber(t2, m2, iBase);
    std::string t3("17.734");
    LongNum m3(t3, iBase);
    showNumber(t3, m3, iBase);
    std::string t4("17");
    LongNum m4(t4, iBase);
    showNumber(t4, m4, iBase);
    std::string t5(".3");
    LongNum m5(t5, iBase);
    showNumber(t5, m5, iBase);
    std::string t6(".321");
    LongNum m6(t6, iBase);
    showNumber(t6, m6, iBase);
    std::string t7("0.321");
    LongNum m7(t7, iBase);
    showNumber(t7, m7, iBase);
    std::string t8("15.");
    LongNum m8(t8, iBase);
    showNumber(t8, m8, iBase);
    mVars.push_back(std::pair<std::string, LongNum>(t1,m1));
    mVars.push_back(std::pair<std::string, LongNum>(t2,m2));
    mVars.push_back(std::pair<std::string, LongNum>(t3,m3));
    mVars.push_back(std::pair<std::string, LongNum>(t4,m4));
    mVars.push_back(std::pair<std::string, LongNum>(t5,m5));
    mVars.push_back(std::pair<std::string, LongNum>(t6,m6));
    mVars.push_back(std::pair<std::string, LongNum>(t7,m7));
    mVars.push_back(std::pair<std::string, LongNum>(t8,m8));

    multiplications(mVars, iBase);
   


    printf("--- divisions ---\n");
    uint iPrecision =  12;
    
    std::string u1("1234");
    LongNum o1(u1, iBase);
    showNumber(u1, o1, iBase);
    std::string u2("543");
    LongNum o2(u2, iBase);
    showNumber(u2, o2, iBase);
    
    std::string u3("1233");
    LongNum o3(u3, iBase);
    showNumber(u3, o3, iBase);
    std::string u4("76543210");
    LongNum o4(u4, iBase);
    o4.normalize();
    showNumber(u4, o4, iBase);
     
    printf("---- [%s] / [%s] ----\n", o1.toString().c_str(), o2.toString().c_str());
    LongNum os = LongNum::divNaturals(o1, o2, iPrecision);
    showResult(o1, o2, os, "/");

    LongNum os1 = LongNum::mulNaturals(o2, os);
    showResult(o2, os, os1, "*");

    printf("---- [%s] / [%s] ----\n", o2.toString().c_str(), o1.toString().c_str());
    os = LongNum::divNaturals(o2, o1, iPrecision);
    showResult(o2, o1, os, "/");
    os1 = LongNum::mulNaturals(o1, os);
    showResult(o1, os, os1, "*");
    printf("---- [%s] / [%s] ----\n", o1.toString().c_str(), o3.toString().c_str());
    os = LongNum::divNaturals(o1, o3, iPrecision);
    showResult(o1, o3, os, "/");
    os1 = LongNum::mulNaturals(o3, os);
    showResult(o3, os, os1, "*");
    printf("---- [%s] / [%s] ----\n", o1.toString().c_str(), o1.toString().c_str());
    os = LongNum::divNaturals(o1, o1, iPrecision);
    showResult(o1, o1, os, "/");
    os1 = LongNum::mulNaturals(o1, os);
    showResult(o1, os, os1, "*");
    
    printf("---- [%s] / [%s] ----\n", o4.toString().c_str(), o2.toString().c_str());
    os = LongNum::divNaturals(o4, o2, iPrecision);
    showResult(o4, o2, os, "/");
    os1 = LongNum::mulNaturals(o2, os);
    showResult(o2, os, os1, "*");

    //    os1 = LongNum::divNaturals(o2, zero, iPrecision);
    //    showResult(o2, zero, os1, "/");

    os1 = LongNum::divNaturals(zero, o2, iPrecision);
    showResult(zero, o2, os1, "/");

    os1 = LongNum::divNaturals(one, o2, iPrecision);
    showResult(one, o2, os1, "/");


    os1 = LongNum::divNaturals(one, one, iPrecision);
    showResult(one, one, os1, "/");

    std::string uA("2613");
    LongNum oA(uA, iBase);
    showNumber(uA, oA, iBase);
    std::string uB("100");
    LongNum oB(uB, iBase);
    showNumber(uB, oB, iBase);
    std::string uC("2000");
    LongNum oC(uC, iBase);
    showNumber(uC, oC, iBase);
    std::string uD("40");
    LongNum oD(uD, iBase);
    showNumber(uD, oD, iBase);
    std::string uE("41.3227");
    LongNum oE(uE, iBase);
    showNumber(uE, oE, iBase);
    std::string uF("4.13227");
    LongNum oF(uF, iBase);
    showNumber(uF, oF, iBase);
    std::string uG("0.413227");
    LongNum oG(uG, iBase);
    showNumber(uG, oG, iBase);
    std::string uH("0.0413227");
    LongNum oH(uH, iBase);
    showNumber(uH, oH, iBase);
    std::string uI("0.00413227");
    LongNum oI(uI, iBase);
    showNumber(uI, oI, iBase);

    
    os1 = LongNum::divNaturals(one, oA, iPrecision);
    showResult(one, oA, os1, "/");


    os1 = LongNum::divNaturals(oA, oA, iPrecision);
    showResult(oA, oA, os1, "/");

    os1 = LongNum::divNaturals(oB, oC, iPrecision);
    showResult(oB, oC, os1, "/");

    os1 = LongNum::divNaturals(oD, oA, iPrecision);
    showResult(oD, oA, os1, "/");


    os1 = LongNum::divNaturals(one, one, iPrecision);
    showResult(one, one, os1, "/");

    double dTest1;

    /*
    LongNum sqA = LongNum::sqrt(oA, iPrecision);
    printf("sqrt(%s) = %s\n", oA.toString().c_str(), sqA.toString().c_str());
    LongNum sqB = LongNum::sqrt(oB, iPrecision);
    printf("sqrt(%s) = %s\n", oB.toString().c_str(), sqB.toString().c_str());
    */
    LongNum sqE = LongNum::sqrt(oE, iPrecision);
    printf("sqrt(%s) = %s\n", oE.toString().c_str(), sqE.toString().c_str());
    dTest1 = atof(uE.c_str());
    printf("calc: sqrt(%f) = %f [%s]\n", dTest1, sqrt(dTest1), sqE.toString().c_str());
  
    LongNum sqF = LongNum::sqrt(oF, iPrecision);
    printf("sqrt(%s) = %s\n", oF.toString().c_str(), sqF.toString().c_str());
    dTest1 = atof(uF.c_str());
    printf("calc: sqrt(%f) = %f [%s]\n", dTest1, sqrt(dTest1), sqF.toString().c_str());
    
    LongNum sqG = LongNum::sqrt(oG, iPrecision);
    printf("sqrt(%s) = %s\n", oG.toString().c_str(), sqG.toString().c_str());
    dTest1 = atof(uG.c_str());
    printf("calc: sqrt(%f) = %f [%s]\n", dTest1, sqrt(dTest1), sqG.toString().c_str());

    LongNum sqH = LongNum::sqrt(oH, iPrecision);
    printf("sqrt(%s) = %s\n", oH.toString().c_str(), sqH.toString().c_str());
    dTest1 = atof(uH.c_str());
    printf("calc: sqrt(%f) = %f [%s]\n", dTest1, sqrt(dTest1), sqH.toString().c_str());

    LongNum sqI = LongNum::sqrt(oI, iPrecision);
    printf("sqrt(%s) = %s\n", oI.toString().c_str(), sqI.toString().c_str());
    dTest1 = atof(uI.c_str());
    printf("calc: sqrt(%f) = %f [%s]\n", dTest1, sqrt(dTest1), sqI.toString().c_str());

    printf("+++\n");
    
    printf("sqrt(0) = %s\n", LongNum::sqrt(LongNum('0', iBase), iPrecision).toString().c_str());
    printf("sqrt(1) = %s\n", LongNum::sqrt(LongNum('1', iBase), iPrecision).toString().c_str());
    printf("sqrt(2) = %s\n", LongNum::sqrt(LongNum('2', iBase), iPrecision).toString().c_str());
    printf("sqrt(3) = %s\n", LongNum::sqrt(LongNum('3', iBase), iPrecision).toString().c_str());
    printf("sqrt(5) = %s\n", LongNum::sqrt(LongNum('5', iBase), iPrecision).toString().c_str());

    LongNum NRem("0200", iBase);
    LongNum NTester("20", iBase);
    printf("%s /%s = %s\n", NRem.toString().c_str(), NTester.toString().c_str(), (NRem.div(NTester, iPrecision)).toString().c_str());
    LongNum N10("3.12", iBase);
    LongNum p0("0", iBase);
    LongNum p1("1", iBase);
    LongNum p2("2", iBase);
    LongNum p("13", iBase);
    printf("pow([%s],[%s]) = [%s]\n", N10.toString().c_str(), p0.toString().c_str(), LongNum::pow(N10,p0).toString().c_str());
    printf("pow([%s],[%s]) = [%s]\n", N10.toString().c_str(), p1.toString().c_str(), LongNum::pow(N10,p1).toString().c_str());
    printf("pow([%s],[%s]) = [%s]\n", N10.toString().c_str(), p2.toString().c_str(), LongNum::pow(N10,p2).toString().c_str());
    printf("pow([%s],[%s]) = [%s]\n", N10.toString().c_str(), p.toString().c_str(), LongNum::pow(N10,p).toString().c_str());

    LongNum N2(LongNum::changeBase(N10, 2));
    printf("[%s] in base [%c] = [%s] in base [%c]\n",N10.toString().c_str(), N10.getOperationTables()->getDigitSym(N10.getBase()), N2.toString().c_str(), '2' );
    LongNum N7(LongNum::changeBase(N2, 7));
    printf("[%s] in base [%c] = [%s] in base [%c]\n",N2.toString().c_str(), N2.getOperationTables()->getDigitSym(N2.getBase()), N7.toString().c_str(), '7' );
    LongNum N16(LongNum::changeBase(N7, 16));
    printf("[%s] in base [%c] = [%s] in base [%c]\n",N7.toString().c_str(), N7.getOperationTables()->getDigitSym(N7.getBase()), N16.toString().c_str(), 'g' );
    LongNum N10b(LongNum::changeBase(N16, 10));
    printf("[%s] in base [%c] = [%s] in base [%c]\n",N16.toString().c_str(), N16.getOperationTables()->getDigitSym(N16.getBase()), N10b.toString().c_str(), 'a' );
    
    } catch (std::string sErr) {
        printf("Eception: [%s]\n", sErr.c_str());
        
    }
    
    /*
    printf(                                   \
    "TODO test:"
        "multiplication with 0"
        "multiplication with 1");
    */
    BaseManager::freeInstance();

    return iResult;
}



int main(int iArgC, char *apArgV[]) {

    /*
    if (iArgC > 2) {
        LongNum N(apArgV[1], 10);
        char *p = strchr(apArgV[1], '.');
        if (p != NULL) {
            p++;
            while (*p != '\0') {
              *p++ = '\0';
            }
        }
        LongNum M(apArgV[1], 10);
        / *
        N.normalize();
        M.normalize();
        X.normalize();
        * /
        printf("N:%s (%s, %d, %d)\n", N.toString().c_str(), N.getDigits().c_str(), N.getCurCommas(), N.getSign());
        N = LongNum::truncate(N);
        printf("N:%s (%s, %d, %d)\n", N.toString().c_str(), N.getDigits().c_str(), N.getCurCommas(), N.getSign());
        printf("M:%s (%s, %d, %d)\n", M.toString().c_str(), M.getDigits().c_str(), M.getCurCommas(), M.getSign());
        char q = N.normalize().getDigits().back();
        LongNum Q = LongNum(q, 10);
        printf("Q:%s (%s, %d, %d)\n", Q.toString().c_str(), Q.getDigits().c_str(), Q.getCurCommas(), Q.getSign());
        
    }
    */

    /*    
    LongNum NTester("0222121", 0, 3, 1);
    LongNum NRem("002021", 0, 3, 1);
    LongNum NQ = NRem.div(NTester, 20);
    NQ = LongNum::truncate(NQ);
    //020 120 021 121 201 220 22
    */

    for (uchar base = 2; base < 20; base++) {
        for (uint i = 0; i < 100; i++) {
            char s0[1024];
            sprintf(s0, "%d", i);
            LongNum s(s0, 10);
            s = LongNum::changeBase(s, base);
            LongNum r = LongNum::sqrt(s, 20);
            LongNum q = r*r;
            printf("%s (%s,%d, %d): ", s.toString().c_str(), s.getDigits().c_str(), s.getCurCommas(), s.getBase());
            printf("%s (%s,%d, %d): -> ", r.toString().c_str(), r.getDigits().c_str(), r.getCurCommas(), r.getBase());
            printf("%s (%s,%d, %d)\n", q.toString().c_str(), q.getDigits().c_str(), q.getCurCommas(), q.getBase());
            
        }
    }
    

    /*
    
    LongNum s("1", 2);
    DigitOperationTables *pDOT = s.getOperationTables();
    printf("additions:\n");
    pDOT->showTable(pDOT->m_addition);
    printf("subtractions:\n");
    pDOT->showTable(pDOT->m_subtraction);
    printf("multiplications:\n");
    pDOT->showTable(pDOT->m_multiplication);
    printf("divisions:\n");
    pDOT->showTable(pDOT->m_division);
    LongNum r = LongNum::sqrt(s, 10);
    printf("s: %s (%s,%d, %d)\n", s.toString().c_str(), s.getDigits().c_str(), s.getCurCommas(), s.getBase());
    printf("r: %s (%s,%d, %d)\n", r.toString().c_str(), r.getDigits().c_str(), r.getCurCommas(), r.getBase());
    */  

    return 0;
}
