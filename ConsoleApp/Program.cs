using ClassLibrary1;
using System;
using System.IO;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
//using System.Runtime.Serialization.Formatters.Soap;

namespace ConsoleApp
{
    class Program
    {
        public static void MainNew()
        {
            Program test = new Program();
            test.SerializeOriginal("SoapOriginal.xml");
            test.SerializeOverride("SoapOverrides.xml");
            test.DeserializeOriginal("SoapOriginal.xml");
            test.DeserializeOverride("SoapOverrides.xml");

        }
        public void SerializeOriginal(string filename)
        {
            // Creates an instance of the XmlSerializer class.
            XmlTypeMapping myMapping = (new SoapReflectionImporter().ImportTypeMapping(typeof(Group)));
            XmlSerializer mySerializer = new XmlSerializer(myMapping);

            // Writing the file requires a TextWriter.
            TextWriter writer = new StreamWriter(filename);

            // Creates an instance of the class that will be serialized.
            Group myGroup = new Group();

            // Sets the object properties.
            myGroup.GroupName = ".NET";

            Byte[] hexByte = new Byte[2] { Convert.ToByte(100), Convert.ToByte(50) };
            myGroup.GroupNumber = hexByte;

            DateTime myDate = new DateTime(2002, 5, 2);
            myGroup.Today = myDate;

            myGroup.PositiveInt = "10000";
            myGroup.IgnoreThis = true;
            myGroup.Grouptype = GroupType.small;
            Car thisCar = (Car)myGroup.myCar("1234566");

            // Prints the license number just to prove the car was created.
            Console.WriteLine("License#: " + thisCar.licenseNumber + "\n");

            // Serializes the class and closes the TextWriter.
            mySerializer.Serialize(writer, myGroup);
            writer.Close();
        }

        public void SerializeOverride(string filename)
        {
            // Creates an instance of the XmlSerializer class
            // that overrides the serialization.
            XmlSerializer overRideSerializer = CreateOverrideSerializer();

            // Writing the file requires a TextWriter.
            TextWriter writer = new StreamWriter(filename);

            // Creates an instance of the class that will be serialized.
            Group myGroup = new Group();

            // Sets the object properties.
            myGroup.GroupName = ".NET";

            Byte[] hexByte = new Byte[2] { Convert.ToByte(100), Convert.ToByte(50) };
            myGroup.GroupNumber = hexByte;

            DateTime myDate = new DateTime(2002, 5, 2);
            myGroup.Today = myDate;

            myGroup.PositiveInt = "10000";
            myGroup.IgnoreThis = true;
            myGroup.Grouptype = GroupType.small;
            Car thisCar = (Car)myGroup.myCar("1234566");

            // Serializes the class and closes the TextWriter.
            overRideSerializer.Serialize(writer, myGroup);
            writer.Close();
        }

        public void DeserializeOriginal(string filename)
        {
            // Creates an instance of the XmlSerializer class.
            XmlTypeMapping myMapping = (new SoapReflectionImporter().ImportTypeMapping(typeof(Group)));
            XmlSerializer mySerializer = new XmlSerializer(myMapping);

            TextReader reader = new StreamReader(filename);

            // Deserializes and casts the object.
            Group myGroup;
            myGroup = (Group)mySerializer.Deserialize(reader);

            Console.WriteLine(myGroup.GroupName);
            Console.WriteLine(myGroup.GroupNumber[0]);
            Console.WriteLine(myGroup.GroupNumber[1]);
            Console.WriteLine(myGroup.Today);
            Console.WriteLine(myGroup.PositiveInt);
            Console.WriteLine(myGroup.IgnoreThis);
            Console.WriteLine();
        }

        public void DeserializeOverride(string filename)
        {
            // Creates an instance of the XmlSerializer class.
            XmlSerializer overRideSerializer = CreateOverrideSerializer();
            // Reading the file requires a TextReader.
            TextReader reader = new StreamReader(filename);

            // Deserializes and casts the object.
            Group myGroup;
            myGroup = (Group)overRideSerializer.Deserialize(reader);

            Console.WriteLine(myGroup.GroupName);
            Console.WriteLine(myGroup.GroupNumber[0]);
            Console.WriteLine(myGroup.GroupNumber[1]);
            Console.WriteLine(myGroup.Today);
            Console.WriteLine(myGroup.PositiveInt);
            Console.WriteLine(myGroup.IgnoreThis);
        }

        private XmlSerializer CreateOverrideSerializer()
        {
            SoapAttributeOverrides mySoapAttributeOverrides = new SoapAttributeOverrides();
            SoapAttributes soapAtts = new SoapAttributes();

            SoapElementAttribute mySoapElement = new SoapElementAttribute();
            mySoapElement.ElementName = "xxxx";
            soapAtts.SoapElement = mySoapElement;
            mySoapAttributeOverrides.Add(typeof(Group), "PositiveInt", soapAtts);

            // Overrides the IgnoreThis property.
            SoapIgnoreAttribute myIgnore = new SoapIgnoreAttribute();
            soapAtts = new SoapAttributes();
            soapAtts.SoapIgnore = false;
            mySoapAttributeOverrides.Add(typeof(Group), "IgnoreThis", soapAtts);

            // Overrides the GroupType enumeration.
            soapAtts = new SoapAttributes();
            SoapEnumAttribute xSoapEnum = new SoapEnumAttribute();
            xSoapEnum.Name = "Over1000";
            soapAtts.SoapEnum = xSoapEnum;

            // Adds the SoapAttributes to the
            // mySoapAttributeOverrides.
            mySoapAttributeOverrides.Add(typeof(GroupType), "large", soapAtts);

            // Creates a second enumeration and adds it.
            soapAtts = new SoapAttributes();
            xSoapEnum = new SoapEnumAttribute();
            xSoapEnum.Name = "ZeroTo1000";
            soapAtts.SoapEnum = xSoapEnum;
            mySoapAttributeOverrides.Add(typeof(GroupType), "small", soapAtts);

            // Overrides the Group type.
            soapAtts = new SoapAttributes();
            SoapTypeAttribute soapType = new SoapTypeAttribute();
            soapType.TypeName = "Team";
            soapAtts.SoapType = soapType;
            mySoapAttributeOverrides.Add(typeof(Group), soapAtts);

            // Creates an XmlTypeMapping that is used to create an instance
            // of the XmlSerializer class. Then returns the XmlSerializer.
            XmlTypeMapping myMapping = (new SoapReflectionImporter(mySoapAttributeOverrides)).ImportTypeMapping(typeof(Group));

            XmlSerializer ser = new XmlSerializer(myMapping);
            return ser;
        }

        static void Main(string[] args)
        {
            string input = "sample.ExamCard";
            if (args.Length > 0)
                input = args[0];

            Console.WriteLine("Hello World!");

            {
                var myObject = new Group();

                // Stream s = new MemoryStream();
                Stream s = new FileStream("myFileName.xml", FileMode.Create);
                SoapFormatter prpSoap = new SoapFormatter();

                XmlWriter writer = new XmlTextWriter(s, Encoding.UTF8);

                // Serializes a class named Group as a SOAP message.  
                SoapReflectionImporter importer = new SoapReflectionImporter();
                XmlTypeMapping myTypeMapping = importer.ImportTypeMapping(typeof(Group), "mathieu");
                XmlSerializer mySerializer = new XmlSerializer(myTypeMapping);
                // To write to a file, create a StreamWriter object.  
                // StreamWriter myWriter = new StreamWriter("myFileName.xml");
                // mySerializer.Serialize(myWriter, myObject);
                // myWriter.Close();

                writer.WriteStartElement("root");
                mySerializer.Serialize(writer, myObject);
            }

            {
                // Construct an instance of the XmlSerializer with the type
                // of object that is being deserialized.
                var mySerializer = new XmlSerializer(typeof(Group));
                // To read the file, create a FileStream.
                using var myFileStream = new FileStream(input, FileMode.Open);
                // Call the Deserialize method and cast to the object type.
                var myObject = (Group)mySerializer.Deserialize(myFileStream);
            }
        }
    }
}
