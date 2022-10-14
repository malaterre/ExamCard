using System;
using System.ComponentModel.DataAnnotations;
using System.Runtime.Remoting.Metadata;

namespace Philips.PmsMR.Platform.ECPlatform
{
    [Serializable]
    public class HtmlItemDescription
    {
        //[SoapAttribute(DataTypeAttribute = "base64Binary")]
        public object dataBuffer;
        // new version:
        /*public object htmlDataBuffer;*/
    }
}
