using System;
using System.Text.Json.Serialization;

namespace Philips.PmsMR.ExamCards.ECModel
{
    [Serializable]
    public class ExamCardData
    {
        public string tableUsage;
        // too much stuff in this table:
        private Philips.PmsMR.Platform.ECPlatform.RelationshipManagerData /*object*/ processingStepManagerData;
        public bool geoLinkPropagation;
    }
    [Serializable]
    //[Obsolete]
    public class OutputDescriptorData /*: IHashCodeProvider, IComparer, IComparable *//* json */
    {
        public object seriesType;
        public object description;
        public object seriesName;
        public object viewingProcedure;
        public bool useViewingProcedure;
        public bool pushToWorkStation;

        public override string ToString()
        {
            Guid g = Guid.NewGuid();
            return g.ToString();
        }
        /*
public int CompareTo(object obj)
{
   if (obj != null && obj.GetType() != GetType())
       throw new ArgumentException(string.Format("Object must be of type {0}", GetType()));
   return CompareTo((OutputDescriptorData)obj);
}
public int CompareTo(OutputDescriptorData other) => (seriesType, description, seriesName, viewingProcedure, useViewingProcedure, pushToWorkStation).CompareTo((other.seriesType, other.description, other.seriesName, other.viewingProcedure, other.useViewingProcedure, other.pushToWorkStation));

int IComparer.Compare(object x, object y)
{
   throw new NotImplementedException();
}
bool Equals(object obj)
{
   throw new NotImplementedException();
}
int GetHashCode(object obj)
{
   throw new NotImplementedException();
}
int IHashCodeProvider.GetHashCode(object obj)
{
   throw new NotImplementedException();
}*/
    }
}
