
#include <util/image/png.h>

using namespace crystalos;
using namespace crystalos::common;
using namespace crystalos::util;
using namespace crystalos::util::images;

void print(char*);
void printHex8(uint8_t);
void printHex32(uint32_t);

PNG::PNG(uint8_t* buffer)
{
    char buf[12];
    PNGImage* img = (PNGImage*)buffer;
    int offset = 0;
    uint8_t* idatChunk = (uint8_t*)MemoryManager::ActiveMemoryManager->malloc(500*1024);

    img->ihdr.Length = bigE(img->ihdr.Length);
    itoa(img->ihdr.Length, 10, buf);
    print("IHDR Chunk lengh: ");
    print(buf);
    print("\n");

    if (img->ihdr.Length != 13) return; //Inavalid IHDR chunk length

    uint8_t* otherChunks = (uint8_t*)(buffer + 33);
    for (int i = 0; ;)
    {
        uint32_t chunkLength = bigE(((uint32_t*)(otherChunks + i))[0]);
        char* chunkType = (char*)(otherChunks + i + 4);
        
        if ((chunkType[0] == 'I') && (chunkType[1] == 'D') && (chunkType[2] == 'A') && (chunkType[3] == 'T') )
        {
            uint8_t* chunkData = (uint8_t*)(otherChunks + i + 8);
            MemoryManager::ActiveMemoryManager->memcpy(chunkData, (idatChunk + offset), chunkLength);
            offset += chunkLength;
        }

        if ((chunkType[0] == 'I') && (chunkType[1] == 'E') && (chunkType[2] == 'N') && (chunkType[3] == 'D') ) break;
        i += 12 + chunkLength;
    }

    if (offset == 0) return;
    print("Decompressing...\n");

    uint8_t* data = (uint8_t*)(idatChunk + 2);
    for (int i = 0; ;)
    {
        int index = 0;
        uint8_t byt0 = ((uint8_t*)(data + i))[index];
        uint8_t byt = byt0;

        if (((byt0 >> 1) & 0x03) == 0)
        {
            print("The data is uncompressed\n");
        }
        else if (((byt0 >> 1) & 0x03) == 1)
        {
            print("The data is compressed with fixed huffman codes\n");
        }
        else if (((byt0 >> 1) & 0x03) == 2)
        {
            print("The data is compressed with dynamic huffman codes\n");
            uint8_t literals = (byt >> 3) & 0x1F;
            index++;
            byt = ((uint8_t*)(data + i))[index];
            uint8_t distances = byt & 0x1F;
            uint8_t lengths = (byt >> 5) & 0x7;
            index++;
            byt = ((uint8_t*)(data + i))[index];
            lengths = ((byt & 0x1) << 3) | lengths;
            printHex8(literals);
            printHex8(distances);
            printHex8(lengths);
            print("\n");

            uint8_t bf1 = (byt >> 1) & 0x7;
            uint8_t bf2 = (byt >> 4) & 0x7;
            uint8_t bf3 = ((byt >> 7) & 0x1) << 2;
            index++;
            byt = ((uint8_t*)(data + i))[index];

            bf3 |= (byt & 0x03);
            uint8_t bf4 = ((byt >> 2) & 0x7);
            uint8_t bf5 = ((byt >> 5) & 0x7);
            index++;
            byt = ((uint8_t*)(data + i))[index];

            uint8_t bf6 = (byt & 0x07);
            uint8_t bf7 = (byt >> 3) & 0x07;
            uint8_t bf8 = ((byt >> 6) & 0x03) << 1;
            index++;
            byt = ((uint8_t*)(data + i))[index];
            
            bf8 |= byt & 0x01;
            uint8_t bf9 = (byt >> 1) & 0x7;
            uint8_t bf10 = (byt >> 4) & 0x7;
            uint8_t bf11 = ((byt >> 7) & 0x1) << 2;
            index++;
            byt = ((uint8_t*)(data + i))[index];

            bf11 |= byt & 0x03;
            uint8_t bf12 = (byt >> 2) & 0x7;
            uint8_t bf13 = (byt >> 5) & 0x7;
            index++;
            byt = ((uint8_t*)(data + i))[index];

            uint8_t bf14 = (byt & 0x07);
            uint8_t bf15 = (byt >> 3) & 0x07;
            uint8_t bf16 = ((byt >> 6) & 0x03) << 1;
            index++;
            byt = ((uint8_t*)(data + i))[index];

            bf16 |= byt & 0x01;
            uint8_t bf17 = (byt >> 1) & 0x7;
            uint8_t bf18 = (byt >> 4) & 0x7;
            uint8_t bf19 = ((byt >> 7) & 0x1) << 2;
            index++;
            byt = ((uint8_t*)(data + i))[index];
            
            bf19 |= byt & 0x03;

            uint8_t bfs[19] = {bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19};
            for (int j = 0; j < lengths+4; j++)
                printHex8(bfs[j]);
            
            
        }
        else
        {
            print("Invalid data..\n");
            break;
        }
        if (byt0 & 0x1 == 1)
        {
            print("This is the final block\n");
            break;
        }        
    }
}
PNG::~PNG()
{

}

void PNG::CrcByte(uint8_t data)
{
    uint32_t index = (CrcRegister ^ data) & 0xFF;
    CrcRegister = CrcTable[index] ^ ((CrcRegister >> 8) & 0x00FFFFFF);
    printHex32(CrcRegister);
}

unsigned long PNG::CRC(uint8_t* buffer, uint32_t length)
{
    CrcRegister = 0xffffffffl;
    for (int i = 0; i < length; i++) CrcByte(buffer[i]);
    return ~CrcRegister;
}

void PNG::MakeCrcTable()
{
    for (int i = 0; i < 256; i++)
    {
        CrcTable[i] = i;
        for (int j = 0; j < 8; j++)
        {
            if ((CrcTable[i] & 0x1) == 0) CrcTable[i] >>= 1;
            else CrcTable[i] = 0xEDB88320l ^ (CrcTable[i] >> 1);
        }
    }
}
