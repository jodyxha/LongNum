.. _square_root_ref:

.. |leq| unicode:: U+2264 .. less or equal

Square Root
===========

.. rst-class:: hidden

   This page describes the :index:`Square Root` algorithm

Algorithm Details
-----------------

We accompany the  algorithm description with a numeric example_

.. figure:: ../../images/sqrt_init_00.png

The first step is to make digits group starting at the decimal point and moving towards the most significant digit, and from the decimal towards the least significant digit.

.. figure:: ../../images/sqrt_init_01.png

The highest group can contain one digit (you may prepend a 0 to wmate it a proper 2-digit group, but this does not matter to the algorithm).

Now find the highest one-digit number *s* whose square is lee than the number represented by the leading two digits.
The digit *s* is the first digit of the solution.

Now subtract *s*\ :sup:`2` from the leading group yielding a number *r*.

Here follows a loop which produces an additional digit of the solution in each iteration.

1. | if there still are more digit pairs left from the argument, then append the next on to *r* yielding *r'*.
   | if there are no more digit pairs,, append '00' to *r* yielding *r'*

2. | if the select digit pair is the first **after** the deimal point, add a decimal point to the solution.

3. | form a number *L* from the current digits, double it and then multply it by 10 (i.e. multiply the base *b*), h = 10*2*L

4. | find the highest digit *x*, such that p = x * (h + x) |leq| r'  
   | the digit x is the next digit of the solution, so append it to L: L = Lx

5. | Subtract the product *p* from *r'* to get the new value of *r*

6. | If the number of digits of ther solution is sufficient, exit the loop; otherwise go to step 1

