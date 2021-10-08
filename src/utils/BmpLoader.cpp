/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */

#include <utils/BmpLoader.h>
#include <utils/RSPCoreExceptions.h>

#include <algorithm>
#include <cerrno>

std::vector<uint32_t> BmpLoader::LoadImg(const std::string &aImgName)
{
    //std::cout << "Bitmap reading file: " << aImgName << std::endl;
    errno = 0;
    //Pass reference to the first element in string, and read as binary
    FILE *file = fopen(aImgName.c_str(), "rb");
    if (file == NULL) {
        std::cout << "File is null" << std::endl;
        std::cout << "Error: " << errno << std::endl;
    }
    ReadHeader(file);
    // TODO: Get Compression and other useful stuff

    //Height can be negative, showing the image is stored from top to bottom
    bool normallyDrawn = true;
    if (bmpHeader.heigth < 0) {
        normallyDrawn = false;
    }
    ReadData(file);

    fclose(file);
    if (normallyDrawn) {
        std::reverse(mImagePixels.begin(), mImagePixels.end());
    }
    return mImagePixels;
}

void BmpLoader::ReadHeader(FILE *aFile)
{
    //Read the 54 byte header
    fread(&bmpHeader, sizeof(uint8_t), sizeof(bmpHeader), aFile);
    //std::cout << "File header read" << std::endl;

    mWidth = bmpHeader.width;
    mHeight = bmpHeader.heigth;

    bytesPerPixel = bmpHeader.bitsPerPixel / 8; //Might be 1 or 4
    if ((bmpHeader.bitsPerPixel % 8) > 0) {
        bytesPerPixel = bytesPerPixel + 1;
    }
}

void BmpLoader::ReadData(FILE *aFile)
{
    //Figure out amount to read
    int paddedRowSize = (bmpHeader.width * 3 + 3) & (~3);

    //Initialize containers for reading
    std::vector<uint8_t> pixelRow;
    pixelRow.resize(paddedRowSize);

    //Skip past the offset
    fseek(aFile, bmpHeader.dataOffset, SEEK_SET);

    for (size_t i = 0; i < abs(bmpHeader.heigth); i++) {
        //Read a Row of pixels with the padding
        fread(pixelRow.data(), sizeof(uint8_t), paddedRowSize, aFile);

        for (size_t j = bmpHeader.width * bytesPerPixel; j > 0; j -= bytesPerPixel) {
            uint32_t combined = ReadPixel(pixelRow, j);
            mImagePixels.push_back(combined);
        }
    }
}

uint32_t BmpLoader::ReadPixel(const std::vector<uint8_t> &aPixelRow, const size_t &aRowPtr)
{
    uint32_t pixel = 0;
    //Reads other direction than the row loop
    for (size_t i = 0; i < bytesPerPixel; i++) {
        pixel |= (((uint32_t)aPixelRow[aRowPtr + i]) << (8 * i));
    }
    return pixel;
}