using Philips.PmsMR.Platform.ECPlatform;
using System;

namespace Philips.PmsMR.ExamCards.ECModel
{
    [Serializable]
    public class ExamCardData
    {
        public string tableUsage;
        public RelationshipManagerData processingStepManagerData;
        public bool geoLinkPropagation;
    }
    [Serializable]
    public class OutputDescriptorData
    {
        public object seriesType;
        public object description;
        public object seriesName;
        public object viewingProcedure;
        public object useViewingProcedure;
        public object pushToWorkStation;
    }

}
