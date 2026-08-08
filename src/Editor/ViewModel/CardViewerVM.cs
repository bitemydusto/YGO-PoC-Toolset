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
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;


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
        [RelayCommand]
        async Task ImportCards()
        {
            var result = await FilePicker.Default.PickAsync(new PickOptions
            {
                PickerTitle = "Select a card file to import",
                FileTypes = new FilePickerFileType(new Dictionary<DevicePlatform, IEnumerable<string>>
                {
                    { DevicePlatform.WinUI, new[] { ".json" } },
                })
            });
            if (result == null) return;
            if (result.FullPath != null)
            {
                IsLoading = true;
                try
                {
                    var jsonString = await File.ReadAllTextAsync(result.FullPath);
                    List<CardJSON> imports = JsonConvert.DeserializeObject<List<CardJSON>>(jsonString)!;
                    List<CardJSON> failedImports = [];
                    List<CardJSON> notFoundImports = [];
                    foreach (var import in imports)
                    {
                        if (import.Name.Length > 64 ||
                            import.ATK > 5110 ||
                            import.DEF > 5110 ||
                            import.Level > 12)
                        {
                            failedImports.Add(import);
                            continue;
                        }

                        bool found = false;
                        foreach (var cardInfo in Cards)
                        {
                            int id = cardInfo.ID.StartsWith("0x") ? Convert.ToInt32(cardInfo.ID, 16) : Convert.ToInt32(cardInfo.ID);
                            int importId = import.ID.StartsWith("0x") ? Convert.ToInt32(import.ID, 16) : Convert.ToInt32(import.ID);

                            if (id == importId)
                            {
                                found = true;

                                cardInfo.Name = import.Name;
                                cardInfo.Level = import.Level;
                                cardInfo.Attribute = import.Attribute;
                                cardInfo.Type = import.Type;
                                cardInfo.SubType = import.SubType;
                                cardInfo.SpellTrapType = import.SpellTrap;
                                cardInfo.ATK = import.ATK.ToString();
                                cardInfo.DEF = import.DEF.ToString();
                                cardInfo.VersionYugi = import.Yugi;
                                cardInfo.VersionKaiba = import.Kaiba;
                                cardInfo.VersionJoey = import.Joey;
                                cardInfo.ImageName = import.ImageName;
                                cardInfo.Description = import.Description;
                            }
                        }
                        if (!found) notFoundImports.Add(import);
                    }

                    await Shell.Current.DisplayAlert("Import Result", $"Imported cards: {imports.Count - failedImports.Count - notFoundImports.Count}\nInvalid cards: {failedImports.Count}\nIDs not found: {notFoundImports.Count}", "OK");
                }
                catch (JsonException e)
                {
                    await Shell.Current.DisplayAlert("Error", $"Failed to parse JSON: {e.Message}", "OK");
                }
                catch (LibraryIsNotLoadedException)
                {
                    await Shell.Current.DisplayAlert("Error", "Library is not loaded. Please load a library before importing cards.", "OK");
                }
                catch (Exception e)
                {
                    await Shell.Current.DisplayAlert("Error", $"Unexpected error occurred while importing cards: {e.Message}", "OK");
                }
                IsLoading = false;
            }
        }
        [RelayCommand]
        async Task ExportSelected()
        {
            var result = await FilePicker.Default.PickAsync(new PickOptions
            {
                PickerTitle = "Select a card file to import",
                FileTypes = new FilePickerFileType(new Dictionary<DevicePlatform, IEnumerable<string>>
                {
                    { DevicePlatform.WinUI, new[] { ".json" } },
                })
            });
            if (result == null) return;
            if (result.FullPath != null)
            {
                IsLoading = true;
                try
                {
                    var jsonString = await File.ReadAllTextAsync(result.FullPath);
                    List<CardJSON> imports = JsonConvert.DeserializeObject<List<CardJSON>>(jsonString)!;

                    CardJSON selected = new()
                    {
                        ID = SelectedCard.ID,
                        Name = SelectedCard.Name,
                        Level = SelectedCard.Level,
                        Attribute = SelectedCard.Attribute,
                        Type = SelectedCard.Type,
                        SubType = SelectedCard.SubType,
                        SpellTrap = SelectedCard.SpellTrapType,
                        ATK = int.Parse(SelectedCard.ATK),
                        DEF = int.Parse(SelectedCard.DEF),
                        ImageName = SelectedCard.ImageName,
                        Yugi = SelectedCard.VersionYugi,
                        Kaiba = SelectedCard.VersionKaiba,
                        Joey = SelectedCard.VersionJoey,
                        Description = SelectedCard.Description
                    };

                    imports.Add(selected);

                    var updatedJsonString = JsonConvert.SerializeObject(imports, Formatting.Indented);
                    await File.WriteAllTextAsync(result.FullPath, updatedJsonString);

                    await Shell.Current.DisplayAlert("Export", $"Card exported successfully.", "OK");
                }
                catch (JsonException e)
                {
                    await Shell.Current.DisplayAlert("Error", $"Failed to parse JSON: {e.Message}", "OK");
                }
                catch (LibraryIsNotLoadedException)
                {
                    await Shell.Current.DisplayAlert("Error", "Library is not loaded. Please load a library before importing cards.", "OK");
                }
                catch (Exception e)
                {
                    await Shell.Current.DisplayAlert("Error", $"Unexpected error occurred while importing cards: {e.Message}", "OK");
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
