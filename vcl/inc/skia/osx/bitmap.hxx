/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_VCL_INC_SKIA_OSX_BITMAP_HXX
#define INCLUDED_VCL_INC_SKIA_OSX_BITMAP_HXX

#include <vcl/dllapi.h>

#include <osx/osxvcltypes.h>

class Image;

namespace SkiaHelper
{
VCL_PLUGIN_PUBLIC CGImageRef createCGImage(const Image& rImage);
};

#endif // INCLUDED_VCL_INC_SKIA_OSX_BITMAP_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
