using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Philips.PmsMR.Platform.ECPlatform
{
    [Serializable]
    public class HtmlItemDescription
    {
        public byte[] dataBuffer;
        // new version:
        public object htmlDataBuffer;
    }
}
