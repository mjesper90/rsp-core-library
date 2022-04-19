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
    // Arrange
    auto begin = std::chrono::high_resolution_clock::now();

    // Act
    ComponentLoader loader("testFiles/Test keybord - Sprite sheet.bmp",
                           "testFiles/GuiItemsPlacement.csv");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    // Assert
    CHECK(duration < 1000);
    CHECK(loader.GetAllComponents().size() == 206);
    MESSAGE("Duration: " << duration);

    SUBCASE("Components Contain pixels")
    {
        // Arrange
        int i = 0;
        int rand = std::rand() % (loader.GetAllComponents().size() + 1);

        // Act & Assert
        for (auto comp : loader.GetAllComponents()) {
            if (i == rand) {
                CHECK(comp.second.mBitmap.GetPixels().size() > 0);
            }
        }
        SUBCASE("Draw sectioned Components")
        {
            std::filesystem::path p = rsp::posix::FileSystem::GetCharacterDeviceByDriverName("vfb2", std::filesystem::path{"/dev/fb?"});
            Framebuffer fb(p.empty() ? nullptr : p.string().c_str());
            for (auto comp : loader.GetAllComponents()) {
                fb.DrawImage(comp.second.mPlacement, comp.second.mBitmap);
            }
            fb.SwapBuffer(BufferedCanvas::SwapOperations::Clear);
        }
    }
}