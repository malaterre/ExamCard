using Philips.PmsMR.Platform.Aswglo;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Philips.PmsMR.Acquisition.AcqGlo
{
    [Serializable]
    public class ScanProcedure
    {
        public byte[] parameterData;
        public object methodDescription;
        public object name;
        public object applicationMode;
        public object linkToOriginalProtocol;
        public object lastModifiedTimeUtc;
        public object linkState;
        public object NameChanged;
        public object Modified;
        public object Disposing;
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
        public object delayMode;
        public int delayDuration;
        public int dynamicCount;
        public int firstDynamic;
    }
}
