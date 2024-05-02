
The ``LongNum`` class
=====================

In order to handle numbers with an arbitrary number of digits and supporting many numeric bases,
it was decided to implement such data with strings. The decimal point is not included in the string representation, 
but stored as an index into the char array.

The state of a ``LongNum`` is given by 5 items

.. code-block:: c++

    std::string            m_sDigits;      // the digits of the number
    uint                   m_iPostDigits;  // number of digits after decimal point
    char                   m_iSign;        // the sign of the numbre (-1/+1)

    uchar                  m_iBase;
    DigitOperationTables  *m_pDOT;

where

``m_sDigits``
    The sequence of digits in the currently used base. The least signifcant digit (LSD) is at index 0,  the most significant digit (MSD) is at the end of the string.
    For bases larger than 10 lowercase letters of the alphabet are used.

``m_iPostDigits``
    The current number of digits after the decimal point, or, equivalently, the index of the element representing the "ones".

``m_iSign``
    The siogn of the number (-1 or +1)

``m_iBase``
    The currently used numeric base. This determines also what symbols are used to represent numbers.

``m_pDOT``
    A pointer to the :ref:`digit operation tables<operation_tables_ref>`.


Examples
~~~~~~~~

Some numbers and their representations

+------------+------+--------------+----+--------------+
|  number    | base | digit string | PD | decimal      |
+------------+------+--------------+----+--------------+
|    1       |  10  | "1"          | 0  |    1         |
+------------+------+--------------+----+--------------+
|   10       |  10  | "01"         | 0  |   10         |
+------------+------+--------------+----+--------------+
|    0.1     |  10  | "10"         | 1  |    0.1       |
+------------+------+--------------+----+--------------+
|    0.01    |  10  | "100"        | 2  |    0.01      |
+------------+------+--------------+----+--------------+
| 4122.77    |   8  | "772214"     | 2  | 2130.984375  |
+------------+------+--------------+----+--------------+
|    0.0003  |   5  | "30000"      | 4  |    0.48      |
+------------+------+--------------+----+--------------+
|  1fb.0c327 |  16  | "723c0bf1"   | 5  |  507.0476446 |
+------------+------+--------------+----+--------------+

Note that this representation is not unique:
``("000100", 3)`` is equivalent to ``("1", 0)``

However, there is a unique canonical representative for every number:
The normalized form has no superfluous '0' digits at the front or the back of its digits.
I.e. neither the MSD nor the LSD are the '0' character.

