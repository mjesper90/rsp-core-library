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
    //Read the 54 byte header
    fread(&bmpHeader, sizeof(uint8_t), sizeof(bmpHeader), file);
    //std::cout << "File header read" << std::endl;

    mWidth = bmpHeader.width;
    mHeight = bmpHeader.heigth;

    //std::cout << "Width:            " << width << std::endl;
    //std::cout << "Height:           " << height << std::endl;

    //short bitsPerPixel;
    //memcpy(&bitsPerPixel, info + 28, sizeof(short));
    bytesPerPixel = bmpHeader.bitsPerPixel / 8; //Might be 1 or 4
    //std::cout << "BytesPerPixel Pre Calc: " << bytesPerPixel << std::endl;
    //std::cout << "Mod calc:               " << (bytesPerPixel % 8) << std::endl;
    if ((bmpHeader.bitsPerPixel % 8) > 0) {
        bytesPerPixel = bytesPerPixel + 1;
    }
    //std::cout << "Signature:        " << std::to_string(bmpHeader.signature) << std::endl;
    //std::cout << "FileSize:         " << bmpHeader.fileSize << std::endl;
    //std::cout << "Reserved:         " << bmpHeader.reserved << std::endl;
    //std::cout << "DataOffset:       " << bmpHeader.dataOffset << std::endl;
    //std::cout << "Size:             " << bmpHeader.size << std::endl;
    //std::cout << "Width:            " << bmpHeader.width << std::endl;
    //std::cout << "Height:           " << bmpHeader.height << std::endl;
    //std::cout << "Planes:           " << bmpHeader.planes << std::endl;
    //std::cout << "BitsPerPix:       " << bmpHeader.bitsPerPixel << std::endl;
    //std::cout << "Compression:      " << bmpHeader.compression << std::endl;
    //std::cout << "ImageSize:        " << bmpHeader.imageSize << std::endl;
    //std::cout << "xPixelsPerM:      " << bmpHeader.xPixelsPerM << std::endl;
    //std::cout << "yPixelsPerM:      " << bmpHeader.yPixelsPerM << std::endl;
    //std::cout << "ColoursUSed:      " << bmpHeader.coloursUsed << std::endl;
    //std::cout << "ImportantColours: " << bmpHeader.importantColours << std::endl;
    //std::cout << "BytesPerPix:      " << bytesPerPixel << std::endl;

    // TODO: Get Compression and other useful stuff

    //Height can be negative, showing the image is stored from top to bottom
    bool normallyDrawn = true;
    if (bmpHeader.heigth < 0) {
        normallyDrawn = false;
    }

    //Figure out amount to read
    int paddedRowSize = (bmpHeader.width * 3 + 3) & (~3); //bytesPerPixel * abs(bmpHeader.heigth) * bmpHeader.width;
    //std::cout << "Padded row size: " << paddedRowSize << std::endl;

    //Initialize containers for reading
    std::vector<uint8_t> pixelRow;
    pixelRow.resize(paddedRowSize);

    //Skip past the offset
    fseek(file, bmpHeader.dataOffset, SEEK_SET);

    for (size_t i = 0; i < abs(bmpHeader.heigth); i++) {
        //Read a Row of pixels with the padding
        fread(pixelRow.data(), sizeof(uint8_t), paddedRowSize, file);
        //std::cout << "Bitmap row " << i << " read into memory" << std::endl;
        for (size_t j = 0; j < bmpHeader.width * 3; j += 3) {
            uint8_t blue = pixelRow[j];      //*iter;
            uint8_t green = pixelRow[j + 1]; //*std::next(iter, 1);
            uint8_t red = pixelRow[j + 2];   //*std::next(iter, 2);
            uint8_t alpha = 0x00;

            uint32_t combined = (((uint32_t)red) << 24) |
                                (((uint32_t)green) << 16) |
                                (((uint32_t)blue) << 8) |
                                ((uint32_t)alpha);
            mImagePixels.push_back(combined);
            //std::cout << "Combined: " << std::hex << combined << std::endl;
        }
    }
    fclose(file);
    if (normallyDrawn) {
        std::reverse(mImagePixels.begin(), mImagePixels.end());
    }
    return mImagePixels;
}