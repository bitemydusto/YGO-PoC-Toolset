using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PoCTools
{
    public enum CardAttribute
    {
        Divine,
        Light,
        Dark,
        Water,
        Fire,
        Earth,
        Wind,
        Spell,
        Trap,
        Unkown
    }
    public enum CardType
    {
        Dragon,
        Zombie,
        Fiend,
        Pyro,
        Rock,
        Machine,
        Fish,
        Dinosaur,
        Insect,
        Beast,
        BeastWarrior,
        Plant,
        Aqua,
        Warrior,
        WingedBeast,
        Fairy,
        SpellCaster,
        Thunder,
        Reptile,
        Trap,
        Spell,
        Divine,
        Unkown
    }
    public enum CardSubType
    {
        Normal,
        Effect,
        Fusion,
        Ritual,
        Spell,
        Trap,
        Unkown
    }
    public enum SpellTrapType
    {
        Normal,
        Counter,
        Field,
        Equip,
        Continuous,
        Quickplay,
        Ritual,
        Unkown
    }
}
