
The Algorithms
==============

The algorithms used in ``LongNum`` correspond to those traditionally 
used in manual arithmetics, such as longhand addition, 
longhand subtraction, longhand multiuplication and longhhaand division.
In addition to that, there is also the implementation of a longhand square root.

(cube root: `longhand cube root <https://wikihow.com/Calculate-Cube-Root-by-Hand)>`_

The actual implementations  handle only positive numbers (including 0).

In order to deal with negative numbers, as well, there are some helper functions 
which make the operands positive but keep track of the sign for the result.


**The Algorithms for Positive Numbers**

.. toctree::
   :maxdepth: 1

   algorithms/addition
   algorithms/subtraction
   algorithms/multiplication
   algorithms/division

.. toctree::
   :maxdepth: 1

   algorithms/square_root
   algorithms/square_root_example

**The Algorithms for Arbitrary Numbers**

.. toctree::
   :maxdepth: 1

   algorithms/op_arbitrary
