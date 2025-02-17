/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */

#include "graphics/Framebuffer.h"

#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>
#include <utils/ExceptionHelper.h>

namespace rsp::graphics
{

Framebuffer::Framebuffer(const char *apDevPath)
    : mFramebufferFile(-1)
{
    if (apDevPath) {
        mFramebufferFile = open(apDevPath, O_RDWR);
        if (mFramebufferFile == -1) {
            std::clog << "Failed to open framebuffer at " << apDevPath << " trying /dev/fb0" << std::endl;
        }
    }
    if (mFramebufferFile == -1) {
        mFramebufferFile = open("/dev/fb0", O_RDWR);
        if (mFramebufferFile == -1) {
            THROW_SYSTEM("Failed to open framebuffer");
        }
    }

    // get fixed screen info
    ioctl(mFramebufferFile, FBIOGET_VSCREENINFO, &mVariableInfo);

    // get variable screen info
    ioctl(mFramebufferFile, FBIOGET_FSCREENINFO, &mFixedInfo);

    // set Canvas specific variables
    mWidth = static_cast<int>(mVariableInfo.xres);
    mHeight = static_cast<int>(mVariableInfo.yres);
    mBytesPerPixel = static_cast<int>(mVariableInfo.bits_per_pixel / 8);
    // std::clog << "Framebuffer opened. Width=" << mWidth << " Height=" << mHeight << " BytesPerPixel=" << mBytesPerPixel << std::endl;

    // set yres_virtual for double buffering
    mVariableInfo.yres_virtual = mVariableInfo.yres * 2;
    if (ioctl(mFramebufferFile, FBIOPUT_VSCREENINFO, &mVariableInfo) == -1) {
        THROW_SYSTEM("Framebuffer ioctl FBIOPUT_VSCREENINFO failed");
    }

    // stop the console from drawing ontop of this programs graphics
    if (access("/dev/tty0", O_RDWR) == 0) {
        mTtyFb = open("/dev/tty0", O_RDWR);
        if (ioctl(mTtyFb, KDSETMODE, KD_GRAPHICS) == -1) {
            THROW_SYSTEM("Framebuffer ioctl KDSETMODE KD_GRAPHICS failed");
        }
    }

    // calculate size of screen
    std::size_t screensize = mVariableInfo.yres * mFixedInfo.line_length;

    mpFrontBuffer = static_cast<uint32_t *>(mmap(0, screensize * 2, PROT_READ | PROT_WRITE, MAP_SHARED, mFramebufferFile, static_cast<off_t>(0)));
    if (mpFrontBuffer == reinterpret_cast<uint32_t *>(-1)) /*MAP_FAILED*/ {
        THROW_SYSTEM("Framebuffer shared memory mapping failed");
    }

    mpBackBuffer = mpFrontBuffer + screensize / sizeof(std::uint32_t);

    if (mVariableInfo.yoffset > 0) {
        std::uint32_t *tmp = mpFrontBuffer;
        mpFrontBuffer = mpBackBuffer;
        mpBackBuffer = tmp;
    }
}

Framebuffer::~Framebuffer()
{
    // At exit we MUST release the tty again
    if (mTtyFb > 0) {
        if (ioctl(mTtyFb, KDSETMODE, KD_TEXT) == -1) {
            std::cout << "ioctl KDSETMODE KD_TEXT failed errno:" << strerror(errno) << std::endl;
        }
    }
    // Close the handle to the framebuffer device
    if (mFramebufferFile > 0) {
        close(mFramebufferFile);
    }
    // No need to call munmap on the shared memory region, this is done automatically on termination.
}

void Framebuffer::SwapBuffer(const SwapOperations aSwapOp, Color aColor)
{
    // swap buffer
    if (mVariableInfo.yoffset == 0) {
        mVariableInfo.yoffset = mVariableInfo.yres;
    } else {
        mVariableInfo.yoffset = 0;
    }
    // Pan to back buffer
    if (ioctl(mFramebufferFile, FBIOPAN_DISPLAY, &mVariableInfo) == -1) {
        std::cout << "ioctl FBIOPAN_DISPLAY failed errno:" << strerror(errno) << std::endl;
    }

    // update pointers
    std::uint32_t *tmp = mpFrontBuffer;
    mpFrontBuffer = mpBackBuffer;
    mpBackBuffer = tmp;

    switch (aSwapOp) {
    case SwapOperations::Copy:
        copy();
        break;

    case SwapOperations::Clear:
        clear(aColor);
        break;

    case SwapOperations::NoOp:
    default:
        break;
    }
}

uint32_t Framebuffer::GetPixel(const Point &aPoint, const bool aFront) const
{
    if (!IsInsideScreen(aPoint)) {
        return 0;
    }
    std::uint32_t location = ((static_cast<std::uint32_t>(aPoint.mX) + mVariableInfo.xoffset) * (mVariableInfo.bits_per_pixel / 8)
        + (static_cast<std::uint32_t>(aPoint.mY) * mFixedInfo.line_length)) / sizeof(std::uint32_t);
    if (aFront) {
        return mpFrontBuffer[location];
    } else {
        return mpBackBuffer[location];
    }
}

void Framebuffer::clear(Color aColor)
{
    // draw to back buffer
    for (std::uint32_t y = 0; y < mVariableInfo.yres; y++) {
        for (std::uint32_t x = 0; x < mVariableInfo.xres; x++) {
            std::size_t location = ((x + mVariableInfo.xoffset) * (mVariableInfo.bits_per_pixel / 8) + y * mFixedInfo.line_length) / sizeof(std::uint32_t);
            mpBackBuffer[location] = aColor;
        }
    }
}

void Framebuffer::copy()
{
    // copy front buffer to back buffer
    for (std::uint32_t y = 0; y < mVariableInfo.yres; y++) {
        for (std::uint32_t x = 0; x < mVariableInfo.xres; x++) {
            std::size_t location = ((x + mVariableInfo.xoffset) * (mVariableInfo.bits_per_pixel / 8) + y * mFixedInfo.line_length) / sizeof(std::uint32_t);
            mpBackBuffer[location] = mpFrontBuffer[location];
        }
    }
}

} // namespace rsp::graphics
