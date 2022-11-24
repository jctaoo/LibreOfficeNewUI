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

 Generated on 2022-08-13 18:01:17 using:
 ./bin/update_pch slideshow slideshow --cutoff=4 --exclude:system --include:module --exclude:local

 If after updating build fails, use the following command to locate conflicting headers:
 ./bin/update_pch_bisect ./slideshow/inc/pch/precompiled_slideshow.hxx "make slideshow.build" --find-conflicts
*/

#include <sal/config.h>
#if PCH_LEVEL >= 1
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <float.h>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <limits.h>
#include <limits>
#include <map>
#include <math.h>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <ostream>
#include <stddef.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>
#include <boost/property_tree/ptree_fwd.hpp>
#endif // PCH_LEVEL >= 1
#if PCH_LEVEL >= 2
#include <osl/diagnose.h>
#include <osl/diagnose.hxx>
#include <osl/doublecheckedlocking.h>
#include <osl/endian.h>
#include <osl/getglobalmutex.hxx>
#include <osl/interlck.h>
#include <osl/mutex.h>
#include <osl/mutex.hxx>
#include <osl/thread.hxx>
#include <osl/time.h>
#include <rtl/alloc.h>
#include <rtl/instance.hxx>
#include <rtl/math.hxx>
#include <rtl/ref.hxx>
#include <rtl/strbuf.h>
#include <rtl/strbuf.hxx>
#include <rtl/string.h>
#include <rtl/string.hxx>
#include <rtl/stringconcat.hxx>
#include <rtl/stringutils.hxx>
#include <rtl/textcvt.h>
#include <rtl/textenc.h>
#include <rtl/ustrbuf.h>
#include <rtl/ustring.h>
#include <rtl/ustring.hxx>
#include <sal/backtrace.hxx>
#include <sal/detail/log.h>
#include <sal/log.hxx>
#include <sal/macros.h>
#include <sal/saldllapi.h>
#include <sal/types.h>
#include <sal/typesizes.h>
#include <vcl/BinaryDataContainer.hxx>
#include <vcl/GraphicExternalLink.hxx>
#include <vcl/IDialogRenderable.hxx>
#include <vcl/Scanline.hxx>
#include <vcl/alpha.hxx>
#include <vcl/animate/Animation.hxx>
#include <vcl/animate/AnimationFrame.hxx>
#include <vcl/bitmap.hxx>
#include <vcl/bitmap/BitmapTypes.hxx>
#include <vcl/bitmapex.hxx>
#include <vcl/cairo.hxx>
#include <vcl/checksum.hxx>
#include <vcl/devicecoordinate.hxx>
#include <vcl/dllapi.h>
#include <comphelper/errcode.hxx>
#include <vcl/fntstyle.hxx>
#include <vcl/font.hxx>
#include <vcl/gdimtf.hxx>
#include <vcl/gfxlink.hxx>
#include <vcl/gradient.hxx>
#include <vcl/graph.hxx>
#include <vcl/mapmod.hxx>
#include <vcl/metaactiontypes.hxx>
#include <vcl/outdev.hxx>
#include <vcl/region.hxx>
#include <vcl/rendercontext/AddFontSubstituteFlags.hxx>
#include <vcl/rendercontext/AntialiasingFlags.hxx>
#include <vcl/rendercontext/DrawGridFlags.hxx>
#include <vcl/rendercontext/DrawImageFlags.hxx>
#include <vcl/rendercontext/DrawModeFlags.hxx>
#include <vcl/rendercontext/DrawTextFlags.hxx>
#include <vcl/rendercontext/GetDefaultFontFlags.hxx>
#include <vcl/rendercontext/ImplMapRes.hxx>
#include <vcl/rendercontext/InvertFlags.hxx>
#include <vcl/rendercontext/RasterOp.hxx>
#include <vcl/rendercontext/SalLayoutFlags.hxx>
#include <vcl/rendercontext/State.hxx>
#include <vcl/rendercontext/SystemTextColorFlags.hxx>
#include <vcl/salnativewidgets.hxx>
#include <vcl/scopedbitmapaccess.hxx>
#include <vcl/task.hxx>
#include <vcl/timer.hxx>
#include <vcl/vclenum.hxx>
#include <vcl/vclptr.hxx>
#include <vcl/vclreferencebase.hxx>
#include <vcl/vectorgraphicdata.hxx>
#include <vcl/wall.hxx>
#endif // PCH_LEVEL >= 2
#if PCH_LEVEL >= 3
#include <avmedia/avmediadllapi.h>
#include <basegfx/basegfxdllapi.h>
#include <basegfx/color/bcolor.hxx>
#include <basegfx/matrix/b2dhommatrix.hxx>
#include <basegfx/matrix/b2dhommatrixtools.hxx>
#include <basegfx/numeric/ftools.hxx>
#include <basegfx/point/b2dpoint.hxx>
#include <basegfx/point/b2ipoint.hxx>
#include <basegfx/polygon/b2dpolygon.hxx>
#include <basegfx/polygon/b2dpolygontools.hxx>
#include <basegfx/polygon/b2dpolypolygon.hxx>
#include <basegfx/polygon/b2dpolypolygontools.hxx>
#include <basegfx/range/Range2D.hxx>
#include <basegfx/range/b2drange.hxx>
#include <basegfx/range/b2drectangle.hxx>
#include <basegfx/range/basicrange.hxx>
#include <basegfx/tuple/Tuple2D.hxx>
#include <basegfx/tuple/Tuple3D.hxx>
#include <basegfx/tuple/b2i64tuple.hxx>
#include <basegfx/tuple/b2ituple.hxx>
#include <basegfx/tuple/b3dtuple.hxx>
#include <basegfx/utils/canvastools.hxx>
#include <basegfx/utils/common.hxx>
#include <basegfx/vector/b2dsize.hxx>
#include <basegfx/vector/b2dvector.hxx>
#include <basegfx/vector/b2enums.hxx>
#include <basegfx/vector/b2isize.hxx>
#include <basegfx/vector/b2ivector.hxx>
#include <canvas/canvastools.hxx>
#include <com/sun/star/animations/TransitionSubType.hpp>
#include <com/sun/star/animations/TransitionType.hpp>
#include <com/sun/star/awt/DeviceInfo.hpp>
#include <com/sun/star/awt/FontSlant.hpp>
#include <com/sun/star/awt/Key.hpp>
#include <com/sun/star/awt/KeyGroup.hpp>
#include <com/sun/star/awt/MouseButton.hpp>
#include <com/sun/star/awt/SystemPointer.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/drawing/FillStyle.hpp>
#include <com/sun/star/drawing/LineCap.hpp>
#include <com/sun/star/drawing/LineStyle.hpp>
#include <com/sun/star/drawing/TextFitToSizeType.hpp>
#include <com/sun/star/drawing/XDrawPage.hpp>
#include <com/sun/star/drawing/XShape.hpp>
#include <com/sun/star/form/FormComponentType.hpp>
#include <com/sun/star/geometry/IntegerSize2D.hpp>
#include <com/sun/star/graphic/XPrimitive2D.hpp>
#include <com/sun/star/lang/DisposedException.hpp>
#include <com/sun/star/lang/EventObject.hpp>
#include <com/sun/star/lang/NoSupportException.hpp>
#include <com/sun/star/lang/XComponent.hpp>
#include <com/sun/star/lang/XTypeProvider.hpp>
#include <com/sun/star/lang/XUnoTunnel.hpp>
#include <com/sun/star/presentation/ParagraphTarget.hpp>
#include <com/sun/star/presentation/XSlideShowView.hpp>
#include <com/sun/star/rendering/XCanvas.hpp>
#include <com/sun/star/style/XStyle.hpp>
#include <com/sun/star/uno/Any.h>
#include <com/sun/star/uno/Any.hxx>
#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/uno/Reference.hxx>
#include <com/sun/star/uno/RuntimeException.hpp>
#include <com/sun/star/uno/Sequence.h>
#include <com/sun/star/uno/Sequence.hxx>
#include <com/sun/star/uno/Type.h>
#include <com/sun/star/uno/Type.hxx>
#include <com/sun/star/uno/TypeClass.hdl>
#include <com/sun/star/uno/XInterface.hpp>
#include <com/sun/star/uno/XWeak.hpp>
#include <com/sun/star/uno/genfunc.h>
#include <com/sun/star/uno/genfunc.hxx>
#include <com/sun/star/util/Date.hpp>
#include <com/sun/star/util/DateTime.hpp>
#include <com/sun/star/util/Time.hpp>
#include <com/sun/star/util/XAccounting.hpp>
#include <comphelper/compbase.hxx>
#include <comphelper/comphelperdllapi.h>
#include <comphelper/interfacecontainer4.hxx>
#include <comphelper/weak.hxx>
#include <cppcanvas/basegfxfactory.hxx>
#include <cppcanvas/bitmap.hxx>
#include <cppcanvas/bitmapcanvas.hxx>
#include <cppcanvas/canvas.hxx>
#include <cppcanvas/canvasgraphic.hxx>
#include <cppcanvas/color.hxx>
#include <cppcanvas/customsprite.hxx>
#include <cppcanvas/renderer.hxx>
#include <cppcanvas/vclfactory.hxx>
#include <cppu/cppudllapi.h>
#include <cppu/unotype.hxx>
#include <cppuhelper/basemutex.hxx>
#include <cppuhelper/compbase.hxx>
#include <cppuhelper/cppuhelperdllapi.h>
#include <cppuhelper/implbase.hxx>
#include <cppuhelper/implbase_ex.hxx>
#include <cppuhelper/implbase_ex_post.hxx>
#include <cppuhelper/implbase_ex_pre.hxx>
#include <cppuhelper/queryinterface.hxx>
#include <cppuhelper/weak.hxx>
#include <cppuhelper/weakref.hxx>
#include <drawinglayer/drawinglayerdllapi.h>
#include <drawinglayer/geometry/viewinformation2d.hxx>
#include <drawinglayer/primitive2d/CommonTypes.hxx>
#include <drawinglayer/primitive2d/Primitive2DContainer.hxx>
#include <drawinglayer/primitive2d/Primitive2DVisitor.hxx>
#include <drawinglayer/primitive2d/baseprimitive2d.hxx>
#include <editeng/editengdllapi.h>
#include <editeng/outlobj.hxx>
#include <editeng/paragraphdata.hxx>
#include <i18nlangtag/lang.h>
#include <o3tl/cow_wrapper.hxx>
#include <o3tl/safeint.hxx>
#include <o3tl/span.hxx>
#include <o3tl/strong_int.hxx>
#include <o3tl/typed_flags_set.hxx>
#include <o3tl/underlyingenumvalue.hxx>
#include <o3tl/unit_conversion.hxx>
#include <salhelper/salhelperdllapi.h>
#include <salhelper/simplereferenceobject.hxx>
#include <svl/SfxBroadcaster.hxx>
#include <svl/cenumitm.hxx>
#include <svl/eitem.hxx>
#include <svl/hint.hxx>
#include <svl/itemset.hxx>
#include <svl/lstner.hxx>
#include <svl/macitem.hxx>
#include <svl/poolitem.hxx>
#include <svl/style.hxx>
#include <svl/stylesheetuser.hxx>
#include <svl/svldllapi.h>
#include <svl/typedwhich.hxx>
#include <svl/whichranges.hxx>
#include <svx/itextprovider.hxx>
#include <svx/sdr/animation/scheduler.hxx>
#include <svx/sdr/overlay/overlayobject.hxx>
#include <svx/sdr/properties/defaultproperties.hxx>
#include <svx/sdr/properties/properties.hxx>
#include <svx/sdtaditm.hxx>
#include <svx/sdtaitm.hxx>
#include <svx/sdtakitm.hxx>
#include <svx/svddef.hxx>
#include <svx/svdglue.hxx>
#include <svx/svdoattr.hxx>
#include <svx/svdobj.hxx>
#include <svx/svdobjkind.hxx>
#include <svx/svdotext.hxx>
#include <svx/svdtext.hxx>
#include <svx/svdtrans.hxx>
#include <svx/svdtypes.hxx>
#include <svx/svxdllapi.h>
#include <svx/xdef.hxx>
#include <svx/xpoly.hxx>
#include <tools/color.hxx>
#include <tools/date.hxx>
#include <tools/datetime.hxx>
#include <tools/degree.hxx>
#include <comphelper/diagnose_ex.hxx>
#include <tools/fldunit.hxx>
#include <tools/fontenum.hxx>
#include <tools/fract.hxx>
#include <tools/gen.hxx>
#include <tools/helpers.hxx>
#include <tools/lineend.hxx>
#include <tools/link.hxx>
#include <tools/long.hxx>
#include <tools/mapunit.hxx>
#include <tools/poly.hxx>
#include <tools/ref.hxx>
#include <tools/solar.h>
#include <tools/time.hxx>
#include <tools/toolsdllapi.h>
#include <tools/weakbase.h>
#include <tools/weakbase.hxx>
#include <typelib/typeclass.h>
#include <typelib/typedescription.h>
#include <typelib/uik.h>
#include <uno/any2.h>
#include <uno/data.h>
#include <uno/sequence2.h>
#include <unotools/fontdefs.hxx>
#include <unotools/resmgr.hxx>
#include <unotools/syslocale.hxx>
#include <unotools/unotoolsdllapi.h>
#endif // PCH_LEVEL >= 3
#if PCH_LEVEL >= 4
#include <activitiesfactory.hxx>
#include <activitiesqueue.hxx>
#include <animatableshape.hxx>
#include <animatedsprite.hxx>
#include <animationfactory.hxx>
#include <attributableshape.hxx>
#include <cursormanager.hxx>
#include <delayevent.hxx>
#include <disposable.hxx>
#include <doctreenode.hxx>
#include <doctreenodesupplier.hxx>
#include <eventmultiplexer.hxx>
#include <eventqueue.hxx>
#include <rgbcolor.hxx>
#include <screenupdater.hxx>
#include <shape.hxx>
#include <shapeattributelayer.hxx>
#include <slideshowcontext.hxx>
#include <slideshowexceptions.hxx>
#include <subsettableshapemanager.hxx>
#include <tools.hxx>
#include <transitionfactory.hxx>
#include <unoview.hxx>
#include <unoviewcontainer.hxx>
#include <usereventqueue.hxx>
#include <viewlayer.hxx>
#endif // PCH_LEVEL >= 4

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */