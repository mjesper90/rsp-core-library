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

    Point mPlacement; //!< Location the component is to be placed on screen
    Bitmap mBitmap;   //!< Bitmap containing the pixel values to be drawn
};

class ComponentLoader
{
  public:
    ComponentLoader(std::string aBitmapSource, std::string aSectioningInfo);
    ~ComponentLoader();

    /**
     * \brief Gets the component with the given name
     * \param aName String used as keys to retrieve component
     * \return A reference to the component stored in member map
     */
    Component &GetComponent(std::string aName);

    /**
     * \brief Gets all the component
     * \return A reference to member map containing all components
     */
    std::unordered_map<std::string, Component> &GetAllComponents();

  protected:
    std::string mLine = "";
    std::string mCell = "";

    std::unordered_map<std::string, Component> mComponents{}; // protected
    std::map<std::string, unsigned int> mColumnHeaders{};
};

} // namespace rsp::graphics

#endif // COMPONENTLOADER_H