using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json.Serialization;

namespace Philips.PmsMR.Platform.ECPlatform
{
    [Serializable]
    public class RelationshipManagerData
    {
        private Hashtable outputsToInputsRelations;
        private Hashtable inputsToOutputsRelations;
        
        [JsonPropertyName("outputsToInputsRelations")]
        // Key: OutputDescriptorData Value:  ArrayList  SingleScan 
        public Dictionary<string, List<object>> MyHashtableSorted
        {
            get
            {
                /*
                return new Dictionary<string, List<object>>(
                    inputsToOutputsRelations.Cast<DictionaryEntry>()
                    .ToDictionary(x => x.Key.ToString(), x => ((ArrayList)x.Value).Cast<object>().ToList())
                    );
                */
                var ret = new Dictionary<string, List<object>>();
                foreach(DictionaryEntry entry in outputsToInputsRelations)
                {
                    //OutputDescriptorData dbg = (OutputDescriptorData)entry.Key;
                    var debug = entry.Key.ToString();
                    ret.Add(entry.Key.ToString(), ((ArrayList)entry.Value).Cast<object>().ToList());
                }
                return ret;
            }
        }

        [JsonPropertyName("inputsToOutputsRelations")]
        // Key: SingleScan Value:  ArrayList  OutputDescriptorData 
        public Dictionary<string, List<object>> MyHashtableSorted2
        {
            get => new Dictionary<string, List<object>>(
                         inputsToOutputsRelations
                         .Cast<DictionaryEntry>()
                         .ToDictionary(x => x.Key.ToString(), x => ((ArrayList)x.Value).Cast<object>().ToList())
                    );
        }
        
    }
}
