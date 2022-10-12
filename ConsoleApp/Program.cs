using Philips.PmsMR.ExamCards.ECModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Soap;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace ConsoleApp
{
    class Program
    {
        public static void Main()
        {
            {
                ExamCard myGroup = new ExamCard();
                FileStream stmCar = new FileStream("wip.ExamCard", FileMode.Create);
                SoapFormatter sopCar = new SoapFormatter();

                sopCar.Serialize(stmCar, myGroup);
            }

            {
                FileStream stmCar = new FileStream("junk/sample.ExamCard",FileMode.Open);
                SoapFormatter sopCar = new SoapFormatter();
                ExamCard vehicle = (ExamCard)sopCar.Deserialize(stmCar);
            }
        }
    }
}
