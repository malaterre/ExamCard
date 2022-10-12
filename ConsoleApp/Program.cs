using ClassLibrary1;
using System;
using System.IO;
using System.Text;
using System.Xml;
using System.Xml.Serialization;

namespace ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            string input = "sample.ExamCard";
            if (args.Length > 0)
                input = args[0];

            Console.WriteLine("Hello World!");

            {
                var myObject = new Class1();

                // Stream s = new MemoryStream();
                Stream s = new FileStream("myFileName.xml", FileMode.Create);
                XmlWriter writer = new XmlTextWriter(s, Encoding.UTF8);

                // Serializes a class named Group as a SOAP message.  
                SoapReflectionImporter importer = new SoapReflectionImporter();
                XmlTypeMapping myTypeMapping = importer.ImportTypeMapping(typeof(Class1), "mathieu");
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
                var mySerializer = new XmlSerializer(typeof(Class1));
                // To read the file, create a FileStream.
                using var myFileStream = new FileStream(input, FileMode.Open);
                // Call the Deserialize method and cast to the object type.
                var myObject = (Class1)mySerializer.Deserialize(myFileStream);
            }
        }
    }
}
