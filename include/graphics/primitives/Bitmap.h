#ifndef BITMAP_H
#define BITMAP_H

#include <utils/RSPCoreExceptions.h>

#include <cstring>
#include <iostream>
#include <vector>

//Removed
struct BMPHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t size;
    uint32_t width;
    uint32_t heigth;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t xPixelsPerM;
    uint32_t yPixelsPerM;
    uint32_t coloursUsed;
    uint32_t importantColours;
} __attribute__((packed));  //To stop alignment

class Bitmap {
   public:
    BMPHeader bmpHeader;     //Removed
    uint32_t height;         //Removed
    uint32_t width;          //Removed
    uint16_t bytesPerPixel;  //Removed
    std::string imgName;
    std::vector<uint32_t> imagePixels;  //Pointer?

    Bitmap();
    Bitmap(std::string aImgName);
    Bitmap(const uint32_t* apPixels, int aHeight, int aWidth, int aBytesPerPixel);
    Bitmap(int aHeight, int aWidth, int aBytesPerPixel);
    ~Bitmap();

    //Ask and return colour type on pixel
};

#endif  //BITMAP_H
