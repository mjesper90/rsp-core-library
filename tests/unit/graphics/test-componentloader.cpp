/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */

#include "graphics/ComponentLoader.h"
#include "graphics/Framebuffer.h"
#include <doctest.h>
#include <posix/FileSystem.h>

using namespace ::rsp::graphics;

TEST_CASE("Component Loader Test")
{
    ComponentLoader loader("testFiles/Test keybord - Sprite sheet.bmp",
                           "testFiles/GuiItemsPlacement.csv");

    std::filesystem::path p = rsp::posix::FileSystem::GetCharacterDeviceByDriverName("vfb2", std::filesystem::path{"/dev/fb?"});
    Framebuffer fb(p.empty() ? nullptr : p.string().c_str());

    // Can we draw the bitmaps?
    fb.DrawImage(Point(100, 100), loader.GetComponent("Keyboard_number"));
    fb.SwapBuffer(BufferedCanvas::SwapOperations::Clear);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}