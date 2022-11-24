/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 This file has been autogenerated by update_pch.sh. It is possible to edit it
 manually (such as when an include file has been moved/renamed/removed). All such
 manual changes will be rewritten by the next run of update_pch.sh (which presumably
 also fixes all possible problems, so it's usually better to use it).

 Generated on 2021-11-03 15:11:24 using:
 ./bin/update_pch external/poppler poppler --cutoff=1 --exclude:system --include:module --include:local

 If after updating build fails, use the following command to locate conflicting headers:
 ./bin/update_pch_bisect ./external/poppler/inc/pch/precompiled_poppler.hxx "make external/poppler.build" --find-conflicts
*/

#include <sal/config.h>
#if PCH_LEVEL >= 1
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <config.h>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <gdir.h>
#include <gfile.h>
#include <glibc.h>
#include <gmem.h>
#include <grandom.h>
#include <gstrtod.h>
#include <limits>
#include <memory>
#include <poppler-config.h>
#include <random>
#include <regex>
#include <set>
#include <vector>
#endif // PCH_LEVEL >= 1
#if PCH_LEVEL >= 2
#endif // PCH_LEVEL >= 2
#if PCH_LEVEL >= 3
#include <fofi/FoFiIdentifier.h>
#include <fofi/FoFiTrueType.h>
#include <fofi/FoFiType1.h>
#include <fofi/FoFiType1C.h>
#include <goo/GooCheckedOps.h>
#include <goo/GooLikely.h>
#include <goo/GooString.h>
#include <goo/GooTimer.h>
#include <goo/gdir.h>
#include <goo/gfile.h>
#include <goo/glibc.h>
#include <goo/gmem.h>
#include <goo/grandom.h>
#include <goo/gstrtod.h>
#include <poppler/Error.h>
#include <splash/SplashTypes.h>
#include <sys/stat.h>
#endif // PCH_LEVEL >= 3
#if PCH_LEVEL >= 4
#endif // PCH_LEVEL >= 4

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */