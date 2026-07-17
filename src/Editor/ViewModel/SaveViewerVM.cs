using CommunityToolkit.Maui.Storage;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PoCTools;
using PoCTools.Decoder;
using PoCTools.Library;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage.Streams;

namespace Editor.ViewModel
{
    public partial class SaveViewerVM(IPoCDecoder decoder, IPoCLibrary library) : ObservableObject
    {
        private IPoCDecoder _decoder = decoder;
        private IPoCLibrary _library = library;

        private byte[] _saveData = [];
        private string _savePath = "";
        private List<SaveInfoVM> saveInfos = []; 

        [ObservableProperty]
        public partial ObservableCollection<SaveInfoVM> SaveInfos { get; set; } = [];
        [ObservableProperty]
        public partial bool IsSaveLoaded { get; set; }
        [ObservableProperty]
        public partial string SearchText { get; set; } = string.Empty;
        [ObservableProperty]
        public partial SaveInfoVM? SelectedCard { get; set; }
        [ObservableProperty]
        public partial bool IsLoading { get; set; }


        async Task LoadSaveData()
        {
            saveInfos.Clear();
            SaveInfos.Clear();

            using var reader = new BinaryReader(new FileStream(_savePath, FileMode.Open, FileAccess.Read));
            _saveData = reader.ReadBytes((int)reader.BaseStream.Length);

            byte[] output = new byte[_saveData.Length];
            _decoder.DecodeBuffer(_saveData, 0x1190, output);
            _saveData = output;

            for (int i = 0x14; i < 0x8CA; i+= 2)
            {
                ushort word = BitConverter.ToUInt16(_saveData, i);
                byte amount = (byte)(word & 0xFF);

                saveInfos.Add(new SaveInfoVM(_library.Cards?[(i - 0x14) / 2], amount.ToString()));
            }

            SaveInfos = new(saveInfos);
            IsSaveLoaded = true;
        }
        [RelayCommand]
        async Task OpenSave()
        {
            var result = await FilePicker.Default.PickAsync();
            if (result?.FullPath != null)
            {
                _savePath = result.FullPath;
                IsLoading = true;
                try
                {
                    await LoadSaveData();
                    //await Shell.Current.DisplayAlert("Success", "Save data loaded!", "OK");
                }
                catch (Exception e)
                {
                    await Shell.Current.DisplayAlert("Error", $"Failed to load save data: {e.Message}", "OK");
                }
                IsLoading = false;
            }

        }
        [RelayCommand]
        async Task SaveChanges()
        {
            IsLoading = true;

            for (int i = 0; i < SaveInfos.Count; i++)
            {
                int index = i* 2 + 0x14;
                _saveData[index] = Convert.ToByte(SaveInfos[i].Amount);
            }
            byte[] output = new byte[_saveData.Length];
            try
            {
                _decoder.EncodeBuffer(_saveData, 0x1190, output);
            }
            catch (Exception e)
            {
                await Shell.Current.DisplayAlert("Error", $"Failed to encode data: {e.Message}", "Ok");
                IsLoading = false;
                return;
            }
            try
            {
                using var writer = new BinaryWriter(File.Create(_savePath));
                writer.Write(output);
            }
            catch (Exception e)
            {
                await Shell.Current.DisplayAlert("Error", $"Failed to write save data: {e.Message}", "Ok");
            }
            await Shell.Current.DisplayAlert("Success", "Data saved!", "OK");
            IsLoading = false;
        }
        [RelayCommand]
        async Task EditAllCards()
        {
            var result = await Shell.Current.DisplayPromptAsync("Edit Amount", "", "OK", "Cancel", "Amount", 3, Keyboard.Numeric);
            if (result != null && int.TryParse(result, out int value) && value <= 255)
            {
                for (int i = 1; i < saveInfos.Count; i++)
                {
                    saveInfos[i].Amount = value.ToString();
                }
                SaveInfos = new(saveInfos);
            }
        }
        [RelayCommand]
        async Task EditAmount(SaveInfoVM card)
        {
            if (saveInfos.IndexOf(card) == 0) return;
            var result = await Shell.Current.DisplayPromptAsync("Edit Amount", "", "OK", "Cancel", "Amount", 3, Keyboard.Numeric);
            if (result != null && int.TryParse(result, out int value) && value <= 255)
            {
                int idx = saveInfos.IndexOf(card);
                saveInfos[idx].Amount = value.ToString();
                SaveInfos[idx].Amount = value.ToString();
            }
        }
        async partial void OnSelectedCardChanged(SaveInfoVM? value)
        {
            if (value == null) return;
            await EditAmount(value);
            SelectedCard = null;
        }
        partial void OnSearchTextChanged(string value)
        {
            if (string.IsNullOrWhiteSpace(SearchText))
            {
                SaveInfos = new(saveInfos);
            }
            else
            {
                var filteredCards = saveInfos.Where(c => c.DisplayName.Contains(SearchText, StringComparison.OrdinalIgnoreCase)).ToList();
                SaveInfos = new(filteredCards);
            }
        }
        string BytesToString(byte[] bytes)
        {
            string hexString = "";
            int i = 0;
            while (i < bytes.Length)
            {
                hexString += ($"{bytes[i].ToString("X").PadLeft(2, '0')}\t");
                i++;
                if (i % 16 == 0) hexString += "\n";
            }
            return hexString;
        }
    }
}
