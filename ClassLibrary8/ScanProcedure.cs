using Philips.PmsMR.Platform.Aswglo;
using System;
using System.Collections;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.Serialization;

namespace Philips.PmsMR.Acquisition.AcqGlo
{
    public enum TypeOfApplicationMode
    {
        AGDEF_APPL_IMAGING,
        
    }
    [Serializable]
    public class ScanProcedure
    {
        public byte[] parameterData;
        
        public string methodDescription;
        /*
        private string[] methodDescriptions;
        [System.Xml.Serialization.XmlElement("methodDescription")]
        public string MethodDescription
        {
            get {
                methodDescriptions = new string[4];
                methodDescriptions[0] = "foo";
                // $"{s[..2]}-{s[2..10]}-{s[^1..]}"
                return String.Format("{0} {1} {2} {3}", methodDescriptions);
            }
            set {
                var tmp = value.Split(' ');
                throw new NotImplementedException();
            }
        }*/
        public string name;
        public TypeOfApplicationMode applicationMode;
        public string[] linkToOriginalProtocol;
        public string lastModifiedTimeUtc;
        public bool linkState;
        public string NameChanged;
        public string Modified;
        public string Disposing;
    }
    [Serializable]
    public partial class ScanProperties : ISerializable
    {
        public bool pause;
        public bool scanAlign;
        public int scanAlignOverlap;
        //[System.Runtime.Remoting.Metadata.SoapType(XmlNamespace = "http://schemas.microsoft.com/clr/nsassem/Philips.PmsMR.ExamCards.ECModel/philips.pmsmr.examcards.ecmodel_cs")]
        public SmartPlanType smartScoutType;
        public bool madeByPhilips;
        public bool suspendableDynamic;
        public ScanProcedureConfig requiredConfiguration;

        protected ScanProperties(SerializationInfo info, StreamingContext context)
        {
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
                        //Debug.Assert(fieldInfo.FieldType.Equals(entry.ObjectType));
                        ((FieldInfo)memberInfo).SetValue(this, value);
                        break;
                }
            }

        }
        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
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
                            object value = field.GetValue(this);
                            info.AddValue(name, value);
                        }
                    }
                }
            }

        }
    }
    [Serializable]
    public class ScanProcedureConfig
    {
        public BitArray allowedMainSystemTypes ;
        public BitArray allowedGradientChains;
        public BitArray swOptions;
        public BitArray hwOptions;
        public BitArray coilOptions;
    }
    [Serializable]
    public class DynamicStepProperties : ISerializable
    {
        public DynamicScanDelayMode delayMode;
        public int delayDuration;
        public int dynamicCount;
        public int firstDynamic;

        protected DynamicStepProperties(SerializationInfo info, StreamingContext context)
        {
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
                        //Debug.Assert(fieldInfo.FieldType.Equals(entry.ObjectType));
                        ((FieldInfo)memberInfo).SetValue(this, value);
                        break;
                }
            }

        }
        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
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
                            object value = field.GetValue(this);
                            info.AddValue(name, value);
                        }
                    }
                }
            }

        }
    }
}
