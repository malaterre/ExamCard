using System;
using System.Xml.Serialization;

// https://learn.microsoft.com/en-us/dotnet/standard/serialization/how-to-override-encoded-soap-xml-serialization
namespace ClassLibrary1
{
    public class Group
    {
        [SoapAttribute(Namespace = "http://www.cpandl.com")]
        public string GroupName;

        [SoapAttribute(DataType = "base64Binary")]
        public Byte[] GroupNumber;

        [SoapAttribute(DataType = "date", AttributeName = "CreationDate")]
        public DateTime Today;
        [SoapElement(DataType = "nonNegativeInteger", ElementName = "PosInt")]
        public string PositiveInt;
        // This is ignored when serialized unless it is overridden.
        [SoapIgnore]
        public bool IgnoreThis;

        public GroupType Grouptype;

        [SoapInclude(typeof(Car))]
        public Vehicle myCar(string licNumber)
        {
            Vehicle v;
            if (licNumber == "")
            {
                v = new Car();
                v.licenseNumber = "!!!!!!";
            }
            else
            {
                v = new Car();
                v.licenseNumber = licNumber;
            }
            return v;
        }
    }

    public abstract class Vehicle
    {
        public string licenseNumber;
        public DateTime makeDate;
    }

    public class Car : Vehicle
    {
    }

    public enum GroupType
    {
        // These enums can be overridden.
        small,
        large
    }
}
