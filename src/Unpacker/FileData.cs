using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace YGO_PoC_Toolset
{
    public class FileData
    {
        public string FileName { get; set; } = "";
        public uint FileSize { get; set; } = 0x0;
        public uint CompressedFileSize { get; set; } = 0x0;
        public uint FileOffset { get; set; } = 0x0;
        public string FileOffsetHex => $"{FileOffset:X}";
    }
}
