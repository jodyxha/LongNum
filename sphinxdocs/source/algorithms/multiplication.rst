.. _multiplication_ref:

Multiplication
==============

.. rst-class:: hidden

   This page describes the :index:`Multiplication` algorithm

For the multiplication of to positive numbers, we use two algorithms:

  * :ref:`multiplying a number with a single digit<mul_single_ref>`
  * :ref:`multiplying two positive numbers<mul_pos_ref>`

.. _mul_single_ref:

The algorithm for the multiplication of a positive number with a single-digit Number
------------------------------------------------------------------------------------

**Input**:
  | ``lN``  positive ``LongNum`` object
  | ``d``   ``char``

**I1**: 
    | Set the result ``sResult`` to the empty string
    | Set the carry ``cCarry`` too the ``char`` '0'

.. code-block:: c++

    std::string sResult = "";
    char cCarry = '0';

**I2**:
    Get the string of ``lN`` digits into the string ``s1``

.. code-block:: c++

     std::string s1 = lN.getDigits();

**L0**:
 Loop through all digits in ``s1``, starting with the least significant one (at position 0).

**L1**: 
    Set the digit ``c1`` to the character at the current position in the string ``s1``.
        
.. code-block:: c++
   
    char c1 = s1.at(i);

**L2**: 
    Find the product if the digits ``c1`` and `d`` s2 from the :ref:`multiplication table<mul_table_ref>`, with a potential new carry.

.. code-block:: c++

    const cresult crp = lN.getOperationTables()->mul(c1, d);

The ``cresult`` object is a pair consisting of the product of the digits and the carry of this operation.

**L3**:
     Find the sum of the previous result and the carry value, with a potential carry, using the :ref:`addition table<add_table_ref>`.


.. code-block:: c++

    const cresult crc = lN.getOperationTables()->add(cCarry, crp.value);

**L4**: 
    Append the result of the last sum to the result string.      

.. code-block:: c++ 

    sResult += crc.value;

**L5**: 
    Set the carry to the sum of the two potential carry values

.. code-block:: c++ 

    cCarry =  lN.getOperationTables()->add(crc.carry,crp.carry).value;

**L6**:
  End Loop

**Post**:
    If there is a carry from the last loop iteration, then append it to the string.

.. code-block:: c++ 

    if (cCarry != '0') {
        sResult += cCarry;
    }

.. _mul_pos_ref:

The algorithm for the multiplication of two positive numbers
------------------------------------------------------------

**Input"**:
    | lN1  positive ``LongNum`` object
    | lN2  positive ``LongNum`` object

**I0**:
    Determine the number of digits after the decimal point for the product

.. code-block:: c++ 

    int iFinalDigits =  lN1.getPostDigits() + lN2.getPsorDigits(); 

**I1**:
    Initialize a ``LongNum`` object without digits

.. code-block:: c++ 

     LongNum lNResult("", lN1.gePostDigits(), lN1.getBase(), lN1.isNegative()?-1:1);

**I2**:
    Get the digit string of the second operand.

.. code-block:: c++ 

    std::string s2 = lN2.getDigits()

**L0**:
    Loop over the digits in ``s2``.

**L1**:
    Multiply the first operand with the current digit of the second operand (using the algorithm described above).

.. code-block:: c++ 

     LongNum lNTemp = mulSingle(lN1, s2.at(i));

**L2**:
    Add this product to the result ``LongNum``

.. code-block:: c++ 

    lNResult = lNResult.add(lNTemp);

**L3**:
    Shift the fist operanby one digit.

.. code-block:: c++ 

    lN1.shift(1);

**L4**:
   End Loop

**Post**:
   Create a ``LongNum`` with the correct number of digits after the decimal point and return it

.. code-block:: c++ 

    return LongNum(lNResult.getDigits(), iFinalDigits, lNResult.getBase(), lNResult.isNegative()?-1:1);
