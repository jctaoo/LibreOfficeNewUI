/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#pragma once

#include <com/sun/star/uno/Any.hxx>

#include <svx/svxdllapi.h>
#include <svx/svdtypes.hxx>

/*
 Stores a bitfield of the layer values that have been set.
*/

class SVXCORE_DLLPUBLIC SdrLayerIDSet final
{
    // For now, have up to 256 layers
    sal_uInt8 aData[32];

public:
    explicit SdrLayerIDSet(bool bInitVal = false)
    {
        memset(aData, bInitVal ? 0xFF : 0x00, sizeof(aData));
    }

    bool operator!=(const SdrLayerIDSet& rCmpSet) const
    {
        return (memcmp(aData, rCmpSet.aData, sizeof(aData))!=0);
    }

    void Set(SdrLayerID a)
    {
        const sal_Int16 nId = a.get();
        if (nId >= 0 && nId < 256)
            aData[nId / 8] |= 1 << (nId % 8);
    }

    void Clear(SdrLayerID a)
    {
        const sal_Int16 nId = a.get();
        if (nId >= 0 && nId < 256)
            aData[nId / 8] &= ~(1 << (nId % 8));
    }

    void Set(SdrLayerID a, bool b)
    {
        if(b)
            Set(a);
        else
            Clear(a);
    }

    bool IsSet(SdrLayerID a) const
    {
        const sal_Int16 nId = a.get();
        return nId >= 0 && nId < 256 && (aData[nId / 8] & 1 << nId % 8) != 0;
    }

    void SetAll()
    {
        memset(aData, 0xFF, sizeof(aData));
    }

    void ClearAll()
    {
        memset(aData, 0x00, sizeof(aData));
    }

    bool IsEmpty() const;

    void operator&=(const SdrLayerIDSet& r2ndSet);

    // initialize this set with a UNO sequence of sal_Int8 (e.g. as stored in settings.xml)
    void PutValue(const css::uno::Any & rAny);

};

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
