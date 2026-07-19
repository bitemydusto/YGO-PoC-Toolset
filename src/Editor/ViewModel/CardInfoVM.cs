using CommunityToolkit.Mvvm.ComponentModel;
using PoCTools;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ViewModel
{
    public partial class CardInfoVM : ObservableObject
    {
        private Card _card;
        private string _libraryPath;
        bool isLoading = true;

        public Card Card => _card;

        [ObservableProperty]
        public partial string DisplayName { get; set; } = string.Empty;
        [ObservableProperty]
        public partial string Name { get; set; } = string.Empty;
        [ObservableProperty]
        public partial string ID { get; set; } = string.Empty;
        [ObservableProperty]
        public partial string ImageName { get; set; } = string.Empty;
        [ObservableProperty]
        public partial string ImagePath { get; set; } = string.Empty;
        [ObservableProperty]
        public partial string ImageError { get; set; } = string.Empty;
        [ObservableProperty]
        public partial string Description { get; set; } = string.Empty;
        [ObservableProperty]
        public partial int Level { get; set; }
        [ObservableProperty]
        public partial string ATK { get; set; }
        [ObservableProperty]
        public partial string DEF { get; set; }
        [ObservableProperty]
        public partial CardAttribute Attribute { get; set; }
        [ObservableProperty]
        public partial CardType Type { get; set; }
        [ObservableProperty]
        public partial CardSubType SubType { get; set; }
        [ObservableProperty]
        public partial SpellTrapType SpellTrapType { get; set; }
        [ObservableProperty]
        public partial string PropertyBits { get; set; } = string.Empty;
        [ObservableProperty]
        public partial bool VersionYugi { get; set; }
        [ObservableProperty]
        public partial bool VersionKaiba { get; set; }
        [ObservableProperty]
        public partial bool VersionJoey { get; set; }

        public CardInfoVM(Card card, string libraryPath)
        {
            _card = card;
            _libraryPath = libraryPath;

            Name = _card.Name;
            ID = $"0x{_card.ID.ToString("X").PadLeft(4, '0')}";
            DisplayName = $"{ID}   {Name}";
            ImageName = _card.ImageName;
            ImagePath = Path.Combine(_libraryPath, "card", _card.ImageName);
            ImageError = Path.Exists(ImagePath) ? string.Empty : "Image not found!";
            Description = _card.Description;

            Type = _card.Type;
            Attribute = _card.Attribute;
            Level = _card.Level;
            SpellTrapType = _card.SpellTrapType;
            SubType = _card.SubType;
            ATK = _card.ATK.ToString();
            DEF = _card.DEF.ToString();
            VersionYugi = _card.VersionYugi;
            VersionKaiba = _card.VersionKaiba;
            VersionJoey = _card.VersionJoey;

            SetPropertyBits();

            isLoading = false;
        }
        private void SetPropertyBits()
        {
            string attrBits = Convert.ToString((_card.PropertyBinary >> 29 & 0x07), 2).PadLeft(3, '0');
            string lvlBits = Convert.ToString((_card.PropertyBinary >> 25 & 0x0F), 2).PadLeft(4, '0');
            string typeBits = Convert.ToString((_card.PropertyBinary >> 20 & 0x1F), 2).PadLeft(5, '0');
            string subTypeBits = Convert.ToString((_card.PropertyBinary >> 18 & 0x03), 2).PadLeft(2, '0');
            string atkBits = Convert.ToString((_card.PropertyBinary >> 9 & 0x01FF), 2).PadLeft(9, '0');
            string defBits = Convert.ToString((_card.PropertyBinary & 0x01FF), 2).PadLeft(9, '0');

            PropertyBits = $"{attrBits}   {lvlBits}   {typeBits}   {subTypeBits}   {atkBits}   {defBits}";
        }
        partial void OnNameChanged(string oldValue, string newValue)
        {
            if (oldValue == newValue) return;

            DisplayName = $"{ID}   {Name}";
            _card.Name = newValue;
        }
        partial void OnDescriptionChanged(string oldValue, string newValue)
        {
            if (oldValue == newValue) return;

            _card.Description = newValue;
        }
        partial void OnImageNameChanged(string oldValue, string newValue)
        {
            if (oldValue == newValue) return;
            if (!string.IsNullOrEmpty(newValue) && Path.HasExtension(newValue))
            {
                ImagePath = Path.Combine(_libraryPath, "card", newValue);
                _card.ImageName = ImageName;
                ImageError = Path.Exists(ImagePath) ? string.Empty : "Image not found!";
            }
        }
        partial void OnLevelChanged(int oldValue, int newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.Level = newValue;
            SetPropertyBits();
        }
        partial void OnATKChanged(string oldValue, string newValue)
        {
            if (oldValue == newValue || isLoading) return;

            bool isValid = newValue == string.Empty || (newValue.All(char.IsDigit) && int.TryParse(newValue, out int parsedValue) && parsedValue >= 0 && parsedValue < 5120);
            if (!isValid)
            {
                ATK = oldValue;
                return;
            }
            if (newValue == string.Empty) return;

            _card.ATK = int.Parse(newValue);
            SetPropertyBits();
        }
        partial void OnDEFChanged(string oldValue, string newValue)
        {
            if (oldValue == newValue || isLoading) return;

            bool isValid = newValue == string.Empty || (newValue.All(char.IsDigit) && int.TryParse(newValue, out int parsedValue) && parsedValue >= 0 && parsedValue < 5120);
            if (!isValid)
            {
                DEF = oldValue;
                return;
            }
            if (newValue == string.Empty) return;

            _card.DEF = int.Parse(newValue);
            SetPropertyBits();
        }
        partial void OnAttributeChanged(CardAttribute oldValue, CardAttribute newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.Attribute = newValue;
            SetPropertyBits();
        }
        partial void OnTypeChanged(CardType oldValue, CardType newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.Type = newValue;
            SetPropertyBits();
        }
        partial void OnSubTypeChanged(CardSubType oldValue, CardSubType newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.SubType = newValue;
            SetPropertyBits();
        }
        partial void OnSpellTrapTypeChanged(SpellTrapType oldValue, SpellTrapType newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.SpellTrapType = newValue;
            SetPropertyBits();
        }
        partial void OnVersionYugiChanged(bool oldValue, bool newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.VersionYugi = newValue;
        }
        partial void OnVersionKaibaChanged(bool oldValue, bool newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.VersionKaiba = newValue;
        }
        partial void OnVersionJoeyChanged(bool oldValue, bool newValue)
        {
            if (oldValue == newValue || isLoading) return;

            _card.VersionJoey = newValue;
        }
    }
}
