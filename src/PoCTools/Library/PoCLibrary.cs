using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PoCTools.Library
{
    public class PoCLibrary : IPoCLibrary
    {
        private Dictionary<string, byte[]> binaries = [];
        private List<string> imgNames = [];
        private List<Card>? cards;

        public List<Card>? Cards => cards;

        public async Task LoadLibrary(string path)
        {
            binaries.Clear();
            imgNames.Clear();
            cards?.Clear();

            //bin#
            string binPath = Path.Combine(path, "bin#");
            if (Directory.Exists(binPath))
            {
                foreach (string file in Directory.EnumerateFiles(binPath))
                {
                    string fileName = Path.GetFileName(file);
                    byte[] bin = await File.ReadAllBytesAsync(file);

                    binaries.Add(fileName, bin);
                }
                List<string> missingBinaries = [];

                if (!binaries.ContainsKey("card_id.bin")) missingBinaries.Add("card_id.bin");
                if (!binaries.ContainsKey("card_nameeng.bin")) missingBinaries.Add("card_nameeng.bin");
                if (!binaries.ContainsKey("card_desceng.bin")) missingBinaries.Add("card_desceng.bin");
                if (!binaries.ContainsKey("card_indxeng.bin")) missingBinaries.Add("card_indxeng.bin");
                if (!binaries.ContainsKey("card_prop.bin")) missingBinaries.Add("card_prop.bin");
                if (!binaries.ContainsKey("card_pack.bin")) missingBinaries.Add("card_pack.bin");

                if (missingBinaries.Count > 0) throw new MissingBinaryException(missingBinaries);
            }
            else throw new DirectoryNotFoundException($"Can't find folder 'bin#' at {path}");

            //card/list_card.txt
            string cardPath = Path.Combine(path, "card");
            if (Directory.Exists(cardPath))
            {
                string listPath = Path.Combine(cardPath, "list_card.txt");
                if (File.Exists(listPath))
                {
                    string[] lines = await File.ReadAllLinesAsync(listPath);
                    foreach (string line in lines)
                    {
                        if (!string.IsNullOrWhiteSpace(line) && !line.StartsWith("//"))
                        {
                            imgNames.Add(line.Trim());
                        }
                    }
                }
            }

            LoadCards();
        }

        public async Task SaveLibrary(string path)
        {
            if (cards == null) throw new LibraryIsNotLoadedException();

            string binPath = Path.Combine(path, "bin#");
            if (!Directory.Exists(binPath)) Directory.CreateDirectory(binPath);
            string cardPath = Path.Combine(path, "card");
            if (!Directory.Exists(cardPath)) Directory.CreateDirectory(cardPath);
            string miniPath = Path.Combine(path, "mini");
            if (!Directory.Exists(miniPath)) Directory.CreateDirectory(miniPath);

            using var nameWriter = new BinaryWriter(File.Create(Path.Combine(binPath, "card_nameeng.bin")));
            using var descWriter = new BinaryWriter(File.Create(Path.Combine(binPath, "card_desceng.bin")));
            using var idxWriter = new BinaryWriter(File.Create(Path.Combine(binPath, "card_indxeng.bin")));
            using var propWriter = new BinaryWriter(File.Create(Path.Combine(binPath, "card_prop.bin")));
            using var packWriter = new BinaryWriter(File.Create(Path.Combine(binPath, "card_pack.bin")));
            using var listWriter = new StreamWriter(File.Create(Path.Combine(cardPath, "list_card.txt")));
            using var miniWriter = new StreamWriter(File.Create(Path.Combine(miniPath, "list_card.txt")));

            int n = 0;
            foreach (Card card in cards)
            {
                nameWriter.Write(card.Name.PadRight(64, '\0').ToCharArray());
                propWriter.Write(card.PropertyBinary);
                packWriter.Write(card.VersionBinary);

                idxWriter.Write((uint)descWriter.BaseStream.Position);
                if (string.IsNullOrEmpty(card.Description)) descWriter.Write(("\0\0").ToCharArray());
                else
                {
                    descWriter.Write(Encoding.UTF8.GetBytes(card.Description.ToCharArray()));
                    descWriter.Write(("\0\0\0").ToCharArray());
                }
                // card/list_card.txt
                listWriter.WriteLine($"// {(string.IsNullOrEmpty(card.Name) ? "Back" : (card.Name))}");
                listWriter.WriteLine($"// {n.ToString().PadLeft(4, '0')}:[{card.ID.ToString().PadLeft(4, '0')}][0x{card.ID.ToString("X").PadLeft(4, '0')}]");
                listWriter.WriteLine(card.ImageName);
                // mini/list_card.txt
                miniWriter.WriteLine($"// {(string.IsNullOrEmpty(card.Name) ? "Back" : (card.Name))}");
                miniWriter.WriteLine($"// {n.ToString().PadLeft(4, '0')}:[{card.ID.ToString().PadLeft(4, '0')}][0x{card.ID.ToString("X").PadLeft(4, '0')}]");
                miniWriter.WriteLine(card.ImageName);

                n++;
            }
            while (idxWriter.BaseStream.Length < 8192) idxWriter.Write((uint)0x00);

        }
        private void LoadCards()
        {
            cards = [];

            using var idReader = new BinaryReader(new MemoryStream(binaries["card_id.bin"]));
            using var nameReader = new BinaryReader(new MemoryStream(binaries["card_nameeng.bin"]));
            using var descReader = new BinaryReader(new MemoryStream(binaries["card_desceng.bin"]));
            using var idxReader = new BinaryReader(new MemoryStream(binaries["card_indxeng.bin"]));
            using var propReader = new BinaryReader(new MemoryStream(binaries["card_prop.bin"]));
            using var packReader = new BinaryReader(new MemoryStream(binaries["card_pack.bin"]));

            idxReader.ReadBytes(4); // Skip the idx of the first description

            int i = 0;
            while (!idReader.BaseStream.Position.Equals(idReader.BaseStream.Length))
            {
                ushort id = idReader.ReadUInt16();
                string name = Encoding.UTF8.GetString(nameReader.ReadBytes(64)).TrimEnd('\0');
                uint nextIdx = idxReader.BaseStream.Position.Equals(idxReader.BaseStream.Length) ? (uint)descReader.BaseStream.Length - 1 : idxReader.ReadUInt32();
                string desc = nextIdx > descReader.BaseStream.Position ? Encoding.UTF8.GetString(descReader.ReadBytes((int)(nextIdx - descReader.BaseStream.Position))).TrimEnd('\0') : "";
                uint prop = propReader.ReadUInt32();
                ushort ver = packReader.ReadUInt16();

                Card card = new Card
                {
                    ID = id,
                    Name = name,
                    Description = desc,
                    PropertyBinary = prop,
                    ImageName = imgNames[i],
                    VersionBinary = ver
                };
                cards.Add(card);

                i++;
            }

        }
    }
}
