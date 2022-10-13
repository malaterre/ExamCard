using Philips.PmsMR.Platform.Aswglo;
using Philips.PmsMR.Platform.ECPlatform;
using Philips.PmsMR.Platform.ScannerContext;
using System;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.Serialization;

namespace Philips.PmsMR.ExamCards.ECModel
{
    [System.Serializable]
    //[System.Runtime.Remoting.Metadata.SoapType(XmlNamespace = "http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.ExamCards.ECModel/philips.pmsmr.examcards.ecmodel_cs")]
    //[System.Xml.Serialization.XmlRoot(Namespace = "foobar", IsNullable = false)]
    public class ExamCard  : ISerializable 
    {
        public string name;
        public ECInstrumentList instrumentList;
        public ExamCardData data;
        public HtmlItemDescription description;
        public PatientPosition patientOrientation;
        public string anatomicRegion;
        public string laterality;
        public string[] namedGeometries;
        public int heartRate;
        public bool autoFillInHeartRate;
        public MainSysType mainSystemType;
        public ExamCardConfig configuration;
        public ExamCardsVersion version;
        public int alignOverlap;
        public bool pushToWorkStation;
        // new version:
        /*public object anatomy;
        public object reviewRequired;
        public object paediatric;
        public object patientWeight;
        public object selectedCoils;*/

        
        // versioning:
        private int memberCount;

        /*public ExamCard()
        {
        }*/

        protected ExamCard(SerializationInfo info, StreamingContext context)
        {
            memberCount = info.MemberCount;
            Debug.Assert(memberCount == 15);

            foreach (SerializationEntry entry in info)
            {
                string name = entry.Name;
                object value = entry.Value;
                Type type = this.GetType();
                PropertyInfo[] dbg1 = type.GetProperties();
                MemberInfo[] dbg2 = type.GetMembers();
                PropertyInfo propInfo = this.GetType().GetProperty(name);
                MemberInfo[] propInfo2 = this.GetType().GetMember(name);
                Debug.Assert(propInfo2.Length == 1);
                MemberInfo memberInfo = propInfo2[0];
                Debug.Assert(memberInfo.MemberType == MemberTypes.Field);
                FieldInfo fieldInfo = (FieldInfo)memberInfo;
                Type fieldType = fieldInfo.FieldType;
                switch (fieldInfo.FieldType.Name)
                {
                    case "Int32":
                        ((FieldInfo)memberInfo).SetValue(this, Int32.Parse((string)value));
                        break;
                    case "Boolean":
                        ((FieldInfo)memberInfo).SetValue(this, Boolean.Parse((string)value));
                        break;
                    default:
                        Debug.Assert(fieldInfo.FieldType.Equals(entry.ObjectType));
                        ((FieldInfo)memberInfo).SetValue(this, value);
                        break;
                }
            }

        }
        void ISerializable.GetObjectData(SerializationInfo info, StreamingContext context)
        {
            var tmp = info.AssemblyName.Split(',');
            //info.AssemblyName = tmp[0]; // "philips.pmsmr.examcards.ecmodel_cs"; // to get rid of Version
            Type type = this.GetType();
            {
                MemberInfo[] members = type.GetMembers();
                foreach (var member in members)
                {
                    if (member.MemberType == MemberTypes.Field)
                    {
                        FieldInfo field = ((FieldInfo)member);
                        string name = member.Name;
                        //if (fieldNames.Contains(name))
                        {
                            Debug.Assert(!name.Equals("memberCount"));
                            object value = field.GetValue(this);
                            info.AddValue(name, value);
                        }
                    }
                }
            }

        }
    }
    [Serializable]
    public class ECInstrumentList
    {
        public object list;
    }
    [Serializable]
    public class ExamCardConfig
    {
        public object allowedMainSystemTypes;
        public object allowedGradientChains;
        public object swOptions;
        public object hwOptions;
        public object coilOptions;
    }
    [Serializable]
    public class ExamCardsVersion
    {
        public string swVersion;
        public string modelVersion;
        public string methodsVersion;
    }
    [Serializable]
    public class ECInstrument
    {
        // old version:
        public object scanStep;
        // new version:
        /*public object scanSet;*/
    }
    [Serializable]
    public class ScanStep
    {
        public string name;
        public object currentScanSet;

    }

}
