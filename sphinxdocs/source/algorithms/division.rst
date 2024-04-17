.. _division_ref:

Division
=========

.. rst-class:: hidden

   This page describes the :index:`DivisiionSubtraction` algorithm

For the multiplication of to positive numbers, we use some helper functions for the algorithm:

  * :ref:`collecting leading digits of the dividend <div_collect_leading_ref>`
  * :ref:`getting the mext digit of the dividend <div_get_next_ref>`
  * :ref:`division by repeated subtraction <div_by_sub_ref>`
  * :ref:`division by repeated subtraction <div_by_sub_ref>`
  * :ref:`dividing two positive numbers <div_pos_ref>`

.. _div_get_next_ref:

Getting the next digit from the dividend
----------------------------------------

Get the next digit from the dividend and add it after the least significant digit of ``lNRemainder``\ 's digits.
If there are no more digits, use a '0'.

.. code-block:: c++

    std::string LongNum::collectNextDigit(std::string &sDigits, LongNum lNRemainder, bool *pbDotSet) {
  
        std::string sRemainder = lNRemainder.normalize().getDigits();
        if (sDigits.length() > 0) {
            sRemainder =  sDigits.back() +  sRemainder;
            sDigits = sDigits.substr(0, sDigits.length()-1);
        } else {
            sRemainder =  "0" + sRemainder; // first 0? if yes: notify remember position
            *pbDotSet |= true;
        }
        return sRemainder;
    }        

``sDigits`` 
    string containing the remaining digits of the dividend

``sRemainder`` 
    string containing the remainder of the last integer division

Returns a string consisting of ``lNRemainder``\ 's digits whith an additional digit after the least significant digit.

.. _div_by_sub_ref:

Division by repeated subtraction
--------------------------------

During the actual division algorithm we need integer division with remainder.
We implement this by repeatedly subtracting the divisor from the dividend.

.. code-block:: c++

    uchar LongNum::simpleDiv(LongNum lN1, LongNum lN2, LongNum &lNRemainder) {
        uchar iC = 0;
         while ((lN2 <= lN1)) {
            lN1 = lN1 - lN2;
            iC++;
         }
        lNRemainder = lN1;
        return iC;
    }



.. _div_pos_ref:

The algorithm for the division of two positive numbers (``LongNum`` objects) with arbitrary length.
------------------------------------------------------------------------------------------------------

**Input**:
    | lN1  positive ``LongNum`` object
    | lN2  positive ``LongNum`` object

**I0**:
    Shift both numbers by the same amount so that neither has any post decimal digits.

.. code-block:: c++

     uint c1 = lN1.getPostDigits();
     uint c2 = lN2.getPostDigits();
     uint iS = (c1 > c2)?c1:c2;
            
     lN1.shift(iS);
     lN2.shift(iS);



**I1**:
    | Get the smallest leading substring of ``lN1``\ 's digits which forms a number larger than ``lN2``. 
    | If ``lN1``\ 's digits run out before, kepp adding '0' digits (and count them)

.. code-block:: c++

    std::string s1 = lN1.getDigits();
    uint iPostDigits = 0;
    std::string sSelected = collectLeadingDigits(s1, lN2, &iPostDigits);

The function ``collectLeadingDigits()`` removes the selected digits from ``s1``
    
**L0**:
    Loop until desired precision (i.e., number of digits) is reached

**L1**:
    Turn the selected string to ``LongNum``

.. code-block:: c++

    LongNum lNSelected(sSelected, 0, lN1.getBase(), 1);

**L2**:
    Perform a simple division of the selected number and ``lN2``

.. code-block:: c++

    LongNum lNRest(lN1.getBase());  
    uchar u = simpleDiv(lNSelected, lN2, lNRest);

**L3**:
    Add the symbol for the number ``u`` afer the least significant digit of the result

.. code-block:: c++

    sResult =  DigitOperationTables::getDigitSym(u) + sResult;

**L4**:
    Gather one more digit from ``s1``. If those digits are used up, add a '0' after the least significant bit and increase digit counter.

.. code-block:: c++

    sSelected = collectNextDigit(s1, lNRest, &bDotSet);

**L5**:
    End loop.

**Post1**:
    Add '0' digits in front of the most significant digit, so that the decmal point ends up in the correct place

.. code-block:: c++

    while (sResult.length() < iPostDigits) {
        sResult = sResult + "0";
    }
          
**Post2**:
    Create a ``LongNum`` object from the digits in the result string.

.. code-block:: c++

    NResult = LongNum(sResult, iPostDigits, lN1.getBase(), lN1.isNegative()?-1:1);
