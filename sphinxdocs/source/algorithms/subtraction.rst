.. _subtraction_ref:

Subtraction
===========

.. rst-class:: hidden

   This page describes the :index:`Subtraction` algorithm

The algorithm for the subtraction of two positive numbers (``LongNum`` objects) with arbitrary length.
------------------------------------------------------------------------------------------------------

**Input**: 
    ``lN1``, ``lN2``

**I0**:  
    Get the strings ``s1``, ``s2`` corresponding to the input ``LongNum`` objects and make them compatible by prepending or appending '0' digits.
    
.. code-block:: c++
 
    std::string s1;
    std::string s2;
    uint iNewPostDigits = makeCompatibleStrings(lN1, lN2, s1, s2);

The position of the result's decimal point will be the larger of the decimal point positions of the two numbers.

**I1**:
   If ``lN1`` ins greater then ``lN2``, then swap the values and remeber the swapping.

.. code-block:: c++
 
    if (lN1 < lN2) {
        s1.swap(s2);
        bSwapped = true;
    }

**I2**:
    | set the result ``sResult`` to the empty string
    | set the carry ``cCarry`` too the ``char`` '0'

.. code-block:: c++

    std::string sResult = "";
    char cCarry = '0';

**L0**:
    loop through all digits, starting with the least significant one (at position 0).

**L1**:
    Get the digit at the current from each string.

.. code-block:: c++

    char c1 = s1.at(i);
    char c2 = s2.at(i);
    
**L2**:
    Find the sum of the current carry and the digit ``c2`` from the :ref:`addition table<add_table_ref>`, with a potential new carry.

.. code-block:: c++

    const cresult cr1 = lN1.getOperationTables()->add(cCarry, c2);

The ``cresult`` object is a pair consisting of the difference of the two digits and the carry of this operation.

**L3**:
    Find the difference between ``c1`` and the previous result from the :ref:`subtraction table<sub_table_ref>`, with a potential carry.

.. code-block:: c++

    const cresult cr = lN1.getOperationTables()->sub(c1, cr1.value);

 
Note: the two carry values have at most the value 1, and only one of these two carry values is greater than 0. 

**L4**:
    Append the result of the last sum to the result string.      

.. code-block:: c++

     sResult += cr.value;

**L5**:
    Set the carry to the sum of the two potential carry values

.. code-block:: c++

     cCarry =  lN1.getOperationTables()->add(cr.carry,cr1.carry).value;

**L6**:
    End loop.

**Post**:
   Create a ``LongNum`` with the correct number of digits after the decimal point and return it

.. code-block:: c++ 

    return LongNum(sResult, iNewPostDigits, lN1.getBase(), 1);
