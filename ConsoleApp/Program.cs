using Philips.PmsMR.Acquisition.AcqGlo;
using Philips.PmsMR.Acquisition.ScanSet;
using Philips.PmsMR.ExamCards.ECModel;
using Philips.PmsMR.Platform.Aswglo;
using Philips.PmsMR.Platform.ECPlatform;
using Philips.PmsMR.Platform.ScannerContext;
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
                myGroup.name = "SR_ADULT_007";
                myGroup.patientOrientation = new PatientPosition();
                myGroup.mainSystemType = new MainSysType();

                ExamCardData e = new ExamCardData();
                HtmlItemDescription h = new HtmlItemDescription();
                RelationshipManagerData r = new RelationshipManagerData();
                SingleScan s = new SingleScan();
                ScanProcedure ss = new ScanProcedure();
                DynamicScanDelayMode d = new DynamicScanDelayMode();

                FileStream stmCar = new FileStream("wip.ExamCard", FileMode.Create);
                SoapFormatter sopCar = new SoapFormatter();

                sopCar.Serialize(stmCar, myGroup);
            }

            {
                //string filename = "data/PHANTOM_QT1_SLU_20151230.ExamCard";
                //string filename = "data/SR_ADULT_007.ExamCard";
                //string filename = "data/SR_ADULT_018.ExamCard";
                //string filename = "data/TADPOLE_CHILD_V3.ExamCard";
                string filename = "data/spine_generic_philips_R53.ExamCard";
                FileStream stmCar = new FileStream(filename, FileMode.Open);
                SoapFormatter sopCar = new SoapFormatter();
                ExamCard vehicle = (ExamCard)sopCar.Deserialize(stmCar);
            }
        }
    }
}
