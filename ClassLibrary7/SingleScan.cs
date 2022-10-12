using Philips.PmsMR.Platform.Aswglo;
using System;

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
        public object list;
    }
    [Serializable]
    public class ExecutionStep
    {
        public object singleScan ;
        public object contrastInjectionProtocol;
        public bool contrast;
        public bool executionStepContrastUsed;
        public object dynamicStep;

    }

    [Serializable]
    public class SingleScan
    {
        public string name;
        public SmartPlanType smartGeoType;
        public object vertebraRanges;
        public object scanProcedure;
        public int stationNumber;
        public object detail;
        public object geometryName;
        public bool reuseStackSizes;
        public object viewingProcedure;
        public bool pushToWorkStation;
        public int geoLinkID;
        public object scanProperties;
        public bool useViewingProcedure;
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
