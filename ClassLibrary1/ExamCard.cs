using Philips.PmsMR.Platform.ScannerContext;
using System;

namespace Philips.PmsMR.ExamCards.ECModel
{
    [Serializable]
    [System.Xml.Serialization.SoapTypeAttribute(Namespace = "urn:CHAMADO")]
    public class ExamCard
    {
        public string name;
        public string instrumentList;
        public string data;
        public string description;
        public PatientPosition patientOrientation;
        public string anatomicRegion;
        public string laterality;
        public string namedGeometries;
        public int heartRate;
        public bool autoFillInHeartRate;
        public string mainSystemType;
        public string configuration;
        public string version;
        public int alignOverlap;
        public bool pushToWorkStation;
    }
}
