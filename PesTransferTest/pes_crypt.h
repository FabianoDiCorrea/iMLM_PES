// pes_crypt.h
#ifndef PES_CRYPT_H
#define PES_CRYPT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MASTER_KEY_LENGTH 64

extern const uint8_t MasterKeyPes21[MASTER_KEY_LENGTH];
extern const uint8_t MasterKeyPes20[MASTER_KEY_LENGTH];

struct FileHeader
{
    uint8_t mysteryData[64];
    uint32_t dataSize;
    uint32_t logoSize;
    uint32_t descSize;
    uint32_t serialLength;
    uint8_t hash[64];
    uint8_t fileTypeString[32];
    uint8_t gameVersionString[32];
};

struct FileDescriptor
{
    uint8_t *encryptionHeader;
    struct FileHeader *fileHeader;

    uint8_t *description;
    uint8_t *logo;
    uint8_t *data;
    uint8_t *serial;
};

struct FileDescriptor *createFileDescriptor();
void destroyFileDescriptor(struct FileDescriptor *desc);

uint8_t *readFile(const char *path, uint32_t *sizePtr);
void writeFile(const char *path, const uint8_t *data, int size);

void decryptWithKey(struct FileDescriptor *descriptor, const uint8_t *input, const uint8_t *masterKey);
uint8_t *encryptWithKey(const struct FileDescriptor *descriptor, int *size, const uint8_t *masterKey);

#endif
