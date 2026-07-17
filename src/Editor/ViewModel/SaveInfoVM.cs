using CommunityToolkit.Mvvm.ComponentModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PoCTools;

namespace Editor.ViewModel
{
    public partial class SaveInfoVM : ObservableObject
    {
        public Card? Card { get; set; }
        public string DisplayName { get; set; } = "";
        [ObservableProperty]
        public partial string Amount { get; set; } = "";
        public bool IsBackCard => Card == null || Card.ID == 0xFFFF;

        public SaveInfoVM(Card? card, string amount)
        {
            DisplayName = card == null ? "0x????   Unkown" : $"0x{card.ID.ToString("X").PadLeft(4, '0')}   {card.Name}";
            Amount = amount;
        }
        partial void OnAmountChanged(string oldValue, string newValue)
        {
            if (oldValue == newValue) return;

            bool isValid = newValue.All(char.IsDigit) && byte.TryParse(newValue, out byte parsedValue) && parsedValue >= 0;

            if (!isValid) Amount = oldValue;
        }
    }
}
