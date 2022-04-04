/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2022 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */
#ifndef COMPONENTLOADER_H
#define COMPONENTLOADER_H

#include "graphics/primitives/Bitmap.h"
#include <map>
#include <sstream>

namespace rsp::graphics
{
class Component
{
  public:
    Component(Point aPlacement, Bitmap aBitmap)
        : mPlacement(aPlacement), mBitmap(aBitmap) {}
    ~Component() {}

    Point mPlacement;
    Bitmap mBitmap;
};

class ComponentLoader
{
  public:
    ComponentLoader(std::string aBitmapSource, std::string aSectioningInfo);
    ~ComponentLoader();

    Component &GetComponent(std::string aName);
    std::unordered_map<std::string, Component> mComponents{}; // protected

  protected:
    std::string mLine = "";
    std::string mCell = "";

    std::map<std::string, unsigned int> mColumnHeaders{};
};

} // namespace rsp::graphics

#endif // COMPONENTLOADER_H