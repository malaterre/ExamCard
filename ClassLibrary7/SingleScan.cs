using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Philips.PmsMR.Acquisition.ScanSet
{
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
}
