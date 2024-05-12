.. _square_root_example_ref:

.. |leq| unicode:: U+2264 .. less or equal

Square Root Example
===================

.. rst-class:: hidden

   This page describes the :index:`Square Root Example` algorithm


We start with the number 

.. figure:: ../../images/sqrt_init_00.png

and prepare it like this

.. figure:: ../../images/sqrt_init_01.png

Now get the first pair :math:`P`

.. figure:: ../../images/sqrt_it_0_loop_1.png

and find the largest number :math:`s` whose square is less or egual to :math:`P`:

.. figure:: ../../images/sqrt_it_0_loop_2.png

This number is the first digit of our result.

Calculate the remainder:

.. figure:: ../../images/sqrt_it_0_loop_3.png

Get the next pair and append it to the remainder

.. figure:: ../../images/sqrt_it_1_loop_1.png

Now find the largest number :math:`x` such that :math:`x * (20*2 + x)` is less than or equal to the remainder :math:`r=143`, where :math:`s` consists of the current solution's digits.  
This number is the next digit of the solution.
In this case we have 3*(40+3) = 129 < 143.

.. figure:: ../../images/sqrt_it_1_loop_2.png

Get the remainder

.. figure:: ../../images/sqrt_it_1_loop_3.png

We repeat the loop again: append the next pair to the remainder

.. figure:: ../../images/sqrt_it_2_loop_1.png

find the largest number :math:`x` satisfying :math:`x*(20*23s + x) \leq 1428`

.. figure:: ../../images/sqrt_it_2_loop_2.png

append this number to the solution digits and calculate the remainder

.. figure:: ../../images/sqrt_it_2_loop_3.png

The next iteration again begins by expanding the remainder with the next pair of input digits,
but because it is the first pair after the input's decinal point we must add the decimal point to the solution.

.. figure:: ../../images/sqrt_it_3_loop_1.png

Determine the next solution digit. Here the value :math:`20*233 = 4660` is greater than :math:3917:, so the nest digit is 0. 

.. figure:: ../../images/sqrt_it_3_loop_2.png

And find the remainder

.. figure:: ../../images/sqrt_it_3_loop_3.png

For the fifth solution digit we again append the next pair to the remainder

.. figure:: ../../images/sqrt_it_4_loop_1.png

Calculate the actual solution digit

.. figure:: ../../images/sqrt_it_4_loop_2.png

and write the remainder

.. figure:: ../../images/sqrt_it_4_loop_3.png

Another iteration

.. figure:: ../../images/sqrt_it_5_loop_1.png

.. figure:: ../../images/sqrt_it_5_loop_2.png

.. figure:: ../../images/sqrt_it_5_loop_3.png


And another

.. figure:: ../../images/sqrt_it_6_loop_1.png

.. figure:: ../../images/sqrt_it_6_loop_2.png

.. figure:: ../../images/sqrt_it_6_loop_3.png

And a last iteration


.. figure:: ../../images/sqrt_it_7_loop_1.png

.. figure:: ../../images/sqrt_it_7_loop_2.png

.. figure:: ../../images/sqrt_it_7_loop_3.png

So we have

.. math::

     \Huge {\sqrt{54328.179} \approx 233.08405}

Of course this can be continued for an arbitrary number of digits:

.. math::

    \sqrt{54328.179} \approx 233.084059944046795814499796642841476428333458527718069590522040979...