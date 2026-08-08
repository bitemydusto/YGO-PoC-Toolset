using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PoCTools
{
    public class CardJSON
    {
        public string ID { get; set; } = string.Empty;
        public string Name { get; set; } = string.Empty;
        public int Level { get; set; }
        [JsonConverter(typeof(StringEnumConverter))]
        public CardAttribute Attribute { get; set; } = CardAttribute.Unkown;
        [JsonConverter(typeof(StringEnumConverter))]
        public CardType Type { get; set; } = CardType.Unkown;
        [JsonConverter(typeof(StringEnumConverter))]
        public CardSubType SubType { get; set; } = CardSubType.Unkown;
        [JsonConverter(typeof(StringEnumConverter))]
        public SpellTrapType SpellTrap {  get; set; } = SpellTrapType.Unkown;
        public int ATK { get; set; }
        public int DEF { get; set; }
        public string ImageName { get; set; } = string.Empty;
        public bool Yugi { get; set; }
        public bool Kaiba { get; set; }
        public bool Joey { get; set; }
        public string Description { get; set; } = string.Empty;
    }
}
