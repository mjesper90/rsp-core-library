/*!
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * \copyright   Copyright 2021 RSP Systems A/S. All rights reserved.
 * \license     Mozilla Public License 2.0
 * \author      Simon Glashoff
 */
#ifndef BITMAP_H
#define BITMAP_H

#include <graphics/primitives/Canvas.h>
#include <graphics/primitives/raster/ImgLoader.h>
#include <memory>

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace rsp::graphics
{

/**
 * Class Bitmap
 *
 * The Bitmap is an object wrapper around raster images.
 * Various raster image formats can be implemented by descending specialized loaders
 * from the ImgLoader class and adding those to the GetRasterLoader method.
 */
class Bitmap : public Canvas
{
  public:
    /**
     * \brief Load bitmap from given file.
     *
     * \param aImgName
     */
    Bitmap(const std::string &arImgName);

    /**
     * \brief Create a bitmap of given pixel data
     *
     * \param apPixels
     * \param aHeight
     * \param aWidth
     * \param aBytesPerPixel
     */
    Bitmap(const uint32_t *apPixels, int aHeight, int aWidth, int aBytesPerPixel);

    /**
     * \brief Create an empty in memory bitmap
     *
     * \param aHeight
     * \param aWidth
     * \param aBytesPerPixel
     */
    Bitmap(int aHeight, int aWidth, int aBytesPerPixel);

    inline void SetPixel(const Point &arPoint, const Color &arColor) override
    {
        if (!IsInsideScreen(arPoint)) {
            return;
        }
        uint32_t location = static_cast<uint32_t>((mWidth * arPoint.mY) + arPoint.mX);
        mImagePixels[location] = arColor;
    }

    uint32_t GetPixel(const Point &aPoint, const bool aFront = false) const;

    /**
     * \brief Get the height of the bitmap.
     *
     * \return uint32_t
     */
    int GetHeight() const
    {
        return mHeight;
    }

    /**
     * \brief Get the width of the bitmap.
     *
     * \return uint32_t
     */
    int GetWidth() const
    {
        return mWidth;
    }

    /**
     * \brief Get a read only reference to the pixel data.
     *
     * \return const std::vector<uint32_t>&
     */
    const std::vector<uint32_t> &GetPixels() const
    {
        return mImagePixels;
    }

  protected:
    static std::unordered_map<std::string, std::function<std::shared_ptr<ImgLoader>()>> msFiletypeMap;
    std::shared_ptr<ImgLoader> GetRasterLoader(const std::string aFileExtension);
    std::vector<uint32_t> mImagePixels{}; // Pointer?
};

} // namespace rsp::graphics
#endif // BITMAP_H
