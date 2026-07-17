using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.ConstrainedExecution;
using System.Text;
using System.Threading.Tasks;

namespace PoCTools
{
    public class Card
    {
        public ushort ID { get; set; } = 0x0;
        public string Name { get; set; } = string.Empty;
        public string Description { get; set; } = string.Empty;
        public string ImageName { get; set; } = string.Empty;
        public uint PropertyBinary { get; set; } = 0x0;
        public ushort VersionBinary { get; set; } = 0x0;
        public CardType Type
        {
            get
            {
                return (int)((this.PropertyBinary >> 20) & 0x1F) switch
                {
                    1 => CardType.Dragon,
                    2 => CardType.Zombie,
                    3 => CardType.Fiend,
                    4 => CardType.Pyro,
                    6 => CardType.Rock,
                    7 => CardType.Machine,
                    8 => CardType.Fish,
                    9 => CardType.Dinosaur,
                    10 => CardType.Insect,
                    11 => CardType.Beast,
                    12 => CardType.BeastWarrior,
                    13 => CardType.Plant,
                    14 => CardType.Aqua,
                    15 => CardType.Warrior,
                    16 => CardType.WingedBeast,
                    17 => CardType.Fairy,
                    18 => CardType.SpellCaster,
                    19 => CardType.Thunder,
                    20 => CardType.Reptile,
                    21 => CardType.Trap,
                    22 => CardType.Spell,
                    24 => CardType.Divine,

                    _ => CardType.Unkown
                };
            }
            set
            {
                int type = value switch
                {
                    CardType.Dragon => 1,
                    CardType.Zombie => 2,
                    CardType.Fiend => 3,
                    CardType.Pyro => 4,
                    CardType.Rock => 6,
                    CardType.Machine => 7,
                    CardType.Fish => 8,
                    CardType.Dinosaur => 9,
                    CardType.Insect => 10,
                    CardType.Beast => 11,
                    CardType.BeastWarrior => 12,
                    CardType.Plant => 13,
                    CardType.Aqua => 14,
                    CardType.Warrior => 15,
                    CardType.WingedBeast => 16,
                    CardType.Fairy => 17,
                    CardType.SpellCaster => 18,
                    CardType.Thunder => 19,
                    CardType.Reptile => 20,
                    CardType.Trap => 21,
                    CardType.Spell => 22,
                    CardType.Divine => 24,

                    _ => 0
                };
                PropertyBinary &= ~(0x1Fu << 20);
                PropertyBinary |= (uint)(type & 0x1F) << 20;
            }
        }
        public CardAttribute Attribute
        { 
            get
            {
                int attr = this.Type switch
                {
                    CardType.Trap => 9,
                    CardType.Spell => 8,
                    
                    _ => (int)(this.PropertyBinary >> 29)
                };
                return attr switch
                {
                    0 => CardAttribute.Divine,
                    1 => CardAttribute.Light,
                    2 => CardAttribute.Dark,
                    3 => CardAttribute.Water,
                    4 => CardAttribute.Fire,
                    5 => CardAttribute.Earth,
                    6 => CardAttribute.Wind,
                    8 => CardAttribute.Spell,
                    9 => CardAttribute.Trap,

                    _ => CardAttribute.Unkown
                };
            }
            set
            {
                int attr = value switch
                {
                    CardAttribute.Divine => 0,
                    CardAttribute.Light => 1,
                    CardAttribute.Dark => 2,
                    CardAttribute.Water => 3,
                    CardAttribute.Fire => 4,
                    CardAttribute.Earth => 5,
                    CardAttribute.Wind => 6,
                    CardAttribute.Spell => 0,
                    CardAttribute.Trap => 1,

                    _ => 0
                };
                PropertyBinary &= ~(0x7u << 29);
                PropertyBinary |= (uint)(attr & 0x7) << 29;
            }
        }
        public int Level
        {
            get
            {
                int lvl = this.Type switch
                {
                    CardType.Spell or CardType.Trap => 0,

                    _ => (int)((this.PropertyBinary >> 25) & 0x0F)
                };
                //if (this.Type == CardType.Divine) lvl = 10;
                return lvl;
            }
            set
            {
                PropertyBinary &= ~(0xFu << 25);
                PropertyBinary |= (uint)(value & 0x0F) << 25;
            }
        }
        public SpellTrapType SpellTrapType
        {
            get
            {
                int st = this.Type switch
                {
                    CardType.Trap or CardType.Spell => (int)((this.PropertyBinary >> 17) & 0x07),

                    _ => 0
                };
                return st switch
                {
                    0 => SpellTrapType.Normal,
                    1 => SpellTrapType.Counter,
                    2 => SpellTrapType.Field,
                    3 => SpellTrapType.Equip,
                    4 => SpellTrapType.Continuous,
                    5 => SpellTrapType.Quickplay,
                    6 => SpellTrapType.Ritual,
                    _ => SpellTrapType.Unkown
                };
            }
            set
            {
                int st = value switch
                {
                    SpellTrapType.Normal => 0,
                    SpellTrapType.Counter => 1,
                    SpellTrapType.Field => 2,
                    SpellTrapType.Equip => 3,
                    SpellTrapType.Continuous => 4,
                    SpellTrapType.Quickplay => 5,
                    SpellTrapType.Ritual => 6,

                    _ => 0
                };
                PropertyBinary &= ~(0x7u << 17);
                PropertyBinary |= (uint)(st & 0x7) << 17;
            }
        }
        public CardSubType SubType
        {
            get
            {
                int sub = this.Type switch
                {
                    CardType.Trap => 8,
                    CardType.Spell => 7,

                    _ => (int)((this.PropertyBinary >> 18) & 0x03)
                };
                //if (this.ID == 0x776) sub = 3;
                //if (this.ID > 0x776 && this.ID < 0x779) sub = 1;
                return sub switch
                {
                    0 => CardSubType.Normal,
                    1 => CardSubType.Effect,
                    2 => CardSubType.Fusion,
                    3 => CardSubType.Ritual,
                    7 => CardSubType.Spell,
                    8 => CardSubType.Trap,

                    _ => CardSubType.Unkown
                };
            }
            set
            {
                int sub = value switch
                {
                    CardSubType.Normal => 0,
                    CardSubType.Effect => 1,
                    CardSubType.Fusion => 2,
                    CardSubType.Ritual => 3,
                    CardSubType.Spell => 0,
                    CardSubType.Trap => 0,

                    _ => 0
                };
                PropertyBinary &= ~(0x3u << 18);
                PropertyBinary |= (uint)(sub & 0x3) << 18;
            }
        }
        public int ATK
        {
            get
            {
                int atk = this.Type switch
                {
                    CardType.Spell or CardType.Trap => 0,

                    _ => (int)(((this.PropertyBinary >> 9) & 0x1FF) * 10)
                };
                //if (this.Type == CardType.Divine) atk = 4000;
                return atk;
            }
            set
            {
                if (value >= 5120) return;

                PropertyBinary &= ~(0x1FFu << 9);
                PropertyBinary |= (uint)((value / 10) & 0x1FF) << 9;
            }
        }
        public int DEF
        {
            get
            {
                int def = this.Type switch
                {
                    CardType.Spell or CardType.Trap => 0,

                    _ => (int)((this.PropertyBinary & 0x1FF) * 10)
                };
                //if (this.Type == CardType.Divine) def = 4000;
                return def;
            }
            set
            {
                if (value >= 5120) return;

                PropertyBinary &= ~0x1FFu;
                PropertyBinary |= (uint)((value / 10) & 0x1FF);
            }
        }
        public bool VersionYugi
        {
            get
            {
                return (this.VersionBinary & 0x1) != 0;
            }
            set
            {
                if (value) VersionBinary |= 0x1;
                else VersionBinary &= 0x6;
            }
        }
        public bool VersionKaiba
        {
            get
            {
                return (this.VersionBinary & 0x2) != 0;
            }
            set
            {
                if (value) VersionBinary |= 0x2;
                else VersionBinary &= 0x5;
            }
        }
        public bool VersionJoey
        {
            get
            {
                return (this.VersionBinary & 0x4) != 0;
            }
            set
            {
                if (value) VersionBinary |= 0x4;
                else VersionBinary &= 0x3;
            }
        }
    }
}
