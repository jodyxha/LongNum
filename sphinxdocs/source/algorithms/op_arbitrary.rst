.. _op_arbitrary_operations:

Basic Operations for Arbitrary Numbers
======================================

Our basic algorithms for addition, subtraction, multiplication and division can only handle positive numbers.
To get these operatons for negative numbers as well, there are some wrapper functions.

``add``
-------

We rely on simple arithmetical equalities, such as ``x + y = -((-x) + -y)``, so we can express addition involving negative numbers in terms of 
addition and subtraction of positive numbers.

.. code-block:: c++

    LongNum LongNum::add(LongNum lN)  {
        LongNum ns(lN.getBase());

        if (isNegative() && lN.isNegative()) {
            //    x + y = -((-x) + -y))
            ns = -addPositives(-*this, -lN);
        } else if  (!isNegative() && !lN.isNegative()) {
            //    x + y = x + y
            ns = addPositives(*this, lN);
        } else if  (!isNegative() && lN.isNegative()) {
            //    x + y = x - (-y)
            ns = subPositives(*this, -lN);
        } else if  (isNegative() && !lN.isNegative()) {
            //    x + y = y - (-x)
            ns = subPositives(lN, -*this);
        }
        return LongNum(ns);
   }


``sub``
-------

Again, we rely on simple arithmetical equalities, such as ``x - y = (-y) - (-x)``, so we can express subtraction involving negative numbers in terms of 
addition and subtraction of positive numbers.

.. code-block:: c++

    LongNum LongNum::sub(LongNum lN)  {
        LongNum ns(lN.getBase());
        
        if (isNegative() && lN.isNegative()) {
            //   x - y = (-y) - (-x)
            ns = subPositives(-lN, -*this);
        } else if  (!isNegative() && !lN.isNegative()) {
            //   x - y = x - y
            ns = subPositives(*this, lN);
        } else if  (!isNegative() && lN.isNegative()) {
            //   x - y =  x + (-y)
            ns = addPositives(*this, -lN);
        } else if  (isNegative() && !lN.isNegative()) {
            //   x - y = (-x) + y
            ns = -addPositives(-*this, lN);
        }
        
        return LongNum(ns);
    }


``mul``
-------

Here too, we rely on simple arithmetical equalities, such as ``x * y = -((-x) * y)``, so we can express multiplication involving negative numbers in terms of 
multiplication of positive numbers.

.. code-block:: c++

    LongNum LongNum::mul(LongNum &lN) {
        LongNum ns(lN.getBase());
        
        if (isNegative() && lN.isNegative()) {
            //   x * y = (-x) * (-y)
            ns = mulPositives(-lN, -*this);
        } else if  (!isNegative() && !lN.isNegative()) {
            //   x * y = x * y
            ns = mulPositives(*this, lN);
        } else if  (!isNegative() && lN.isNegative()) {
            //   x * y = -(x * (-y))
            ns = -mulPositives(*this, -lN);
        } else if  (isNegative() && !lN.isNegative()) {
            //   x * y = -((-x) * y)
            ns = -mulPositives(-*this, lN);
        }
        
        return LongNum(ns);
    }


``div``
-------

As above too, we rely on simple arithmetical equalities, such as ``x / y = (-x) / (-y)``, so we can express division involving negative numbers in terms of 
division of positive numbers.

.. code-block:: c++

    LongNum LongNum::div(LongNum &lN, uint iPrecision) {
        LongNum ns(lN.getBase());
        // exclude 0-divisor
        if (lN == LongNum("0", lN.getBase())) {
            throw(std::string("Errur: division by 0\n"));
        } else {
            if (isNegative() && lN.isNegative()) {
                //   x / y = (-x) / (-y)
                ns = divPositives(-lN, -*this, iPrecision);
            } else if  (!isNegative() && !lN.isNegative()) {
                //   x / y = x / y
                ns = divPositives(*this, lN, iPrecision);
            } else if  (!isNegative() && lN.isNegative()) {
                //   x / y = -(x / (-y))
                ns = -divPositives(*this, -lN, iPrecision);
            } else if  (isNegative() && !lN.isNegative()) {
                //   x / y = -((-x) / y)
                ns = -divPositives(-*this, lN, iPrecision);
            }
        }
        return LongNum(ns);
    }

