.. _addition_ref:

Addition
========

.. rst-class:: hidden

   This page describes the :index:`Addition` algorithm

The algorithm for the addition of two positive numbers (``LongNum`` objects) with arbitrary length.
---------------------------------------------------------------------------------------------------

**Input**: 
    ``lN1``, ``lN2``

**I0**:  
    Get the strings ``s1``, ``s2`` corresponding to the input ``LongNum`` objects and make them compatible by prepending or appending '0' digits.
    
.. code-block:: c++
 
    std::string s1;
    std::string s2;
    uint iNewDigits = makeCompatibleStrings(lN1, lN2, s1, s2);

The position of the result's decimal point will be the larger of the decimal point positions of the two numbers.

**I1**: 
    | Set the result ``sResult`` to the empty string
    | Set the carry ``cCarry`` to the ``char`` '0'

.. code-block:: c++

    std::string sResult = "";
    char cCarry = '0';

**L0**:
    Loop through all digits, starting with the least significant one (at position 0).

**L1**: 
    Set the digit ``c1`` to the character at the current position in the string ``s1``.
        
.. code-block:: c++
   
    char c1 = s1.at(i);

**L2**: 
    Find the sum of the current carry and the digit in s2 from the :ref:`addition table<add_table_ref>`, with a potential new carry.
    
.. code-block:: c++
                
    const cresult cr1 = lN1.getOperationTables()->add(cCarry, c1);

The ``cresult`` object is a pair consisting of the sum of the digits anf the carry of this operation.

**L3**: 
    Find the sum of the previous result and the character at the current position in ``s2``, with a potential carry.

.. code-block:: c++ 

    const cresult cr = lN1.getOperationTables()->add(cr1.value, s2.at(i));

Note: the two carry values have at most the value 1, and only one of these two carry values is greater than 0. 

**L4**: 
    Append the result of the last sum to the result string.      

.. code-block:: c++ 

    sResult += cr.value;

**L5**: 
    Set the carry to the sum of the two potential carry values

.. code-block:: c++ 

    cCarry =  lN1.getOperationTables()->add(cr.carry,cr1.carry).value;

Note: this operation will have no carry, because at most one of the operands has value '1'.

**L6**:
    End loop

**Post1**:
    If there is a carry from the last loop iteration, then append it to the string.

.. code-block:: c++ 

    if (cCarry != '0') {
        sResult += cCarry;
    }


**Post2**:
   Create a ``LongNum`` with the correct number of digits after the decimal point and return it

.. code-block:: c++ 

    return LongNum(sResult, iNewDigits, lN1.getBase(), 1);


Regarding the potential carry values
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
A noted above, of the two carry values at most one has the value '1',

**The proof**

1. If you add two single digits ``d1``, ``d2`` with maximum value (i.e. ``d1 = d2 = base - '1'``) then the sum will be ``2*base-'2'``.
This yields a result digit with the value of ``base -'2'`` and a carry with value '1'.

2. If the sum of the ``d1`` and ``cCarry`` yields a carry, the digit's value will be '0'. So the sum of this with ``d2`` will not yield a carry.

**Example**

In base 7, 
    6 + 6 = 5 with carry 1
if there is a carry from the previous sum
    | (6+1)  = 0 with carry 1   
    | (6+1) + 6 = 0 + 6 = 6 (with no new carry). 
