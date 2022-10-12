using Philips.PmsMR.Platform.Aswglo;
using Philips.PmsMR.Platform.ECPlatform;
using Philips.PmsMR.Platform.ScannerContext;
using System;

namespace Philips.PmsMR.ExamCards.ECModel
{
    [Serializable]
    [System.Xml.Serialization.SoapTypeAttribute(Namespace = "urn:CHAMADO")]
    public class ExamCard
    {
        public string name;
        public ECInstrumentList instrumentList;
        public ExamCardData data;
        public HtmlItemDescription description;
        public PatientPosition patientOrientation;
        public string anatomicRegion;
        public string laterality;
        public string[] namedGeometries;
        public int heartRate;
        public bool autoFillInHeartRate;
        public MainSysType mainSystemType;
        public ExamCardConfig configuration;
        public ExamCardsVersion version;
        public int alignOverlap;
        public bool pushToWorkStation;
        // new version:
        /*public object anatomy;
        public object reviewRequired;
        public object paediatric;
        public object patientWeight;
        public object selectedCoils;*/
    }
    [Serializable]
    public class ECInstrumentList
    {
        public object list;
    }
    [Serializable]
    public class ExamCardConfig
    {
        public object allowedMainSystemTypes;
        public object allowedGradientChains;
        public object swOptions;
        public object hwOptions;
        public object coilOptions;
    }
    [Serializable]
    public class ExamCardsVersion
    {
        public string swVersion;
        public string modelVersion;
        public string methodsVersion;
    }
    [Serializable]
    public class ECInstrument
    {
        // old version:
        public object scanStep;
        // new version:
        /*public object scanSet;*/
    }
    [Serializable]
    public class ScanStep
    {
        public string name;
        public object currentScanSet;

    }

}
