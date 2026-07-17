namespace PoCTools.Decoder
{
    public class PoCDecoder: IPoCDecoder
    {
        // FUN_005be170
        ushort GetCollectionChecksum(byte[] buffer, int offset, int length)
        {
            ushort sum = 0;
            for (int i = offset; i < offset + length; i += 2)
            {
                ushort word = BitConverter.ToUInt16(buffer, i);
                sum += word;
            }
            return unchecked((ushort)(0 - sum));
        }
        // FUN_0043a3a0
        public void DecodeBuffer(byte[] inputBuffer, int length, byte[] outputBuffer)
        {
            bool isValid = GenerateSpecialChecksum(inputBuffer, length).AsSpan().SequenceEqual(inputBuffer.AsSpan(0, 4));

            if (isValid)
            {
                DecryptPayload(inputBuffer, 4, length, outputBuffer, 0x35, 299);
            }
        }

        // FUN_0043a130
        void DecryptPayload(byte[] inputBuffer, int offset, int length, byte[] outputBuffer, uint exponent, uint modulus)
        {
            uint uVar2 = 0;
            ushort maskedLength = (ushort)(length & 0xffff);
            int outIdx = 0;

            if (maskedLength != 0)
            {
                do
                {
                    uint uVar4 = (uVar2 * 8 + 8 < maskedLength)
                        ? 8U
                        : (uint)(maskedLength - (uVar2 * 8));

                    if (uVar4 != 0)
                    {
                        byte bVar3 = 0;
                        // Every loop iteration reads from an offset stepping through blocks
                        int baseByteOffset = offset + (int)uVar2 * 9;

                        do
                        {
                            // Safely pull a 16-bit window out of the buffer to extract 9 bits from
                            int currentBytePos = baseByteOffset + bVar3;
                            ushort bitsWindow = 0;
                            if (currentBytePos < inputBuffer.Length)
                                bitsWindow = inputBuffer[currentBytePos];
                            if (currentBytePos + 1 < inputBuffer.Length)
                                bitsWindow |= (ushort)(inputBuffer[currentBytePos + 1] << 8);

                            // Extract the sliding 9-bit symbol
                            uint extractedSymbol = (uint)((bitsWindow & (0x1FF << (bVar3 & 0x1F))) >> (bVar3 & 0x1F));

                            // Decrypt the symbol using Modular Exponentiation
                            byte decryptedByte = (byte)ModularExponentiation(extractedSymbol, exponent, modulus);

                            if (outIdx < outputBuffer.Length)
                            {
                                outputBuffer[outIdx++] = decryptedByte;
                            }

                            bVar3++;
                            uVar4--;
                        } while (uVar4 != 0);
                    }
                    uVar2 = (uVar2 + 1) & 0xffff;
                } while (uVar2 * 8 < maskedLength);
            }
        }

        // FUN_00439fb0
        uint ModularExponentiation(uint baseVal, uint exponent, uint modulus)
        {
            ushort uVar4 = (ushort)exponent;
            if (uVar4 == 0) return 1;

            // Find the highest bit set in the exponent
            int iVar1 = 0x1f;
            do
            {
                iVar1--;
            } while (((uVar4 >> (iVar1 & 0x1f)) & 1) == 0);

            uint uVar3 = 1;
            if (iVar1 < 0) return exponent;

            // Process bits from MSB to LSB (Square and Multiply)
            do
            {
                uint uVar2 = (((uVar4 >> (iVar1 & 0x1f)) & 1) == 1)
                    ? (baseVal & 0xffff)
                    : 1U;

                uVar3 = (uVar2 * uVar3 * uVar3) % (modulus & 0xffff);
                iVar1--;
            } while (-1 < iVar1);

            return uVar3;
        }
        // FUN_0043a360
        public void EncodeBuffer(byte[] inputBuffer, int length, byte[] outputBuffer)
        {
            ushort collectionChecksum = GetCollectionChecksum(inputBuffer, 8, 0x1184);
            inputBuffer[0x118C] = (byte)(collectionChecksum & 0xFF);
            inputBuffer[0x118D] = (byte)((collectionChecksum >> 8) & 0xFF);

            EncryptPayload(inputBuffer, length, outputBuffer, 4, 5, 299);

            var specialChecksum = GenerateSpecialChecksum(outputBuffer, length);

            if(specialChecksum != null)
            {
                outputBuffer[0] = specialChecksum[0];
                outputBuffer[1] = specialChecksum[1];
                outputBuffer[2] = specialChecksum[2];
                outputBuffer[3] = specialChecksum[3];
            }
        }

        // FUN_0043a040
        void EncryptPayload(byte[] inputBuffer, int length, byte[] outputBuffer, int outOffset, uint exponent, uint modulus)
        {
            uint uVar2 = 0;
            ushort maskedLength = (ushort)(length & 0xffff);

            if (maskedLength != 0)
            {
                do
                {
                    // Determine how many bytes to read in this block (up to 8)
                    byte bVar4 = (uVar2 * 8 + 8 < maskedLength)
                        ? (byte)8
                        : (byte)(maskedLength - (uVar2 * 8));

                    int baseByteOffset = outOffset + (int)uVar2 * 9;

                    // Zero out the 9-byte destination block
                    for (int i = 0; i < 9; i++)
                    {
                        if (baseByteOffset + i < outputBuffer.Length)
                        {
                            outputBuffer[baseByteOffset + i] = 0;
                        }
                    }

                    uint uVar5 = bVar4;
                    if (bVar4 != 0)
                    {
                        byte bitShiftCounter = 0;
                        int inIdx = (int)uVar2 * 8;

                        do
                        {
                            byte rawByte = inputBuffer[inIdx++];

                            // Encrypt the 8-bit byte into a 9-bit symbol using Modular Exponentiation
                            uint encryptedSymbol = ModularExponentiation(rawByte, exponent, modulus);

                            // Shift the 9-bit symbol into its position in the bitstream
                            uint shiftedSymbol = encryptedSymbol << (bitShiftCounter & 0x1F);

                            // Pack the symbol across the current byte and the next adjacent byte
                            int currentBytePos = baseByteOffset + bitShiftCounter;
                            if (currentBytePos < outputBuffer.Length)
                            {
                                outputBuffer[currentBytePos] |= (byte)shiftedSymbol;
                            }
                            if (currentBytePos + 1 < outputBuffer.Length)
                            {
                                outputBuffer[currentBytePos + 1] |= (byte)(shiftedSymbol >> 8);
                            }

                            bitShiftCounter++;
                            uVar5--;
                        } while (uVar5 != 0);
                    }

                    uVar2 = (uVar2 + 1) & 0xffff;
                } while (uVar2 * 8 < maskedLength);
            }
        }

        // FUN_0043a200
        byte[]? GenerateSpecialChecksum(byte[] buffer, int length)
        {
            if (buffer == null || length < 4) return null;

            // Magic bytes
            byte ch0 = 0x7D;
            byte ch1 = 0x13;
            byte ch2 = 0x8A;
            byte ch3 = 0xBB;

            // Accumulate payload bytes into their corresponding positional channels
            for (int i = 4; i < length; i++)
            {
                int channel = (i - 4) % 4; // Cycles through 0, 1, 2, 3

                switch (channel)
                {
                    case 0: ch0 = (byte)(ch0 + buffer[i]); break;
                    case 1: ch1 = (byte)(ch1 + buffer[i]); break;
                    case 2: ch2 = (byte)(ch2 + buffer[i]); break;
                    case 3: ch3 = (byte)(ch3 + buffer[i]); break;
                }
            }

            return new byte[] { (byte)(~ch0 + 1), (byte)(~ch1 + 1), (byte)(~ch2 + 1), (byte)(~ch3 + 1) };
        }

    }
}
