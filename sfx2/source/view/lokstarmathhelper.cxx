/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sal/config.h>

#include <sfx2/ipclient.hxx>
#include <sfx2/lokcomponenthelpers.hxx>
#include <sfx2/lokhelper.hxx>
#include <sfx2/objsh.hxx>

#include <comphelper/dispatchcommand.hxx>
#include <toolkit/helper/vclunohelper.hxx>
#include <tools/fract.hxx>
#include <tools/UnitConversion.hxx>
#include <vcl/layout.hxx>
#include <vcl/window.hxx>

#include <com/sun/star/embed/XEmbeddedObject.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>

LokStarMathHelper::LokStarMathHelper(const SfxViewShell* pViewShell)
    : mpViewShell(pViewShell)
{
    if (mpViewShell)
    {
        if (const SfxInPlaceClient* pIPClient = mpViewShell->GetIPClient())
        {
            if (const auto& xEmbObj = pIPClient->GetObject())
            {
                css::uno::Reference<css::lang::XServiceInfo> xComp(xEmbObj->getComponent(),
                                                                   css::uno::UNO_QUERY);
                if (xComp && xComp->supportsService("com.sun.star.formula.FormulaProperties"))
                {
                    if (css::uno::Reference<css::frame::XModel> xModel{ xComp,
                                                                        css::uno::UNO_QUERY })
                    {
                        if (auto xController = xModel->getCurrentController())
                        {
                            mpIPClient = pIPClient;
                            mxFrame = xController->getFrame();
                        }
                    }
                }
            }
        }
    }
}

void LokStarMathHelper::Dispatch(
    const OUString& cmd, const css::uno::Sequence<css::beans::PropertyValue>& rArguments) const
{
    if (mxFrame)
        comphelper::dispatchCommand(cmd, mxFrame, rArguments);
}

namespace
{
// Find a child SmGraphicWindow*
vcl::Window* FindSmGraphicWindow(vcl::Window* pWin)
{
    if (!pWin)
        return nullptr;

    if (pWin->IsStarMath())
        return pWin;

    pWin = pWin->GetWindow(GetWindowType::FirstChild);
    while (pWin)
    {
        if (vcl::Window* pSmGraphicWindow = FindSmGraphicWindow(pWin))
            return pSmGraphicWindow;
        pWin = pWin->GetWindow(GetWindowType::Next);
    }
    return nullptr;
}

// Find a child window that corresponds to SmGraphicWidget
vcl::Window* FindChildSmGraphicWidgetWindow(vcl::Window* pWin)
{
    if (!pWin)
        return nullptr;

    // The needed window is a VclDrawingArea
    if (dynamic_cast<VclDrawingArea*>(pWin))
        return pWin;

    pWin = pWin->GetWindow(GetWindowType::FirstChild);
    while (pWin)
    {
        if (vcl::Window* pSmGraphicWidgetWindow = FindChildSmGraphicWidgetWindow(pWin))
            return pSmGraphicWidgetWindow;
        pWin = pWin->GetWindow(GetWindowType::Next);
    }
    return nullptr;
}
}

vcl::Window* LokStarMathHelper::GetGraphicWindow()
{
    if (!mpGraphicWindow)
    {
        if (mxFrame)
        {
            css::uno::Reference<css::awt::XWindow> xDockerWin = mxFrame->getContainerWindow();
            mpGraphicWindow.set(FindSmGraphicWindow(VCLUnoHelper::GetWindow(xDockerWin)));
        }
    }

    return mpGraphicWindow.get();
}

vcl::Window* LokStarMathHelper::GetWidgetWindow()
{
    if (!mpWidgetWindow)
        mpWidgetWindow.set(FindChildSmGraphicWidgetWindow(GetGraphicWindow()));

    return mpWidgetWindow.get();
}

const SfxViewShell* LokStarMathHelper::GetSmViewShell()
{
    if (vcl::Window* pGraphWindow = GetGraphicWindow())
    {
        return SfxViewShell::GetFirst(false, [pGraphWindow](const SfxViewShell* shell) {
            return shell->GetWindow() && shell->GetWindow()->IsChild(pGraphWindow);
        });
    }
    return nullptr;
}

tools::Rectangle LokStarMathHelper::GetBoundingBox() const
{
    if (mpIPClient)
    {
        tools::Rectangle r(mpIPClient->GetObjArea());
        if (SfxObjectShell* pObjShell = const_cast<SfxViewShell*>(mpViewShell)->GetObjectShell())
        {
            const o3tl::Length unit = MapToO3tlLength(pObjShell->GetMapUnit());
            if (unit != o3tl::Length::twip && unit != o3tl::Length::invalid)
                r = o3tl::convert(r, unit, o3tl::Length::twip);
        }
        return r;
    }
    return {};
}

bool LokStarMathHelper::postMouseEvent(int nType, int nX, int nY, int nCount, int nButtons,
                                       int nModifier, double fPPTScaleX, double fPPTScaleY)
{
    const tools::Rectangle rBBox = GetBoundingBox();
    if (Point aMousePos(nX, nY); rBBox.Contains(aMousePos))
    {
        if (vcl::Window* pWindow = GetWidgetWindow())
        {
            aMousePos -= rBBox.TopLeft();

            // In lok, Math does not convert coordinates (see SmGraphicWidget::SetDrawingArea,
            // which disables MapMode), and uses twips internally (see SmDocShell ctor and
            // SmMapUnit), but the conversion factor can depend on the client zoom.
            // 1. Remove the twip->pixel factor in the passed scales
            double fScaleX = o3tl::convert(fPPTScaleX, o3tl::Length::px, o3tl::Length::twip);
            double fScaleY = o3tl::convert(fPPTScaleY, o3tl::Length::px, o3tl::Length::twip);
            // 2. Adjust the position according to the scales
            aMousePos
                = Point(std::round(aMousePos.X() * fScaleX), std::round(aMousePos.Y() * fScaleY));

            LokMouseEventData aMouseEventData(
                nType, aMousePos, nCount, MouseEventModifiers::SIMPLECLICK, nButtons, nModifier);
            SfxLokHelper::postMouseEventAsync(pWindow, aMouseEventData);

            return true;
        }
    }
    return false;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab cinoptions=b1,g0,N-s cinkeys+=0=break: */
