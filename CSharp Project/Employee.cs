using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CS_Part_1
{
    class Employee
    {
        private string name;
        private int numA;
        private char numB;
        private string hireDate;

        public Employee(String n, int a, char b, String hd)
        {
            name = n;
            numA = a;
            numB = b;
            hireDate = hd;
        }
        public Employee()
        {
            name = "";
            numA = 0;
            numB = ' ';
            hireDate = "";
        }

        public string getName()
        {
            return name;
        }
        public string getNum()
        {
            return String.Format("{0:000}", numA) + '-' + numB;
        }
        public string getHireDate()
        {
            return hireDate;
        }

        public void setName(string n)
        {
            name = n;
        }
        public void setNum(int a, char b)
        {
            try
            {
                numA = a;
                if (numA < 0 || numA > 999)
                    throw new InvalidEmployeeNumber();
            }
            catch(InvalidEmployeeNumber)
            {
                numA = 0;
            }
            numB = b;
        }
        public void setHireDate(string hd)
        {
            hireDate = hd;
        }
    }

    class InvalidEmployeeNumber : Exception
    {
	    public InvalidEmployeeNumber()
	    {
		    Console.WriteLine("Invalid Employee Number");
	    }
    }
}
