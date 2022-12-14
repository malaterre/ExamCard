using Philips.PmsMR.Acquisition.AcqGlo;
using Philips.PmsMR.Acquisition.ScanSet;
using Philips.PmsMR.ExamCards.ECModel;
using Philips.PmsMR.Platform.Aswglo;
using Philips.PmsMR.Platform.ECPlatform;
using Philips.PmsMR.Platform.ScannerContext;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters;
using System.Runtime.Serialization.Formatters.Soap;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace ConsoleApp
{
    public enum E { V }
    [Serializable]
    public class Person /* : System.Runtime.Serialization.ISerializable */
    {
        public E my_enum; // field
        public bool my_bool;
        [NonSerialized]
        private long propCode;
        //[XmlInclude]
        //[SerializableAttribute]
        public long PropertyCode // property
        {
            get { return propCode; }
            set { propCode = value; }
        }
        /*
        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("my_enum", my_enum);
            info.AddValue("my_bool", my_bool);
        }*/
    }
    sealed public class ProgramBinder : SerializationBinder
    {
        override public void BindToName(Type serializedType, out string assemblyName, out string typeName)
        {
            throw new NotImplementedException();
        }
        public override Type BindToType(string assemblyName, string typeName)
        {
            Type typeToDeserialize = Type.GetType(String.Format("{0}, {1}",
                typeName, assemblyName));

            Console.WriteLine(String.Format("Input is {0}, {1}", typeName, assemblyName));
            Console.WriteLine(String.Format("Output is {0}", typeToDeserialize.AssemblyQualifiedName));

            return typeToDeserialize;
        }
    }
    class Program
    {
        public static void Main()
        {
            /*{
                ExamCard myGroup = new ExamCard();
                myGroup.name = "SR_ADULT_007";
                myGroup.patientOrientation = new PatientPosition();
                myGroup.mainSystemType = new MainSysType();

                ExamCardData e = new ExamCardData();
                HtmlItemDescription h = new HtmlItemDescription();
                RelationshipManagerData r = new RelationshipManagerData();
                SingleScan s = new SingleScan();
                ScanProcedure ss = new ScanProcedure();
                //DynamicScanDelayMode d = new DynamicScanDelayMode();

                FileStream stmCar = new FileStream("wip.ExamCard", FileMode.Create);
                SoapFormatter sopCar = new SoapFormatter();

                sopCar.Serialize(stmCar, myGroup);
            }*/
            {
                Person p = new Person();
                using (FileStream fs = new FileStream("myfile.soap", FileMode.Create))
                {
                    SoapFormatter f = new SoapFormatter();
                    //f.AssemblyFormat = FormatterAssemblyStyle.Simple;
                    //f.TypeFormat = FormatterTypeStyle.TypesAlways;
                    //f.TypeFormat = FormatterTypeStyle.TypesWhenNeeded;
                    f.FilterLevel = TypeFilterLevel.Full;
                    f.FilterLevel = TypeFilterLevel.Low;
                    f.Serialize(fs, p);
                    fs.Close();
                }
            }
            /*
            {
                ExamCard examCard = new ExamCard();
                examCard.name = "SR_ADULT_007";
                examCard.patientOrientation = new PatientPosition();
                examCard.mainSystemType = new MainSysType();

                FileStream fileStream = new FileStream("junk.ExamCard", FileMode.Create);
                SoapFormatter soapFormatter = new SoapFormatter();
                soapFormatter.Serialize(fileStream, examCard);
            }*/
            {
                Hashtable t = new Hashtable();
                OutputDescriptorData k = new OutputDescriptorData();
                SingleScan s = new SingleScan();
                ArrayList l = new ArrayList();
                l.Add(s);
                t.Add(k, l);
            }
            {
                //string filename = "data/PHANTOM_QT1_SLU_20151230.ExamCard";
                string filename = "data/SR_ADULT_007.ExamCard";
                //string filename = "data/SR_ADULT_018.ExamCard";
                //string filename = "data/TADPOLE_CHILD_V3.ExamCard";
                //string filename = "data/spine_generic_philips_R53.ExamCard";
                FileStream stmCar = new FileStream(filename, FileMode.Open);
                SoapFormatter sopCar = new SoapFormatter();
                //sopCar.Binder = new ProgramBinder();
                ExamCard examCard = (ExamCard)sopCar.Deserialize(stmCar);

                if(true)
                {
                    var options = new JsonSerializerOptions { WriteIndented = true, IncludeFields = true };
                    string jsonString = JsonSerializer.Serialize(examCard, options);
                    File.WriteAllText("roundtrip" + ".json", jsonString);
                }


                FileStream fileStream = new FileStream("roundtrip.ExamCard", FileMode.Create);
                SoapFormatter soapFormatter = new SoapFormatter();
                soapFormatter.AssemblyFormat = FormatterAssemblyStyle.Simple; // Get rid of version and al.
                soapFormatter.TypeFormat = FormatterTypeStyle.TypesWhenNeeded; // full resolution for member enum
                //soapFormatter.Binder = new ProgramBinder();
                soapFormatter.Serialize(fileStream, examCard);
            }
        }
    }
}

