using Philips.PmsMR.Platform.Aswglo;
using System;

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
    public class ScanProperties
    {
        public bool pause;
        public bool scanAlign;
        public int scanAlignOverlap;
        public SmartPlanType smartScoutType;
        public bool madeByPhilips;
        public bool suspendableDynamic;
        public object requiredConfiguration;
    }
    [Serializable]
    public class ScanProcedureConfig
    {
        public object allowedMainSystemTypes ;
        public object allowedGradientChains;
        public object swOptions;
        public object hwOptions;
        public object coilOptions;
    }
    [Serializable]
    public class DynamicStepProperties
    {
        public DynamicScanDelayMode delayMode;
        public int delayDuration;
        public int dynamicCount;
        public int firstDynamic;
    }
}
