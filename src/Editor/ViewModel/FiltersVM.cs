using CommunityToolkit.Mvvm.ComponentModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ViewModel
{
    public partial class FiltersVM : ObservableObject
    {
        [ObservableProperty]
        public partial bool Normal { get; set; } = true;

        [ObservableProperty]
        public partial bool Effect { get; set; } = true;
        [ObservableProperty]
        public partial bool Fusion { get; set; } = true;
        [ObservableProperty]
        public partial bool Ritual { get; set; } = true;
        [ObservableProperty]
        public partial bool NormalSpell { get; set; } = true;
        [ObservableProperty]
        public partial bool QuickPlaySpell { get; set; } = true;
        [ObservableProperty]
        public partial bool ContinuousSpell { get; set; } = true;
        [ObservableProperty]
        public partial bool EquipSpell { get; set; } = true;
        [ObservableProperty]
        public partial bool FieldSpell { get; set; } = true;
        [ObservableProperty]
        public partial bool RitualSpell { get; set; } = true;
        [ObservableProperty]
        public partial bool NormalTrap { get; set; } = true;
        [ObservableProperty]
        public partial bool ContinuousTrap { get; set; } = true;
        [ObservableProperty]
        public partial bool CounterTrap { get; set; } = true;
    }
}
