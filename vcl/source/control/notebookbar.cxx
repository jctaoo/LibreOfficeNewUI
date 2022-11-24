/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <sal/config.h>

#include <string_view>
#include <utility>

#include <vcl/layout.hxx>
#include <vcl/notebookbar/notebookbar.hxx>
#include <vcl/syswin.hxx>
#include <vcl/taskpanelist.hxx>
#include <vcl/NotebookbarContextControl.hxx>
#include <cppuhelper/implbase.hxx>
#include <comphelper/processfactory.hxx>
#include <rtl/bootstrap.hxx>
#include <osl/file.hxx>
#include <config_folders.h>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/FrameAction.hpp>
#include <com/sun/star/ui/ContextChangeEventMultiplexer.hpp>
#include <comphelper/lok.hxx>
#include <iostream>

static OUString getCustomizedUIRootDir()
{
    OUString sShareLayer("${$BRAND_BASE_DIR/" LIBO_ETC_FOLDER "/" SAL_CONFIGFILE(
        "bootstrap") ":UserInstallation}/user/config/soffice.cfg/");
    rtl::Bootstrap::expandMacros(sShareLayer);
    return sShareLayer;
}

static bool doesFileExist(std::u16string_view sUIDir, std::u16string_view sUIFile)
{
    OUString sUri = OUString::Concat(sUIDir) + sUIFile;
    osl::File file(sUri);
    return( file.open(0) == osl::FileBase::E_None );
}

/**
 * split from the main class since it needs different ref-counting mana
 */
class NotebookBarContextChangeEventListener : public ::cppu::WeakImplHelper<css::ui::XContextChangeEventListener, css::frame::XFrameActionListener>
{
    bool mbActive;
    VclPtr<NotebookBar> mpParent;
    css::uno::Reference<css::frame::XFrame> mxFrame;
public:
    NotebookBarContextChangeEventListener(NotebookBar *p, css::uno::Reference<css::frame::XFrame> xFrame) :
        mbActive(false),
        mpParent(p),
        mxFrame(std::move(xFrame))
    {}

    void setupFrameListener(bool bListen);
    void setupListener(bool bListen);

    // XContextChangeEventListener
    virtual void SAL_CALL notifyContextChangeEvent(const css::ui::ContextChangeEventObject& rEvent) override;

    // XFrameActionListener
    virtual void SAL_CALL frameAction(const css::frame::FrameActionEvent& rEvent) override;

    virtual void SAL_CALL disposing(const ::css::lang::EventObject&) override;
};

NotebookBar::NotebookBar(Window* pParent, const OString& rID, const OUString& rUIXMLDescription,
                         const css::uno::Reference<css::frame::XFrame>& rFrame,
                         const NotebookBarAddonsItem& aNotebookBarAddonsItem)
    : Control(pParent)
    , m_pEventListener(new NotebookBarContextChangeEventListener(this, rFrame))
    , m_pViewShell(nullptr)
    , m_bIsWelded(false)
    , m_sUIXMLDescription(rUIXMLDescription)
{
    m_pEventListener->setupFrameListener(true);

    SetStyle(GetStyle() | WB_DIALOGCONTROL);
    OUString sUIDir = AllSettings::GetUIRootDir();
    bool doesCustomizedUIExist = doesFileExist(getCustomizedUIRootDir(), rUIXMLDescription);
    if ( doesCustomizedUIExist )
        sUIDir = getCustomizedUIRootDir();

    bool bIsWelded = comphelper::LibreOfficeKit::isActive();
    if (bIsWelded)
    {
        m_bIsWelded = true;
        m_xVclContentArea = VclPtr<VclVBox>::Create(this);
        m_xVclContentArea->Show();
        // now access it using GetMainContainer and set dispose callback with SetDisposeCallback
    }
    else
    {
        m_pUIBuilder.reset(
            new VclBuilder(this, sUIDir, rUIXMLDescription, rID, rFrame, true, &aNotebookBarAddonsItem));

        // In the Notebookbar's .ui file must exist control handling context
        // - implementing NotebookbarContextControl interface with id "ContextContainer"
        // or "ContextContainerX" where X is a number >= 1
        NotebookbarContextControl* pContextContainer = nullptr;
        int i = 0;
        do
        {
            OUString aName = "ContextContainer";
            if (i)
                aName += OUString::number(i);

            pContextContainer = dynamic_cast<NotebookbarContextControl*>(m_pUIBuilder->get<Window>(OUStringToOString(aName, RTL_TEXTENCODING_UTF8)));
            if (pContextContainer)
                m_pContextContainers.push_back(pContextContainer);
            i++;
        }
        while( pContextContainer != nullptr );
    }

    UpdateBackground();
}

void NotebookBar::SetDisposeCallback(const Link<const SfxViewShell*, void> rDisposeCallback, const SfxViewShell* pViewShell)
{
    m_rDisposeLink = rDisposeCallback;
    m_pViewShell = pViewShell;
}

NotebookBar::~NotebookBar()
{
    disposeOnce();
}

void NotebookBar::dispose()
{
    m_pContextContainers.clear();
    if (m_pSystemWindow && m_pSystemWindow->ImplIsInTaskPaneList(this))
        m_pSystemWindow->GetTaskPaneList()->RemoveWindow(this);
    m_pSystemWindow.clear();

    if (m_rDisposeLink.IsSet())
        m_rDisposeLink.Call(m_pViewShell);

    if (m_bIsWelded)
        m_xVclContentArea.disposeAndClear();
    else
        disposeBuilder();

    m_pEventListener->setupFrameListener(false);
    m_pEventListener->setupListener(false);
    m_pEventListener.clear();

    Control::dispose();
}

bool NotebookBar::PreNotify(NotifyEvent& rNEvt)
{
    // capture KeyEvents for taskpane cycling
    if (rNEvt.GetType() == NotifyEventType::KEYINPUT)
    {
        if (m_pSystemWindow)
            return m_pSystemWindow->PreNotify(rNEvt);
    }
    return Window::PreNotify( rNEvt );
}

Size NotebookBar::GetOptimalSize() const
{
    if (isLayoutEnabled(this))
        return VclContainer::getLayoutRequisition(*GetWindow(GetWindowType::FirstChild));

    return Control::GetOptimalSize();
}

static int flag = 0;
static void PrintHierarchy(vcl::Window* window, int level) {
    for (int i = 0; i < level; i ++) {
        printf("  ");
    }
    printf("-> %d: ", level);

    switch (window->GetType())
    {
        case WindowType::NONE:
            printf("WindowType::NONE                ");
            break;
        case WindowType::MESSBOX:
            printf("WindowType::MESSBOX     or ::FIRST        ");
            break;
        case WindowType::INFOBOX:
            printf("WindowType::INFOBOX             ");
            break;
        case WindowType::WARNINGBOX:
            printf("WindowType::WARNINGBOX          ");
            break;
        case WindowType::ERRORBOX:
            printf("WindowType::ERRORBOX            ");
            break;
        case WindowType::QUERYBOX:
            printf("WindowType::QUERYBOX            ");
            break;
        case WindowType::WINDOW:
            printf("WindowType::WINDOW              ");
            break;
        case WindowType::WORKWINDOW:
            printf("WindowType::WORKWINDOW          ");
            break;
        case WindowType::CONTAINER:
            printf("WindowType::CONTAINER           ");
            break;
        case WindowType::FLOATINGWINDOW:
            printf("WindowType::FLOATINGWINDOW      ");
            break;
        case WindowType::DIALOG:
            printf("WindowType::DIALOG              ");
            break;
        case WindowType::MODELESSDIALOG:
            printf("WindowType::MODELESSDIALOG      ");
            break;
        case WindowType::CONTROL:
            printf("WindowType::CONTROL             ");
            break;
        case WindowType::PUSHBUTTON:
            printf("WindowType::PUSHBUTTON          ");
            break;
        case WindowType::OKBUTTON:
            printf("WindowType::OKBUTTON            ");
            break;
        case WindowType::CANCELBUTTON:
            printf("WindowType::CANCELBUTTON        ");
            break;
        case WindowType::HELPBUTTON:
            printf("WindowType::HELPBUTTON          ");
            break;
        case WindowType::IMAGEBUTTON:
            printf("WindowType::IMAGEBUTTON         ");
            break;
        case WindowType::MENUBUTTON:
            printf("WindowType::MENUBUTTON          ");
            break;
        case WindowType::MOREBUTTON:
            printf("WindowType::MOREBUTTON          ");
            break;
        case WindowType::SPINBUTTON:
            printf("WindowType::SPINBUTTON          ");
            break;
        case WindowType::RADIOBUTTON:
            printf("WindowType::RADIOBUTTON         ");
            break;
        case WindowType::CHECKBOX:
            printf("WindowType::CHECKBOX            ");
            break;
        case WindowType::TRISTATEBOX:
            printf("WindowType::TRISTATEBOX         ");
            break;
        case WindowType::EDIT:
            printf("WindowType::EDIT                ");
            break;
        case WindowType::MULTILINEEDIT:
            printf("WindowType::MULTILINEEDIT       ");
            break;
        case WindowType::COMBOBOX:
            printf("WindowType::COMBOBOX            ");
            break;
        case WindowType::LISTBOX:
            printf("WindowType::LISTBOX             ");
            break;
        case WindowType::MULTILISTBOX:
            printf("WindowType::MULTILISTBOX        ");
            break;
        case WindowType::FIXEDTEXT:
            printf("WindowType::FIXEDTEXT           ");
            break;
        case WindowType::FIXEDLINE:
            printf("WindowType::FIXEDLINE           ");
            break;
        case WindowType::FIXEDBITMAP:
            printf("WindowType::FIXEDBITMAP         ");
            break;
        case WindowType::FIXEDIMAGE:
            printf("WindowType::FIXEDIMAGE          ");
            break;
        case WindowType::GROUPBOX:
            printf("WindowType::GROUPBOX            ");
            break;
        case WindowType::SCROLLBAR:
            printf("WindowType::SCROLLBAR           ");
            break;
        case WindowType::SCROLLBARBOX:
            printf("WindowType::SCROLLBARBOX        ");
            break;
        case WindowType::SPLITTER:
            printf("WindowType::SPLITTER            ");
            break;
        case WindowType::SPLITWINDOW:
            printf("WindowType::SPLITWINDOW         ");
            break;
        case WindowType::SPINFIELD:
            printf("WindowType::SPINFIELD           ");
            break;
        case WindowType::PATTERNFIELD:
            printf("WindowType::PATTERNFIELD        ");
            break;
        case WindowType::METRICFIELD:
            printf("WindowType::METRICFIELD         ");
            break;
        case WindowType::FORMATTEDFIELD:
            printf("WindowType::FORMATTEDFIELD      ");
            break;
        case WindowType::CURRENCYFIELD:
            printf("WindowType::CURRENCYFIELD       ");
            break;
        case WindowType::DATEFIELD:
            printf("WindowType::DATEFIELD           ");
            break;
        case WindowType::TIMEFIELD:
            printf("WindowType::TIMEFIELD           ");
            break;
        case WindowType::PATTERNBOX:
            printf("WindowType::PATTERNBOX          ");
            break;
        case WindowType::NUMERICBOX:
            printf("WindowType::NUMERICBOX          ");
            break;
        case WindowType::METRICBOX:
            printf("WindowType::METRICBOX           ");
            break;
        case WindowType::CURRENCYBOX:
            printf("WindowType::CURRENCYBOX         ");
            break;
        case WindowType::DATEBOX:
            printf("WindowType::DATEBOX             ");
            break;
        case WindowType::TIMEBOX:
            printf("WindowType::TIMEBOX             ");
            break;
        case WindowType::LONGCURRENCYBOX:
            printf("WindowType::LONGCURRENCYBOX     ");
            break;
        case WindowType::SCROLLWINDOW:
            printf("WindowType::SCROLLWINDOW        ");
            break;
        case WindowType::TOOLBOX:
            printf("WindowType::TOOLBOX             ");
            break;
        case WindowType::DOCKINGWINDOW:
            printf("WindowType::DOCKINGWINDOW       ");
            break;
        case WindowType::STATUSBAR:
            printf("WindowType::STATUSBAR           ");
            break;
        case WindowType::TABPAGE:
            printf("WindowType::TABPAGE             ");
            break;
        case WindowType::TABCONTROL:
            printf("WindowType::TABCONTROL          ");
            break;
        case WindowType::TABDIALOG:
            printf("WindowType::TABDIALOG           ");
            break;
        case WindowType::BORDERWINDOW:
            printf("WindowType::BORDERWINDOW        ");
            break;
        case WindowType::BUTTONDIALOG:
            printf("WindowType::BUTTONDIALOG        ");
            break;
        case WindowType::SYSTEMCHILDWINDOW:
            printf("WindowType::SYSTEMCHILDWINDOW   ");
            break;
        case WindowType::SLIDER:
            printf("WindowType::SLIDER              ");
            break;
        case WindowType::MENUBARWINDOW:
            printf("WindowType::MENUBARWINDOW       ");
            break;
        case WindowType::TREELISTBOX:
            printf("WindowType::TREELISTBOX         ");
            break;
        case WindowType::HELPTEXTWINDOW:
            printf("WindowType::HELPTEXTWINDOW      ");
            break;
        case WindowType::INTROWINDOW:
            printf("WindowType::INTROWINDOW         ");
            break;
        case WindowType::LISTBOXWINDOW:
            printf("WindowType::LISTBOXWINDOW       ");
            break;
        case WindowType::DOCKINGAREA:
            printf("WindowType::DOCKINGAREA         ");
            break;
        case WindowType::RULER:
            printf("WindowType::RULER               ");
            break;
        case WindowType::HEADERBAR:
            printf("WindowType::HEADERBAR           ");
            break;
        case WindowType::VERTICALTABCONTROL:
            printf("WindowType::VERTICALTABCONTROL or ::LAST ");
            break;
        case WindowType::TOOLKIT_FRAMEWINDOW:
            printf("WindowType::TOOLKIT_FRAMEWINDOW ");
            break;
        case WindowType::TOOLKIT_SYSTEMCHILDWINDOW:
            printf("WindowType::TOOLKIT_SYSTEMCHILDWINDOW");
            break;
    }
    printf("\n");

    auto count = window->GetChildCount();
    for (sal_Int16 i = 0; i < count; i ++) {
        auto child = window->GetChild(i);
        PrintHierarchy(child, level + 1);
    }
}

void NotebookBar::setPosSizePixel(tools::Long nX, tools::Long nY, tools::Long nWidth, tools::Long nHeight, PosSizeFlags nFlags)
{
    bool bCanHandleSmallerWidth = false;
    bool bCanHandleSmallerHeight = false;

    bool bIsLayoutEnabled = isLayoutEnabled(this);
    Window *pChild = GetWindow(GetWindowType::FirstChild);

    if (bIsLayoutEnabled && pChild->GetType() == WindowType::SCROLLWINDOW)
    {
        WinBits nStyle = pChild->GetStyle();
        if (nStyle & (WB_AUTOHSCROLL | WB_HSCROLL))
            bCanHandleSmallerWidth = true;
        if (nStyle & (WB_AUTOVSCROLL | WB_VSCROLL))
            bCanHandleSmallerHeight = true;
    }

    Size aSize(GetOptimalSize());
    if (!bCanHandleSmallerWidth)
        nWidth = std::max(nWidth, aSize.Width());
    if (!bCanHandleSmallerHeight)
        nHeight = std::max(nHeight, aSize.Height());

    Control::setPosSizePixel(nX, nY, nWidth, nHeight, nFlags);

    if (bIsLayoutEnabled && (nFlags & PosSizeFlags::Size))
        VclContainer::setLayoutAllocation(*pChild, Point(0, 0), Size(nWidth, nHeight));
}

void NotebookBar::Resize()
{
    if(m_pUIBuilder && m_pUIBuilder->get_widget_root())
    {
        vcl::Window* pWindow = m_pUIBuilder->get_widget_root()->GetChild(0);
        if (pWindow)
        {
            Size aSize = pWindow->GetSizePixel();
            aSize.setWidth( GetSizePixel().Width() );
            pWindow->SetSizePixel(aSize);
        }
    }
    if(m_bIsWelded)
    {
        vcl::Window* pChild = GetWindow(GetWindowType::FirstChild);
        assert(pChild);
        VclContainer::setLayoutAllocation(*pChild, Point(0, 0), GetSizePixel());
        Control::Resize();
    }
    Control::Resize();
}

void NotebookBar::SetSystemWindow(SystemWindow* pSystemWindow)
{
    m_pSystemWindow = pSystemWindow;
    if (!m_pSystemWindow->ImplIsInTaskPaneList(this))
        m_pSystemWindow->GetTaskPaneList()->AddWindow(this);
}

void SAL_CALL NotebookBarContextChangeEventListener::notifyContextChangeEvent(const css::ui::ContextChangeEventObject& rEvent)
{
    if (mpParent)
    {
        for (NotebookbarContextControl* pControl : mpParent->m_pContextContainers)
            pControl->SetContext(vcl::EnumContext::GetContextEnum(rEvent.ContextName));
    }
}

void NotebookBarContextChangeEventListener::setupListener(bool bListen)
{
    if (comphelper::LibreOfficeKit::isActive())
        return;

    auto xMultiplexer(css::ui::ContextChangeEventMultiplexer::get(::comphelper::getProcessComponentContext()));

    if (bListen)
    {
        try
        {
            xMultiplexer->addContextChangeEventListener(this, mxFrame->getController());
        }
        catch (const css::uno::Exception&)
        {
        }
    }
    else
        xMultiplexer->removeAllContextChangeEventListeners(this);

    mbActive = bListen;
}

void NotebookBarContextChangeEventListener::setupFrameListener(bool bListen)
{
    if (bListen)
        mxFrame->addFrameActionListener(this);
    else
        mxFrame->removeFrameActionListener(this);
}

void SAL_CALL NotebookBarContextChangeEventListener::frameAction(const css::frame::FrameActionEvent& rEvent)
{
    if (!mbActive)
        return;

    if (rEvent.Action == css::frame::FrameAction_COMPONENT_REATTACHED)
    {
        setupListener(true);
    }
    else if (rEvent.Action == css::frame::FrameAction_COMPONENT_DETACHING)
    {
        setupListener(false);
        // We don't want to give up on listening; just wait for
        // another controller to be attached to the frame.
        mbActive = true;
    }
}

void NotebookBar::SetupListener(bool bListen)
{
    m_pEventListener->setupListener(bListen);
}

void SAL_CALL NotebookBarContextChangeEventListener::disposing(const ::css::lang::EventObject&)
{
    mpParent.clear();
}

void NotebookBar::DataChanged(const DataChangedEvent& rDCEvt)
{
    UpdateBackground();
    Control::DataChanged(rDCEvt);
}

void NotebookBar::StateChanged(const  StateChangedType nStateChange )
{
    UpdateBackground();
    Control::StateChanged(nStateChange);
    Invalidate();
}

void NotebookBar::UpdateBackground()
{
    const StyleSettings& rStyleSettings = GetSettings().GetStyleSettings();
    const BitmapEx& aPersona = rStyleSettings.GetPersonaHeader();
    Wallpaper aWallpaper(aPersona);
    aWallpaper.SetStyle(WallpaperStyle::TopRight);
    if (!aPersona.IsEmpty())
    {
        SetBackground(aWallpaper);
        UpdatePersonaSettings();
        GetOutDev()->SetSettings( PersonaSettings );
    }
    else
    {
        SetBackground(rStyleSettings.GetDialogColor());
        UpdateDefaultSettings();
        GetOutDev()->SetSettings( DefaultSettings );
    }

    Invalidate(tools::Rectangle(Point(0,0), GetSizePixel()));
}

void NotebookBar::UpdateDefaultSettings()
{
    AllSettings aAllSettings( GetSettings() );
    StyleSettings aStyleSet( aAllSettings.GetStyleSettings() );

    ::Color aTextColor = aStyleSet.GetFieldTextColor();
    aStyleSet.SetDialogTextColor( aTextColor );
    aStyleSet.SetButtonTextColor( aTextColor );
    aStyleSet.SetRadioCheckTextColor( aTextColor );
    aStyleSet.SetGroupTextColor( aTextColor );
    aStyleSet.SetLabelTextColor( aTextColor );
    aStyleSet.SetWindowTextColor( aTextColor );
    aStyleSet.SetTabTextColor(aTextColor);
    aStyleSet.SetToolTextColor(aTextColor);

    aAllSettings.SetStyleSettings(aStyleSet);
    DefaultSettings = aAllSettings;
}

void NotebookBar::UpdatePersonaSettings()
{
    AllSettings aAllSettings( GetSettings() );
    StyleSettings aStyleSet( aAllSettings.GetStyleSettings() );

    ::Color aTextColor = aStyleSet.GetPersonaMenuBarTextColor().value_or(COL_BLACK );
    aStyleSet.SetDialogTextColor( aTextColor );
    aStyleSet.SetButtonTextColor( aTextColor );
    aStyleSet.SetRadioCheckTextColor( aTextColor );
    aStyleSet.SetGroupTextColor( aTextColor );
    aStyleSet.SetLabelTextColor( aTextColor );
    aStyleSet.SetWindowTextColor( aTextColor );
    aStyleSet.SetTabTextColor(aTextColor);
    aStyleSet.SetToolTextColor(aTextColor);

    aAllSettings.SetStyleSettings(aStyleSet);
    PersonaSettings = aAllSettings;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
