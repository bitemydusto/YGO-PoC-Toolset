using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PoCTools.Decoder
{
    public interface IPoCDecoder
    {
        public void DecodeBuffer(byte[] inputBuffer, int length, byte[] outputBuffer);
        public void EncodeBuffer(byte[] inputBuffer, int length, byte[] outputBuffer);
    }
}
