.. _square_root_ref:

.. |leq| unicode:: U+2264 .. less or equal

Square Root
===========

.. rst-class:: hidden

   This page describes the :index:`Square Root` algorithm

Algorithm Details
-----------------


**Initialisation phase**

1. | The first step is to make digits group starting at the decimal point and moving towards the most significant digit, and from the decimal towards the least significant digit.
   | The highest group can contain one digit (you may prepend a 0 to make it a proper 2-digit group, but this does not matter to the algorithm).
   | Furthermore, we imagine the fractional part of the number to be followed by as many pairs "00" as needed for the desired accuracy of the result.
   | Let :math:`P` be the number represented by the highest group.

For example, the number ``54328.179`` would be grouped and padded like this: ``05|43|28|.17|90|00|00``, and in this case :math:`P = 5`.

2. | Now find the highest one-digit number :math:`s` whose square is less than or equal to  :math:`P`. 
   | :math:`~~~~s = max \{0\leq i < b \; | \; i^2 \leq P\}`
   | The digit :math:`s` is the first digit of} the solution :math:`L`:
   | :math:`~~~~L=s`

3. | Subtract :math:`s^2` from the leading group yielding a number :math:`r`: 
   | :math:`~~~~r = P - s^2`

Here follows a loop which produces an additional digit of the solution in each iteration.

**Loop**

1. | Get the next pair of digits :math:`P` from the argument, and append to :math:`r`: 
   | :math:`~~~~r' = r \circ P`
   | (there will always be a next pair, because we consider the tail of the argument to consist of an infinite sequence of pairs "00")

2. | If the selected digit pair :math:`P` is the first pair **after** the decimal point, add a decimal point to the solution.

3. | Form a number :math:`L` from the current solution digits, double it and then multply it by 10 (i.e. multiply the base :math:`b`):
   | :math:`~~~~h = b \times 2 \times L`
   | (in base 2, we effectivley append '00' to :math:`L`)

4. | Find the highest digit :math:`x`, such that 
   | :math:`~~~~p = x * (h + x) \leq r'`  
   | The digit :math:`x` is the next digit of the solution, so append it to math:`L`: 
   | :math:`~~~~L \longrightarrow L \circ x`

5. | Subtract the product :math:`p` from :math:`r'` to get the new value of math:`r` 
   | :math:`~~~~r = r' - p`

6. | If the remainder :math:`r = 0`, the loop can be exited, because from now on, the algorithm will only produce 0s.

7. | If the number of digits of ther solution is sufficient, exit the loop; otherwise go to step 1


Click :ref:`here<square_root_example_ref>` for a detailed example of the application of this algorithm to a number.