// TestEmployee.cs
// Driver for exception handling classes

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CS_Part_1
{
    class TestEmployee
    {
        static void Main(string[] args)
        {
            ProductionWorker scott = new ProductionWorker(
                "Scott Pilgrim", 1000, 'Q', "4/23/1982", 2, -7.00);
            ProductionWorker gman = new ProductionWorker();
            gman.setName("Gideon Graves");
            gman.setNum(-600, 'G');
            gman.setHireDate("12/25/81");
            gman.setShift(12);
            gman.setRate(20.00);

            Console.WriteLine("Name: " + scott.getName());
            Console.WriteLine("Employee Number: " + scott.getNum());
            Console.WriteLine("Hire Date: " + scott.getHireDate());
            Console.WriteLine("Shift: " + scott.getShift());
            Console.WriteLine("Pay Rate: " + scott.getRate().ToString("0.00"));

            Console.WriteLine("\nName: " + gman.getName());
            Console.WriteLine("Employee Number: " + gman.getNum());
            Console.WriteLine("Hire Date: " + gman.getHireDate());
            Console.WriteLine("Shift: " + gman.getShift());
            Console.WriteLine("Pay Rate: " + gman.getRate().ToString("0.00"));
        }
    }
}
