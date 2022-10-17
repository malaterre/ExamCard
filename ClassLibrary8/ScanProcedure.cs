using Philips.PmsMR.Platform.Aswglo;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Serialization;
using System.Text;
using System.Text.Json.Serialization;

namespace Philips.PmsMR.Acquisition.AcqGlo
{
    public enum TypeOfApplicationMode
    {
        AGDEF_APPL_IMAGING,

    }

    [Serializable]
    public class ScanProcedure
    {
        class Header
        {
            /* uint <=> UInt32 */
            public uint v1;
            public uint nints;
            public uint v4;
            public uint nfloats;
            public uint v6;
            public uint nstrings;
            public uint v8;
            public uint numparams;
        }
        [Flags]
        enum ParamType
        {
            FloatType = 0,
            IntegerType = 1,
            StringType = 2,
            EnumType = 4,
        }
        //[StructLayout(LayoutKind.Sequential, Pack = 1)]
        class Param
        {
            //[MarshalAs(UnmanagedType.ByValArray, SizeConst = 32 + 1)]
            //public char[] name;
            public string name;
            public byte boolean; // for disp
            public ParamType param_type;
            public uint dim;
            public int v4;
            public int offset;
            // bookkeeping:
            public long cur_pos1;
            public long cur_pos2;
        }
        private void decodeParameterData(byte[] parameterData)
        {
            // https://stackoverflow.com/questions/14715519/parsing-byte-array-that-contains-different-types-of-data
            // int length = parameterData.Length;
            // byte[] bytes = { 0, 0, 0, 25 };
            // int i = BitConverter.ToInt32(bytes, 0);
            Header header;
            var listParam = new List<Param>();

            using (var reader = new BinaryReader(new MemoryStream(parameterData), Encoding.ASCII))
            {
                // var i = reader.ReadInt32();
                header = new Header
                {
                    v1 = reader.ReadUInt32(),
                    nints = reader.ReadUInt32(),
                    v4 = reader.ReadUInt32(),
                    nfloats = reader.ReadUInt32(),
                    v6 = reader.ReadUInt32(),
                    nstrings = reader.ReadUInt32(),
                    v8 = reader.ReadUInt32(),
                    numparams = reader.ReadUInt32(),
                };
                for (var i = 0; i < header.numparams; ++i)
                {
                    var param = new Param
                    {
                        name = Encoding.ASCII.GetString(reader.ReadBytes(32 + 1).TakeWhile(b => b != 0x0).ToArray()),
                        boolean = reader.ReadByte(),
                        param_type = (ParamType)reader.ReadInt32(),
                        dim = reader.ReadUInt32(),
                        cur_pos1 = reader.BaseStream.Position,
                        v4 = reader.ReadInt32(),
                        cur_pos2 = reader.BaseStream.Position,
                        offset = reader.ReadInt32(),
                    };
                    Debug.Assert(param.boolean == 0 || param.boolean == 1);
                    Debug.Assert(Enum.IsDefined(typeof(ParamType), param.param_type));
                    listParam.Add(param);
                }
            }
            var ret = new Dictionary<Param, object>();
            var arraySpan = new Span<byte>(parameterData);
            List<float> retFloat;
            List<int> retInt;
            List<uint> retEnum;
            foreach (var param in listParam)
            {
                var o2 = param.offset + (int)param.cur_pos2;
                var o1 = param.v4 + (int)param.cur_pos1;
                Span<byte> slice2 = arraySpan.Slice(o2, 4 * (int)param.dim);
                Span<byte> slice1 = arraySpan.Slice(o1, 4 * (int)param.dim);
                switch (param.param_type)
                {
                    case ParamType.FloatType:
                        using (var reader = new BinaryReader(new MemoryStream(slice2.ToArray()), Encoding.ASCII))
                        {
                            retFloat = new List<float>();
                            for (int j = 0; j < param.dim; ++j)
                                retFloat.Add(reader.ReadSingle());
                            ret.Add(param, retFloat);
                        }
                        break;
                    case ParamType.IntegerType:
                        using (var reader = new BinaryReader(new MemoryStream(slice2.ToArray()), Encoding.ASCII))
                        {
                            retInt = new List<int>();
                            for (int j = 0; j < param.dim; ++j)
                                retInt.Add(reader.ReadInt32());
                            ret.Add(param, retInt);
                        }
                        break;
                    case ParamType.EnumType:
                        using (var reader = new BinaryReader(new MemoryStream(slice2.ToArray()), Encoding.ASCII))
                        {
                            retEnum = new List<uint>();
                            for (int j = 0; j < param.dim; ++j)
                            {
                                var val = reader.ReadUInt32();
                                Debug.Assert(val == 0x0);
                                retEnum.Add(val);
                            }
                            ret.Add(param, retEnum);
                        }
                        break;
                    default:
                        throw new NotImplementedException();

                }
            }
            if (header.v1 == 0x1)
            {
                System.Console.WriteLine("ICI");
            }
            else
            {
                System.Console.WriteLine("ICI");
            }

        }
        private byte[] parameterData;

        [JsonPropertyName("parameterData")]
        public byte[] MyParameterData
        {
            get
            {
                Guid g = Guid.NewGuid();
                System.Console.WriteLine(g.ToString());
                var fs = new FileStream(g.ToString() + ".raw", FileMode.Create);
                fs.Write(parameterData, 0, parameterData.Length);
                fs.Close();

                decodeParameterData(parameterData);
                return parameterData;
            }
        }

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
        public BitArray allowedMainSystemTypes;
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
