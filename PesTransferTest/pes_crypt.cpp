// pes_crypt.cpp
#include "pes_crypt.h"
#include "mt19937ar.h"
#include <string.h>
#include <sys/stat.h>

#define ENCRYPTION_HEADER_SIZE 320

const uint8_t MasterKeyPes20[MASTER_KEY_LENGTH] = {
    0xE2, 0xBF, 0x51, 0x07, 0x54, 0xE6, 0x21, 0x78,
    0x2C, 0x5E, 0x8D, 0x33, 0x13, 0x7A, 0xC9, 0x15,
    0x99, 0x77, 0xD9, 0xA0, 0x1B, 0xC2, 0x95, 0xD9,
    0xBB, 0x9B, 0xB1, 0x00, 0x84, 0x1C, 0xB3, 0x62,
    0xE5, 0x40, 0xD9, 0x56, 0x45, 0x5B, 0x7C, 0x7C,
    0x4F, 0xF1, 0xDA, 0x26, 0xB4, 0x5A, 0x0C, 0x5C,
    0x4D, 0x6B, 0x9E, 0x98, 0x75, 0xA9, 0x39, 0x07,
    0x4C, 0x4B, 0x55, 0xBD, 0x8E, 0x01, 0xA9, 0x31,
};

const uint8_t MasterKeyPes21[MASTER_KEY_LENGTH] = {
    0x90, 0x61, 0xD8, 0x66, 0x43, 0x77, 0x24, 0xF8,
    0x92, 0xBA, 0xB8, 0x71, 0x21, 0xC7, 0x60, 0x63,
    0xF0, 0x91, 0x9A, 0x7D, 0xED, 0x47, 0x80, 0xDE,
    0x51, 0xF5, 0xDD, 0xD1, 0x08, 0xFE, 0x32, 0x84,
    0xF5, 0x09, 0x92, 0x00, 0xB2, 0x3E, 0x88, 0x9F,
    0xEB, 0x24, 0x43, 0x05, 0x58, 0x76, 0x00, 0x22,
    0x9B, 0xFE, 0xEC, 0xF6, 0x50, 0x00, 0x29, 0xD3,
    0x42, 0x75, 0x50, 0xB9, 0xEC, 0xD2, 0xF6, 0x75,
};

static uint32_t rol(uint32_t a, uint32_t shift)
{
    return (a << shift) | (a >> (32 - shift));
}

static uint32_t ror(uint32_t a, uint32_t shift)
{
    return (a >> shift) | (a << (32 - shift));
}

static void xorRepeatingBlocks(uint8_t *output, const uint8_t *input, int length)
{
    for (int i = 0; i < length; ++i)
        output[i & 63] ^= input[i];
}

static void xorWithLongParam(const uint8_t *input, uint8_t *output, uint64_t param)
{
    const uint64_t * input64 = (const uint64_t *)input;
          uint64_t *output64 = (uint64_t *)output;

    for (int i = 0; i < 8; ++i)
        output64[i] = input64[i] ^ param;
}

static void reverseLongs(uint8_t *output, const uint8_t *input)
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            output[i*8 + j] = input[i*8 + 7 - j];
}

static void cryptStream(uint8_t *output, const uint8_t *key, const uint8_t *input, int length)
{
    const uint32_t *input32 = (const uint32_t *)input;
    uint32_t *output32 = (uint32_t *)output;

    init_by_array((uint32_t *)key, 16);
    uint32_t c0 = genrand_int32();
    uint32_t c1 = genrand_int32();
    uint32_t c2 = genrand_int32();
    uint32_t c3 = genrand_int32();

    for (int i = 0; i < length/4; ++i) {
        uint32_t c4 = genrand_int32();

        output32[i] = c4 ^ c3 ^ c2 ^ c1 ^ c0 ^ input32[i];

        c0 = ror(c1, 15);
        c1 = rol(c2, 11);
        c2 = rol(c3, 7);
        c3 = ror(c4, 13);
    }
    if (length & 3) {
        uint32_t rest;
        memcpy(&rest, &input[length & (~3)], length & 3);

        rest ^= genrand_int32() ^ c3 ^ c2 ^ c1 ^ c0;

        memcpy(&output[length & (~3)], &rest, length & 3);
    }
}

static void cryptHeader(uint8_t *output, const uint8_t *input, const uint8_t *key)
{
    uint8_t headerKey[64], shuffledMasterKey[64];

    memcpy(headerKey, &input[256], 64);
    reverseLongs(shuffledMasterKey, key);
    xorRepeatingBlocks(headerKey, shuffledMasterKey, 64);
    cryptStream(output, headerKey, input, ENCRYPTION_HEADER_SIZE);
    memcpy(&output[256], &input[256], 64);
}

void decryptWithKey(struct FileDescriptor *descriptor, const uint8_t *input, const uint8_t *masterKey)
{
    descriptor->encryptionHeader = (uint8_t *)malloc(ENCRYPTION_HEADER_SIZE);
    descriptor->fileHeader       = (struct FileHeader *)malloc(sizeof(struct FileHeader));

    cryptHeader(descriptor->encryptionHeader, input, masterKey);
    input += ENCRYPTION_HEADER_SIZE;

    uint8_t rollingKey[64], intermediateKey[64];
    memcpy(rollingKey, descriptor->encryptionHeader, 64);
    xorRepeatingBlocks(rollingKey, &descriptor->encryptionHeader[64], 256);

    xorWithLongParam(rollingKey, intermediateKey, sizeof(struct FileHeader));
    cryptStream((uint8_t *)descriptor->fileHeader, intermediateKey, input, sizeof(struct FileHeader));
    input += sizeof(struct FileHeader);

    descriptor->data        = (uint8_t *)malloc(descriptor->fileHeader->dataSize);
    descriptor->logo        = (uint8_t *)malloc(descriptor->fileHeader->logoSize);
    descriptor->description = (uint8_t *)malloc(descriptor->fileHeader->descSize);
    descriptor->serial      = (uint8_t *)malloc(descriptor->fileHeader->serialLength*2);

    xorWithLongParam(rollingKey, intermediateKey, 0);
    cryptStream(descriptor->description, intermediateKey, input, descriptor->fileHeader->descSize);
    input += descriptor->fileHeader->descSize;

    xorWithLongParam(rollingKey, intermediateKey, 1);
    cryptStream(descriptor->logo, intermediateKey, input, descriptor->fileHeader->logoSize);
    input += descriptor->fileHeader->logoSize;

    xorWithLongParam(rollingKey, intermediateKey, 2);
    cryptStream(descriptor->data, intermediateKey, input, descriptor->fileHeader->dataSize);
    input += descriptor->fileHeader->dataSize;

    xorWithLongParam(rollingKey, intermediateKey, 3);
    cryptStream(descriptor->serial, intermediateKey, input, descriptor->fileHeader->serialLength*2);
}

uint8_t *encryptWithKey(const struct FileDescriptor *descriptor, int *size, const uint8_t *masterKey)
{
    *size = ENCRYPTION_HEADER_SIZE
          + sizeof(struct FileHeader)
          + descriptor->fileHeader->dataSize
          + descriptor->fileHeader->logoSize
          + descriptor->fileHeader->descSize
          + descriptor->fileHeader->serialLength*2;

    uint8_t *result = (uint8_t *)malloc(*size);
    if (!result)
        return NULL;

    uint8_t *output = result;

    cryptHeader(output, descriptor->encryptionHeader, masterKey);
    output += ENCRYPTION_HEADER_SIZE;

    uint8_t rollingKey[64], intermediateKey[64];
    memcpy(rollingKey, descriptor->encryptionHeader, 64);
    xorRepeatingBlocks(rollingKey, &descriptor->encryptionHeader[64], 256);

    xorWithLongParam(rollingKey, intermediateKey, sizeof(struct FileHeader));
    cryptStream(output, intermediateKey, (uint8_t *)descriptor->fileHeader, sizeof(struct FileHeader));
    output += sizeof(struct FileHeader);

    xorWithLongParam(rollingKey, intermediateKey, 0);
    cryptStream(output, intermediateKey, descriptor->description, descriptor->fileHeader->descSize);
    output += descriptor->fileHeader->descSize;

    xorWithLongParam(rollingKey, intermediateKey, 1);
    cryptStream(output, intermediateKey, descriptor->logo, descriptor->fileHeader->logoSize);
    output += descriptor->fileHeader->logoSize;

    xorWithLongParam(rollingKey, intermediateKey, 2);
    cryptStream(output, intermediateKey, descriptor->data, descriptor->fileHeader->dataSize);
    output += descriptor->fileHeader->dataSize;

    xorWithLongParam(rollingKey, intermediateKey, 3);
    cryptStream(output, intermediateKey, descriptor->serial, descriptor->fileHeader->serialLength*2);

    return result;
}

struct FileDescriptor *createFileDescriptor()
{
    struct FileDescriptor *result = (struct FileDescriptor *)malloc(sizeof(struct FileDescriptor));
    if (result)
        memset(result, 0, sizeof(struct FileDescriptor));
    return result;
}

void destroyFileDescriptor(struct FileDescriptor *desc)
{
    if (!desc) return;
    if (desc->encryptionHeader) free(desc->encryptionHeader);
    if (desc->fileHeader)       free(desc->fileHeader);
    if (desc->description)      free(desc->description);
    if (desc->logo)             free(desc->logo);
    if (desc->data)             free(desc->data);
    if (desc->serial)           free(desc->serial);
    free(desc);
}

uint8_t *readFile(const char *path, uint32_t *sizePtr)
{
    FILE *inStream = fopen(path, "rb");
    if (!inStream)
        return NULL;

    struct stat file;
    if (stat(path, &file)) {
        fclose(inStream);
        return NULL;
    }
    int size = file.st_size;

    uint8_t *input = (uint8_t *)malloc(size);
    if (!input) {
        fclose(inStream);
        return NULL;
    }
    fread(input, 1, size, inStream);
    fclose(inStream);

    if (sizePtr)
        *sizePtr = size;

    return input;
}

void writeFile(const char *path, const uint8_t *data, int size)
{
    FILE *outStream = fopen(path, "wb");
    if (!outStream)
        return;
    fwrite(data, 1, size, outStream);
    fclose(outStream);
}
