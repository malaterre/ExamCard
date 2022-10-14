using Philips.PmsMR.Acquisition.AcqGlo;
using Philips.PmsMR.Platform.Aswglo;
using Philips.PmsMR.Platform.ECPlatform;
using System;
using System.Collections;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.Serialization;

namespace Philips.PmsMR.Acquisition.ScanSet
{
    [Serializable]
    public class ScanSet
    {
        public ExecutionStepList executionSteps;
    }

    [Serializable]
    public class     ExecutionStepList
    {
        public ArrayList list;
    }
    [Serializable]
    public class ExecutionStep
    {
        public SingleScan singleScan ;
        public ContrastInjectionProtocol contrastInjectionProtocol;
        public bool contrast;
        public bool executionStepContrastUsed;
        public DynamicStepProperties dynamicStep;

    }

    [Serializable]
    public class SingleScan : ISerializable
    {
        public string name;
        public SmartPlanType smartGeoType;
        public ArrayList vertebraRanges;
        public ScanProcedure scanProcedure;
        public int stationNumber;
        public HtmlItemDescription detail;
        public string geometryName;
        public bool reuseStackSizes;
        public object viewingProcedure;
        public bool pushToWorkStation;
        public int geoLinkID;
        public ScanProperties scanProperties;
        public bool useViewingProcedure;
        protected SingleScan(SerializationInfo info, StreamingContext context)
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
    public class ContrastInjectionProtocol
    {
        public string agent;
        public string routeOfAdministration;
        public int volume;
        public double concentration;
        public int injectionStartDynamicNumber;
    }
}
