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

#include <ConfigAccess.hxx>

#include <unotools/configitem.hxx>
#include <o3tl/any.hxx>
#include <com/sun/star/uno/Sequence.hxx>

namespace chart
{
using namespace ::com::sun::star;

namespace
{
class ChartConfigItem : public ::utl::ConfigItem
{
private:
    virtual void ImplCommit() override;

public:
    ChartConfigItem();

    bool getUseErrorRectangle();
    virtual void Notify(const uno::Sequence<OUString>& aPropertyNames) override;
};
}

ChartConfigItem::ChartConfigItem()
    : ConfigItem("Office.Chart/ErrorProperties")
{
}

void ChartConfigItem::ImplCommit() {}
void ChartConfigItem::Notify(const uno::Sequence<OUString>&) {}

bool ChartConfigItem::getUseErrorRectangle()
{
    uno::Sequence<OUString> aNames{ "ErrorRectangle" };

    auto b = o3tl::tryAccess<bool>(GetProperties(aNames)[0]);
    return b && *b;
}

namespace ConfigAccess
{
bool getUseErrorRectangle()
{
    //a ChartConfigItem Singleton
    static ChartConfigItem SINGLETON;
    bool bResult(SINGLETON.getUseErrorRectangle());
    return bResult;
}
} //namespace ConfigAccess

} //namespace chart

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
