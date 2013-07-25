using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CS_Part_1
{
    class ProductionWorker : Employee
    {
        private int shift;
        private double rate;

        public ProductionWorker(String name, int a, char b,     // constructor
                String hd, int sft, double rt)
        {
            setName(name);
            setNum(a, b);
            setHireDate(hd);
            setShift(sft);
            setRate(rt);
        }
        public ProductionWorker()   // default constructor
        {
            setName("");
            setNum(0, ' ');
            setHireDate("");
            setShift(1);
            setRate(0);
        }

        public int getShift()
        {
            return shift;
        }
        public double getRate()
        {
            return rate;
        }
        public void setShift(int sft)
        {
            try
            {
                shift = sft;
                if (shift != 1 && shift != 2)
                    throw new InvalidShift();
            }
            catch (InvalidShift)
            {
                shift = 1;
            }
        }
        public void setRate(double rt)
        {
            try
            {
                rate = rt;
                if (rate < 0)
                    throw new InvalidPayrate();
            }
            catch (InvalidPayrate)
            {
                rate = 0;
            }
        }
    }           // end class ProductionWorker

    class InvalidShift : Exception
    {
	    public InvalidShift()
	    {
		    Console.WriteLine("Invalid Shift");
	    }
    }
    class InvalidPayrate : Exception
    {
	    public InvalidPayrate()
	    {
            Console.WriteLine("Invalid Payrate");
	    }
    }
}
