using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Maui.Storage;
using System.Collections.ObjectModel;
using PoCTools;
using PoCTools.Library;


namespace Editor.ViewModel
{
    public partial class CardViewerVM(IPoCLibrary library) : ObservableObject
    {
        IPoCLibrary _library = library;

        private List<CardInfoVM> cards = [];

        [ObservableProperty]
        public partial CardInfoVM SelectedCard { get; set; }
        [ObservableProperty]
        public partial int SelectedAttributeIndex { get; set; } = -1;
        [ObservableProperty]
        public partial int SelectedTypeIndex { get; set; } = -1;
        [ObservableProperty]
        public partial int SelectedSubTypeIndex { get; set; } = -1;
        [ObservableProperty]
        public partial int SelectedSpellTrapTypeIndex { get; set; } = -1;
        [ObservableProperty]
        public partial string SearchText { get; set; } = string.Empty;
        [ObservableProperty]
        public partial bool IsLoading { get; set; }
        [ObservableProperty]
        public partial bool IsLibraryLoaded { get; set; }
        [ObservableProperty]
        public partial bool IsCardSelected { get; set; }


        [ObservableProperty]
        public partial ObservableCollection<CardInfoVM> Cards { get; set; } = [];

        async Task Load(string path)
        {

            try
            {
                await _library.LoadLibrary(path);
            }
            catch (MissingBinaryException e)
            {
                await Shell.Current.DisplayAlert("Error", $"Missing binaries: {string.Join(", ", e.MissingBinaries)}", "OK");
                return;
            }
            catch (DirectoryNotFoundException e)
            {
                await Shell.Current.DisplayAlert("Error", e.Message, "OK");
                return;
            }
            catch (Exception e)
            {
                await Shell.Current.DisplayAlert("Error", $"Unexpected error occurred while loading library: {e.Message}", "OK");
                return;
            }
            foreach (var item in _library.Cards!)
            {
                cards.Add(new CardInfoVM(item, path));
            }
            Cards = new(cards);

            IsLibraryLoaded = true;
            //await Shell.Current.DisplayAlert("Success", "Library loaded!", "OK");
        }

        [RelayCommand]
        async Task OpenLibrary()
        {
            var result = await FolderPicker.Default.PickAsync();
            if (result.Folder != null)
            {
                IsLoading = true;
                await Load(result.Folder.Path);
                IsLoading = false;
            }

        }
        [RelayCommand]
        async Task SaveLibrary()
        {
            var result = await FolderPicker.Default.PickAsync();
            if (result.Folder != null)
            {
                IsLoading = true;
                try
                {
                    await _library.SaveLibrary(result.Folder.Path);
                    await Shell.Current.DisplayAlert("Success", $"Library saved!", "OK");
                }
                catch (LibraryIsNotLoadedException)
                {
                    await Shell.Current.DisplayAlert("Error", "Library is not loaded. Please load a library before saving.", "OK");
                }
                catch (Exception e)
                {
                    await Shell.Current.DisplayAlert("Error", $"Unexpected error occurred while saving library: {e.Message}", "OK");
                }
                IsLoading = false;
            }
        }
        partial void OnSearchTextChanged(string value)
        {
            if (string.IsNullOrWhiteSpace(SearchText))
            {
                Cards = new(cards);
            }
            else
            {
                var filteredCards = cards.Where(c => c.Name.Contains(SearchText, StringComparison.OrdinalIgnoreCase)).ToList();
                Cards = new(filteredCards);
            }
        }
        partial void OnSelectedCardChanged(CardInfoVM value)
        {
            if (value == null) return;
            IsCardSelected = true;

            SelectedAttributeIndex = AttributeList.ToList().IndexOf(value.Attribute.ToString());
            SelectedTypeIndex = TypeList.ToList().IndexOf(value.Type.ToString());
            SelectedSubTypeIndex = SubTypeList.ToList().IndexOf(value.SubType.ToString());
            SelectedSpellTrapTypeIndex = SpellTrapList.ToList().IndexOf(value.SpellTrapType.ToString());

        }
        partial void OnSelectedAttributeIndexChanged(int value)
        {
            if (Enum.TryParse<CardAttribute>(AttributeList[value], out var attr))
            {
                SelectedCard.Attribute = attr;
            }
        }
        partial void OnSelectedTypeIndexChanged(int value)
        {
            if (Enum.TryParse<CardType>(TypeList[value], out var type))
            {
                SelectedCard.Type = type;
            }
        }
        partial void OnSelectedSubTypeIndexChanged(int value)
        {
            if (Enum.TryParse<CardSubType>(SubTypeList[value], out var sub))
            {
                SelectedCard.SubType = sub;
            }
        }
        partial void OnSelectedSpellTrapTypeIndexChanged(int value)
        {
            if (Enum.TryParse<SpellTrapType>(SpellTrapList[value], out var st))
            {
                SelectedCard.SpellTrapType = st;
            }
        }
        public IReadOnlyList<string> LevelList { get; } = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"];
        public IReadOnlyList<string> AttributeList { get; } = Enum.GetNames(typeof(CardAttribute));
        public IReadOnlyList<string> TypeList { get; } = Enum.GetNames(typeof(CardType));
        public IReadOnlyList<string> SubTypeList { get; } = Enum.GetNames(typeof(CardSubType));
        public IReadOnlyList<string> SpellTrapList { get; } = Enum.GetNames(typeof(SpellTrapType));
    }
}
