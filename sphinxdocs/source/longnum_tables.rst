.. _operation_tables_ref:

The Operation Tables
====================

For all longhand algorithms it is necessary to have knowledge of 
the arithmetic operations on single digits, for all bases..
For this, the class ``DigitOperationTables`` defines lookup tables for all 
basic operations except for division:

  * :ref:`The Addition Table<add_table_ref>`
  * :ref:`The Subtraction Table<sub_table_ref>`
  * :ref:`The Multiplication Table<mul_table_ref>`
  * :ref:`The Square  Root Table<sqrt_table_ref>`

For division, there we use simple euclidean division during computation.

Each table is a map of maps of structures holding the operation result 
and the carry of the operation. All maps are indexed by the digits (``char``).

.. code-block:: c++

    // data structure for elementary digit operation results
    typedef struct {
        char carry;
        char value;
    } cresult;

    // look-up table: op_table[c1][c2] = (carry, (c1 op c2)% base)
    typedef std::map<char, cresult>   op_table_row;
    // look-up table: first operand => op_table_row
    typedef std::map<char, op_table_row> op_table;

For square roots there is a look-up for all two-digit numbers 
returning the largest square lower than the given number.

The square root look-up type is different from the other tables:

.. code-block:: c++

    // lookup for elementary square roots 
    typedef std::map<std::string, char>   sqrt_table;


Some Examples
~~~~~~~~~~~~~
The **addition table for base 3** is:

.. code-block:: c++

    m_addition['0']['0'] = ('0', '0')
    m_addition['0']['1'] = ('0', '1')
    m_addition['0']['2'] = ('0', '2')
    m_addition['1']['0'] = ('0', '1')
    m_addition['1']['1'] = ('0', '2')
    m_addition['1']['2'] = ('1', '0')
    m_addition['2']['0'] = ('0', '2')
    m_addition['2']['1'] = ('1', '0')
    m_addition['2']['2'] = ('1', '1')

For square roots there is a look up for all two-digit numbers 
returning the highest square lower than the given number.

The **square root table for base 4** is:

.. code-block:: c++

    m_sqrt["00"] = '0'
    m_sqrt["01"] = '1'
    m_sqrt["02"] = '1'
    m_sqrt["03"] = '1'
    m_sqrt["10"] = '2'
    m_sqrt["11"] = '2'
    m_sqrt["12"] = '2'
    m_sqrt["13"] = '2'
    m_sqrt["20"] = '2'
    m_sqrt["21"] = '3'
    m_sqrt["22"] = '3'
    m_sqrt["23"] = '3'
    m_sqrt["30"] = '3'
    m_sqrt["31"] = '3'
    m_sqrt["32"] = '3'
    m_sqrt["33"] = '3'


The Digits
----------

The array ``dig_list`` serves as a look-up for digit symbols corresponding 
to numbers. 

.. code-block:: c++

    static constexpr const char dig_list[] = "0123456789abcdefghijklmnopqrstuvwxyz";

We have limited the range of bases to be between 2 and 36 
(numerals and lowercase alphabet) in order to have number representations 
that don't look too odd, as oppposed to, for example, ``4$9a%?_3}!2``.


.. _add_table_ref:

The Addition Table
------------------

The elements of the addition tables are pairs of carry and addition results 
indexed by digits.
The array ``dig_list`` serves as a look-up for digit symbols corresponding 
to numbers. We have limited the range of bases to be between 2 and 36 
(numerals and lowercase alphabet) in order to have number representations 
that don't look too odd, like for example ``4$9a%?_3}!2``.


.. code-block:: c++

    op_table m_addition;

    for (uchar i = 0; i < iBase; i++) {
        op_table_row tr_add;
        for (uchar j = 0; j < iBase; j++) {
            tr_add[dig_list[j]] = {dig_list[(i+j)/iBase], dig_list[(i+j)%iBase]};
        }
        m_addition[dig_list[i]]       = tr_add;
    }

So if ``c1`` and ``c2`` are two digits repreeneted in a particular base, and ``cr = m_addition[c1][c2]``,
then ``cr.value`` is the digit representing the sum (modulo the base) of the values ``c1`` and ``c2`` represent, 
and ``cr.carry`` is the symbol for the carry of the addition.


.. _sub_table_ref:

The Subtraction Table
---------------------

The elements of the subtraction tables are pairs of carry and subtraction 
results indexed by digits.


.. code-block:: c++

    op_table m_subtraction;

    for (uchar i = 0; i < iBase; i++) {
        op_table_row tr_sub;
        for (uchar j = 0; j < iBase; j++) {

            if (j <= i) {
                tr_sub[dig_list[j]] = {'0', dig_list[(i-j)]};
            } else{
                // we can't do (i-j)%iBase because '%' doesn't work well for negative dividends
                tr_sub[dig_list[j]] = {'1', dig_list[(i+iBase-j)%iBase]};
            }
        }
    }

.. code-block:: text

    +-----+----------+----------+----------+----------+
    |     |    '0'   |    '1'   |    '2'   |    '3'   |
    +-----+----------+----------+----------+----------+
    | '0' | '0', '0' | '0', '1' | '0', '2' | '0', '3' |
    +-----+----------+----------+----------+----------+
    | '1' | '0', '1' | '0', '2' | '0', '3' | '1', '0' |
    +-----+----------+----------+----------+----------+
    | '2' | '0', '2' | '0', '3' | '1', '0' | '1', '1' |
    +-----+----------+----------+----------+----------+
    | '3' | '0', '3' | '1', '0' | '1', '1' | '1', '2' |
    +-----+----------+----------+----------+----------+
    
So if ``c1`` and ``c2`` are two digits repreeneted in a particular base, and ``cr = m_subtraction[c1][c2]``,
then ``cr.value`` is the digit representing the difference (modulo the base) of the values ``c1`` and ``c2`` 
represent, and ``cr.carry`` is the digit for the carry of the subtraction.


.. _mul_table_ref:

The Multiplication Table
------------------------

The elements of the multiplication tables are pairs of carry and multiplication 
results indexed by digits. 

.. code-block:: c++

    op_table m_multiplication;

    for (uchar i = 0; i < iBase; i++) {
        op_table_row tr_mul;
        for (uchar j = 0; j < iBase; j++) {
            tr_mul[dig_list[j]] = {dig_list[(i*j)/m_iBase], dig_list[(i*j)%m_iBase]};
        }
        m_multiplication[dig_list[i]] = tr_mul;
    }

So if ``c1`` and ``c2`` are two digits repreeneted in a particular base, and ``cr = m_multiplications[c1][c2]``,
then ``cr.value`` is the digit representing the product (modulo the base) of the values ``c1`` and ``c2``
represent, and ``cr.carry`` is the digit for the carry of the multiplication.


.. _sqrt_table_ref:

The Suare Root Table
--------------------

For the square root look-up we use a simple map associating a string consisting of 2 digits represnting the operand
with a digit representing the integer square root of the operand.

Two digits are needed to represent numbers from 0 to iBase*iBase-1.
