/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 * \author      Steffen Brummer
 */
#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <utils/CoreException.h>

namespace rsp::graphics
{

/**
 * Color class
 *
 * A color consist of tree base colors: Red, green and blue,
 * and an optional alpha channel that determines transparency.
 *
 * Note: Not all hardware supports alpha channel transparency.
 */
class Color
{
  public:
    typedef uint32_t ARGB_t;

    /**
     * \brief Predefined basic colors
     */
    enum : ARGB_t {
        White = 0x00FFFFFF,
        Silver = 0x00C0C0C0,
        Grey = 0x00808080,
        Black = 0x00000000,
        Red = 0x00FF0000,
        Maroon = 0x00800000,
        Yellow = 0x00FFFF00,
        Olive = 0x00808000,
        Lime = 0x0000FF00,
        Green = 0x00008000,
        Aqua = 0x0000FFFF,
        Teal = 0x00008080,
        Blue = 0x000000FF,
        Navy = 0x00000080,
        Fuchsia = 0x00FF00FF,
        Purple = 0x00800080
    };

    /**
     * \brief Construct with given base colors.
     *
     * \param aRed
     * \param aGreen
     * \param aBlue
     * \param aAlpha
     */
    Color(uint8_t aRed, uint8_t aGreen, uint8_t aBlue, uint8_t aAlpha);

    /**
     * \brief Construct from ARGB value.
     *
     * \param aARGB
     */
    Color(ARGB_t aARGB);

    /**
     * \brief Copy constructor.
     *
     * \param aColor
     */
    Color(const Color &aColor);

    /**
     * \brief Get the red base color value.
     *
     * \return Red value
     */
    uint8_t GetRed() const;
    /**
     * \brief Set the red base color value.
     *
     * \param aValue
     */
    void SetRed(uint8_t aValue);

    /**
     * \brief Get the green base color value.
     *
     * \return Green value
     */
    uint8_t GetGreen() const;
    /**
     * \brief Set the green base color value.
     *
     * \param aValue
     */
    void SetGreen(uint8_t aValue);

    /**
     * \brief Get the blue base color value.
     *
     * \return Blue value
     */
    uint8_t GetBlue() const;
    /**
     * \brief Set the blue base color value.
     *
     * \param aValue
     */
    void SetBlue(uint8_t aValue);

    /**
     * \brief Get the alpha channel value.
     *
     * \return Alpha value
     */
    uint8_t GetAlpha() const;
    /**
     * \brief Set the alpha channel value.
     *
     * \param aValue
     */
    void SetAlpha(uint8_t aValue);

    /**
     * \brief Get the ARGB value.
     * \return ARGB
     */
    operator ARGB_t() const;
    /**
     * \brief Assign the value from the given Color object.
     *
     * \param aValue
     */
    Color &operator=(const Color &aColor);

  protected:
    /**
     * \brief Color value type
     */
    typedef union {
        ARGB_t rgba;
#ifdef LITTLE_ENDIAN
        struct __item_type {
            uint32_t blue : 8;
            uint32_t green : 8;
            uint32_t red : 8;
            uint32_t alpha : 8;
        } item;
#else
        struct __item_type {
            uint32_t alpha : 8;
            uint32_t red : 8;
            uint32_t green : 8;
            uint32_t blue : 8;
        } item;
#endif
    } ColorValue_t;

    ColorValue_t mValue{};
};

} // namespace rsp::graphics
#endif // COLOR_H
