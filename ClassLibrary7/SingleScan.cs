using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
        public object smartGeoType;
        public object vertebraRanges;
        public object scanProcedure;
        public object stationNumber;
        public object detail;
        public object geometryName;
        public object reuseStackSizes;
        public object viewingProcedure;
        public object pushToWorkStation;
        public object geoLinkID;
        public object scanProperties;
        public object useViewingProcedure;
    }

    [Serializable]
    public class ContrastInjectionProtocol
    {
        public object agent;
        public object routeOfAdministration;
        public int volume;
        public double concentration;
        public int injectionStartDynamicNumber;
    }
}
