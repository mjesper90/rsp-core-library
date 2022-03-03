/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */
#ifndef TOUCHAREA_H
#define TOUCHAREA_H

#include "graphics/controls/Image.h"
#include "graphics/controls/Input.h"
#include "graphics/primitives/Rect.h"
#include "messaging/eventTypes/ClickedEvent.h"

using namespace rsp::messaging;

namespace rsp::graphics
{

class TouchArea
{
  public:
    TouchArea() : mTouchArea(0, 0, 0, 0), mClickedTopic(ClickTopic::NullTopic), mclickEvent(""){};
    TouchArea(Rect &aArea, ClickTopic aClickTopic = ClickTopic::NullTopic, std::string aClickInfo = "");
    ~TouchArea();

    void ProcessInput(Input &aInput);

    bool IsHit(const Point &aPoint) const;
    void RegisterOnPressed(std::function<void(Control::States)> aFunc);
    void RegisterOnClicked(std::function<void(int, ClickedEvent &)> aFunc);

    TouchArea &operator=(const TouchArea &) = default;

    TouchArea &SetArea(const Rect &arRect)
    {
        mTouchArea = arRect;
        return *this;
    }
    const Rect &GetArea() const { return mTouchArea; }

  protected:
    std::function<void(Control::States)> mPressed{};
    std::function<void(int, ClickedEvent &)> mClicked{};

    Rect mTouchArea{};
    Point mCurrentPress{0, 0};
    Point mOriginalPress{0, 0};
    ClickTopic mClickedTopic;
    ClickedEvent mclickEvent;
};

} // namespace rsp::graphics
#endif // TOUCHAREA_H
