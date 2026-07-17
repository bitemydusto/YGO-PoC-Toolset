using System.Text;
using System.Text.Json;

namespace YGO_PoC_Toolset
{
    internal class Program
    {
        static void Main(string[] args)
        {
            List<FileData> files = [];
            string filePath = "";

            if (args.Length > 0)
            {
                filePath = args[0];
                if (!File.Exists(filePath))
                {
                    Console.WriteLine("File does not exist!\nPress any key to exit...");
                    Console.ReadKey();
                    return;
                }
            }
            else
            {
                Console.WriteLine("Program opened without argument! Drag a your packed binary file into the .exe!\nPress any key to exit...");
                Console.ReadKey();
                return;
            }

            try
            {
                files = GetFileInfos(filePath);
                if (files.Count == 0) return;
            }
            catch { Console.WriteLine("Error: Failed to read file infos!"); }
            try
            {
                WriteFileInfo(files, $"{Path.GetFileNameWithoutExtension(filePath)}_info.json");
            }
            catch { Console.WriteLine("Error: Failed to write file infos!"); }
            try
            {
                UnpackFiles(files, filePath);
            }
            catch { Console.WriteLine("Error: Failed to unpack files!"); }


            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }
        static void UnpackFiles(List<FileData> files, string filePath)
        {
            Console.WriteLine($"Unpacking {Path.GetFileName(filePath)}...");

            foreach (FileData file in files)
            {
                // Read file data
                var reader = new BinaryReader(new FileStream(filePath, FileMode.Open, FileAccess.Read));
                reader.BaseStream.Position = file.FileOffset;
                byte[] bytes = reader.ReadBytes((int)file.FileSize);
                reader.Close();


                // LZSS decompression
                if (file.FileSize != file.CompressedFileSize)
                {
                    bytes = Decompress(bytes, file.FileSize);
                }

                // YPK1 decoding
                if (IsYPK1(bytes))
                {
                    // Not sure what uses it, but it's in the original code so I kept it
                    bytes = DecodeYPK1(bytes);
                }

                string outPath = Path.Combine(Path.GetFileNameWithoutExtension(filePath), file.FileName);
                string? outDir = Path.GetDirectoryName(outPath);
                if (!string.IsNullOrEmpty(outDir)) Directory.CreateDirectory(outDir);
                File.WriteAllBytes(outPath, bytes);
            }

            Console.WriteLine($"Done! Unpacked {files.Count} files!");
        }
        static List<FileData> GetFileInfos(string path)
        {
            List<FileData> files = [];

            BinaryReader reader = new BinaryReader(new FileStream(path, FileMode.Open));

            if (reader.BaseStream.Length < 12) { Console.WriteLine("Error: File is too small! Press any key to exit..."); return files; }

            byte[] header = reader.ReadBytes(8);
            string strHeader = Encoding.ASCII.GetString(header);
            if (strHeader != "KCEJYUGI") { Console.WriteLine("Error: Invalid file header! Press any key to exit..."); return files; }

            uint fileCount = reader.ReadUInt32();
            
            for(uint i = 0; i < fileCount; i++)
            {
                byte[] fileNameBytes = reader.ReadBytes(256);
                uint fileOffset = reader.ReadUInt32();
                uint fileSize = reader.ReadUInt32();
                uint cmpFileSize = reader.ReadUInt32();

                // Decrypt filename
                for (int j = 0; j < fileNameBytes.Length; j++)
                {
                    byte b = fileNameBytes[j];
                    fileNameBytes[j] = (byte)((b >> 4) | (b << 4));
                }

                files.Add(new FileData
                {
                    FileName = Encoding.ASCII.GetString(fileNameBytes).TrimEnd('\0'),
                    FileOffset = fileOffset,
                    FileSize = fileSize,
                    CompressedFileSize = cmpFileSize
                });
            }
            reader.Close();
            return files;
        }
        static void WriteFileInfo(List<FileData> files, string outputPath)
        {
            string json = JsonSerializer.Serialize(files, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(outputPath, json);
        }
        public static byte[] Decompress(byte[] input, uint outputSize)
        {
            // LZSS decompression algorithm
            byte[] output = new byte[outputSize];
            byte[] dictionary = new byte[4096];

            int inputPos = 0;
            int outputPos = 0;

            int dictPos = 0xFEE;
            int flags = 0;

            while (outputPos < outputSize)
            {
                flags >>= 1;

                if ((flags & 0x100) == 0)
                {
                    flags = 0xFF00 | input[inputPos++];
                }

                if ((flags & 1) != 0)
                {
                    // Literal
                    byte b = input[inputPos++];

                    output[outputPos++] = b;

                    dictionary[dictPos] = b;
                    dictPos = (dictPos + 1) & 0xFFF;
                }
                else
                {
                    // Back-reference
                    byte b1 = input[inputPos++];
                    byte b2 = input[inputPos++];

                    int offset = b1 | ((b2 & 0xF0) << 4);
                    int length = (b2 & 0x0F) + 3;

                    for (int i = 0; i < length; i++)
                    {
                        byte b = dictionary[offset];

                        output[outputPos++] = b;

                        dictionary[dictPos] = b;

                        dictPos = (dictPos + 1) & 0xFFF;
                        offset = (offset + 1) & 0xFFF;

                        if (outputPos >= outputSize)
                            break;
                    }
                }
            }

            return output;
        }
        static bool IsYPK1(byte[] data)
        {
            return data.Length >= 4 &&
                   data[0] == (byte)'Y' &&
                   data[1] == (byte)'P' &&
                   data[2] == (byte)'K' &&
                   data[3] == (byte)'1';
        }
        static byte[] DecodeYPK1(byte[] data)
        {
            byte[] result = new byte[data.Length - 4];

            for (int i = 4; i < data.Length; i++)
            {
                byte b = data[i];

                b = (byte)((b >> 4) | (b << 4)); // swap nibbles
                b ^= 0xCC;

                result[i - 4] = b;
            }

            return result;
        }
    }
}
