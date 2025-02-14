
#include "mixr/graphics/Display.hpp"

#include "mixr/graphics/fonts/IFont.hpp"
#include "mixr/graphics/Image.hpp"
#include "mixr/graphics/Texture.hpp"
#include "mixr/graphics/Material.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/colors/IColor.hpp"
#include "mixr/base/colors/Rgba.hpp"
#include "mixr/base/colors/Hsva.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/PairStream.hpp"

#include <string>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <limits>

#include <GL/glu.h>

// if OpenGL extension is not defined by glu.h, try loading glext.h
#ifndef GL_BGR_EXT
#include <GL/glext.h>
#endif

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Display, "Display")

BEGIN_SLOTTABLE(Display)
   "name",                 //  1) Display name
   "colorTable",           //  2) Color table
   "normalFont",           //  3) Normal font (AbstractFont or base::Identifier)
   "left",                 //  4) Left ortho bound
   "right",                //  5) Right ortho bound
   "bottom",               //  6) Bottom ortho bound
   "top",                  //  7) Top ortho bound
   "near",                 //  8) Near ortho bound
   "far",                  //  9) Far ortho bound
   "vpX",                  // 10) Viewport x origin
   "vpY",                  // 11) Viewport y origin
   "vpWidth",              // 12) Viewport width
   "vpHeight",             // 13) Viewport height
   "displays",             // 14) Sub-displays
   "stdLineWidth",         // 15) Standard Line width
   "textures",             // 16) Texture(s)
   "clearColor",           // 17) Clear (Background) color; base::Color
   "leftBracketChar",      // 18) Left bracket character;  base::String or base::INumber; default: '['
   "rightBracketChar",     // 19) Right bracket character; base::String or base::INumber; default: ']'
   "reverseVideoBrackets", // 20) Reverse video brackets flag:
                           //     If true, brackets are drawn with reversed video font,
                           //     otherwise follow the field's drawing mode.  default: false
   "fonts",                // 21) List of fonts
   "clearDepth",           // 22) clear depth; range: [ 0, 1 ] or negative for no depth buffer
   "orientation",          // 23) display orientation { normal, cw90, ccw90, inverted } default: normal
   "materials",            // 24) List of material objects
   "antiAliasing",         // 25) Anti-aliasing flag (on/off)
END_SLOTTABLE(Display)

BEGIN_SLOT_MAP(Display)
   ON_SLOT( 1, setSlotName,                  base::String)
   ON_SLOT( 2, setSlotColorTable,            base::PairStream)
   ON_SLOT( 3, setSlotNormalFont,            IFont)
   ON_SLOT( 3, setSlotNormalFont,            base::Identifier)
   ON_SLOT( 4, setSlotLeftOrthoBound,        base::INumber)
   ON_SLOT( 5, setSlotRightOrthoBound,       base::INumber)
   ON_SLOT( 6, setSlotBottomOrthoBound,      base::INumber)
   ON_SLOT( 7, setSlotTopOrthoBound,         base::INumber)
   ON_SLOT( 8, setSlotNearOrthoBound,        base::INumber)
   ON_SLOT( 9, setSlotFarOrthoBound,         base::INumber)
   ON_SLOT(10, setSlotViewportXOrigin,       base::Integer)
   ON_SLOT(11, setSlotViewportYOrigin,       base::Integer)
   ON_SLOT(12, setSlotViewportWidth,         base::Integer)
   ON_SLOT(13, setSlotViewportHeight,        base::Integer)
   ON_SLOT(14, setSlotSubdisplayStream,      base::PairStream)
   ON_SLOT(14, setSlotSubdisplaySingle,      Display)
   ON_SLOT(15, setSlotStdLineWidth,          base::INumber)
   ON_SLOT(16, setSlotTexturesStream,        base::PairStream)
   ON_SLOT(16, setSlotTexturesSingle,        Texture)
   ON_SLOT(17, setSlotClearColor,            base::IColor)
   ON_SLOT(18, setSlotLeftBracketCharacter,  base::Integer)
   ON_SLOT(18, setSlotLeftBracketCharacter,  base::String)
   ON_SLOT(19, setSlotRightBracketCharacter, base::Integer)
   ON_SLOT(19, setSlotRightBracketCharacter, base::String)
   ON_SLOT(20, setSlotReverseVideoBrackets,  base::Boolean)
   ON_SLOT(21, setFontList,                  base::PairStream)
   ON_SLOT(22, setSlotClearDepth,            base::INumber)
   ON_SLOT(23, setSlotDisplayOrientation,    base::Identifier)
   ON_SLOT(24, setSlotMaterials,             base::PairStream)
   ON_SLOT(24, setSlotMaterials,             Material)
   ON_SLOT(25, setSlotAntialias,             base::Boolean)
END_SLOT_MAP()

Display::Display()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Display::initData()
{
   // Colors
   color.set(1.0f,1.0f,1.0f,1.0f);
   clearColor.set(0.0f,0.0f,0.0f,0.0f);

   {
      base::PairStream* p{defaultColors()};
      setColorTable(p);
      p->unref();

      colorName = new base::Identifier();

      const auto nc = new base::Rgba(0.0, 1.0, 0.0, 1.0); // default: green
      setNormColor( nc );
      nc->unref();

      const auto hc = new base::Rgba(1.0, 0.0, 0.0, 1.0); // default: red
      setHighlightColor( hc );
      hc->unref();
   }
}

void Display::copyData(const Display& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   name = org.name;
   subdisplayFlg = org.subdisplayFlg;

   orientation = org.orientation;

   if (subdisplays != nullptr) { subdisplays->unref(); }
   if (org.subdisplays != nullptr) {
      subdisplays = org.subdisplays->clone();
      processSubdisplays();
   }
   else subdisplays = nullptr;

   if (textures != nullptr) { textures->unref(); }
   if (org.textures != nullptr) {
      textures = org.textures->clone();
      processTextures();
   }
   else textures = nullptr;

   if (materials != nullptr) materials->unref();
   if (org.materials != nullptr) {
      materials = org.materials->clone();
      processMaterials();
   }

   stdLinewidth = org.stdLinewidth;
   linewidth = org.linewidth;

   antialias = org.antialias;
   focusPtr = org.focusPtr;
   mx = org.mx;
   my = org.my;

   clearDepth = org.clearDepth;

   setColorTable(org.colorTable);
   setNormColor(org.normColor);
   setHighlightColor(org.hiColor);
   color = org.color;
   *colorName = *org.colorName;
   clearColor = org.clearColor;

   setFontList(org.fontList);
   setFont( org.currentFont );

   if (org.normalFontName != nullptr) setNormalFont( org.normalFontName );
   else setNormalFont( org.normalFont );

   oLeft = org.oLeft;
   oRight = org.oRight;
   oBottom = org.oBottom;
   oTop = org.oTop;
   oNear = org.oNear;
   oFar = org.oFar;
   vpX = org.vpX;
   vpY = org.vpY;
   vpWidth = org.vpWidth;
   vpHeight = org.vpHeight;

   leftBracketChar = org.leftBracketChar;
   rightBracketChar = org.rightBracketChar;
   reversedFlg   = org.reversedFlg;
   underlinedFlg = org.underlinedFlg;
}

void Display::deleteData()
{
   if (subdisplays != nullptr) { subdisplays->unref(); subdisplays = nullptr; }
   if (textures != nullptr) { textures->unref(); textures = nullptr; }
   if (materials != nullptr) { materials->unref(); materials = nullptr; }
   if (colorTable != nullptr) { colorTable->unref(); colorTable = nullptr; }
   if (colorName != nullptr) { colorName->unref(); colorName = nullptr; }
   if (normColor != nullptr) { normColor->unref(); normColor = nullptr; }
   if (hiColor != nullptr) { hiColor->unref(); hiColor = nullptr; }
   if (currentFont != nullptr) { currentFont->unref(); currentFont = nullptr; }
   if (normalFont != nullptr) { normalFont->unref(); normalFont = nullptr; }
   if (normalFontName != nullptr) { normalFontName->unref(); normalFontName = nullptr; }
}

//------------------------------------------------------------------------------
// updateTC() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Display::updateTC(const double dt)
{
   BaseClass::updateTC(dt);

   // Update any sub-displays ...
   if (subdisplays != nullptr) {
      base::List::Item* item{subdisplays->getFirstItem()};
      while (item != nullptr) {
         const auto pair = dynamic_cast<base::Pair*>(item->getValue());
         if (pair != nullptr) {
            const auto obj = dynamic_cast<Display*>( pair->object() );
            if (obj != nullptr) obj->tcFrame(dt);
         }
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Display::reset()
{
   BaseClass::reset();
   if (subdisplays != nullptr) {
      // Reset all of our sub-displays
      base::List::Item* item{subdisplays->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto obj = static_cast<Component*>(pair->object());
         if (obj != nullptr) obj->reset();
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// select() -- select this display
//------------------------------------------------------------------------------
void Display::select()
{
   glLineWidth(linewidth);
   lcColor4v(color.ptr());
}

//------------------------------------------------------------------------------
// keyboardEvent() -- pass keyboard events to the focus
//------------------------------------------------------------------------------
void Display::keyboardEvent(const int key)
{
   const auto focusDisplay = dynamic_cast<Display*>( focusPtr );
   if ( focusDisplay != nullptr && focusDisplay != this) {
      // When our focus is a Display ...
      focusDisplay->keyboardEvent(key);

   } else {
      // When our focus is NOT at a Display ...

      // Null pointer?  Try to set to our current subpage
      if (focusPtr == nullptr && subpage() != nullptr) focusPtr = subpage();
      if (isMessageEnabled(MSG_DEBUG)) {
         std::cout << "Display::keyboardEvent(" << this << "," << key << "), focusPtr = " << focusPtr << std::endl;
      }
      if (focusPtr != nullptr) focusPtr->event(key);
   }

}

//------------------------------------------------------------------------------
// onMouseEnter() - called when the mouse enters the display
//------------------------------------------------------------------------------
void Display::onMouseEnter()
{
   // Derive from this class and do what you want with this function call
}

//------------------------------------------------------------------------------
// onMouseExit() - called when the mouse exits the display
//------------------------------------------------------------------------------
void Display::onMouseExit()
{
   // Derive from this class and do what you want with this function call
}

//------------------------------------------------------------------------------
// buttonEvent() -- pass button events to the focus
//------------------------------------------------------------------------------
void Display::buttonEvent(const int b)
{
   // Null pointer?  Try to set to our current subpage
   if (focusPtr == nullptr && subpage() != nullptr) focusPtr = subpage();

   //if (isMessageEnabled(MSG_DEBUG)) {
   //std::cout << "Display::buttonEvent(" << this << "," << b << "), focusPtr = " << focusPtr << std::endl;
   //}
   if (focusPtr != nullptr) focusPtr->event(b);
}

//------------------------------------------------------------------------------
// mouseEvent() -- pass mouse input events to the focus
//------------------------------------------------------------------------------
void Display::mouseEvent(const int /* button */, const int /* state */, const int x, const int y)
{
   // keep track of our mouse state
   setMouse(x,y);
}

//------------------------------------------------------------------------------
// setMouse() -- sets the current mouse position
//------------------------------------------------------------------------------
void Display::setMouse(const int x, const int y, Display* const subdisplay)
{
   int lx{x};
   int ly{y};

   if (subdisplay != nullptr) {
      // When we're called from a sub-display,
      //   offset the coordinates and set the focus to the sub-display
      GLsizei sdX{}, sdY{};
      subdisplay->getViewportOrigin(&sdX, &sdY);
      lx = x + sdX;
      ly = y + sdY;
        if (focus() != nullptr && focus() != subdisplay) {
            // if our previous focus was a display, exit it properly
            if (focus()->isClassType(typeid(Display))) {
                const auto dis = static_cast<graphics::Display*>(focus());
                dis->onMouseExit();
            }
            focus(subdisplay);
            // enter our new mouse display
            subdisplay->onMouseEnter();
        }
   } else {
        // if we aren't a subdisplay, but we are a display, we
        // still need to call our entry and exit routines
        if (focus() != nullptr && focus()->isClassType(typeid(Display))) {
            const auto dis = static_cast<graphics::Display*>(focus());
            dis->onMouseExit();
        }

      // When we are NOT called from a sub-display,
      //   reset the focus to one of our own pages or components
      if (focus() != nullptr) {
         if (focus()->isClassType(typeid(Display))) {
            focus( subpage() );
         }
      }
      else focus( subpage() );
   }

   // Set our new mouse coordinates
   mx = lx;
   my = ly;

   // Send these coordinates to our parent display
   const auto parentDisplay = static_cast<Display*>(findContainerByType(typeid(Display)));
   if (parentDisplay != nullptr) {
      parentDisplay->setMouse(lx,ly,this);
   }

   // If we have no focus whatsoever at the end, we take the focus
    if (focusPtr == nullptr) {
        // call our entry procedure!
        onMouseEnter();
        focus(this);

    }
}

//------------------------------------------------------------------------------
// focus() -- set/return the input 'focus'
//------------------------------------------------------------------------------
Graphic* Display::focus() const
{
   return focusPtr;
}

Graphic* Display::focus(Graphic* s)
{
   return (focusPtr = s);
}

//------------------------------------------------------------------------------
// clear() --
//------------------------------------------------------------------------------
void Display::clear()
{
   if (clearDepth >= 0.0f) {
      glClearDepth(clearDepth);
      glEnable(GL_DEPTH_TEST);
   } else {
      glDisable(GL_DEPTH_TEST);
   }

   base::Vec4f cc = getClearColor();
   glClearColor(cc[base::IColor::RED], cc[base::IColor::GREEN], cc[base::IColor::BLUE], cc[base::IColor::ALPHA]);

   if (clearDepth >= 0.0f)
      glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
   else
      glClear(GL_COLOR_BUFFER_BIT);
}

//------------------------------------------------------------------------------
// swapbuffers() --
//------------------------------------------------------------------------------
void Display::swapBuffers()
{
}

//-----------------------------------------------------------------------------
// hide our display --
//-----------------------------------------------------------------------------
void Display::hide()
{
}

//-----------------------------------------------------------------------------
// show our display --
//-----------------------------------------------------------------------------
void Display::show()
{
}

//------------------------------------------------------------------------------
// setLineWidth() -- change the current linewidth
//------------------------------------------------------------------------------
GLfloat Display::setLinewidth(const GLfloat lw)
{
   GLfloat olw{linewidth};
   if (linewidth != lw) {
      linewidth = lw;
      glLineWidth(linewidth);
   }
   return olw;
}

//------------------------------------------------------------------------------
// setStdLineWidth() --
//------------------------------------------------------------------------------
void Display::setStdLineWidth(const GLfloat lw)
{
   if (lw != stdLinewidth) {
      stdLinewidth = lw;
      //setLinewidth(stdLinewidth);
   }
}

//------------------------------------------------------------------------------
// setAntialiasing() --
//------------------------------------------------------------------------------
bool Display::setAntialiasing(const bool on)
{
   antialias = on;
   return true;
}

//------------------------------------------------------------------------------
// setOrtho() -- set the ortho parameters (call before init())
//------------------------------------------------------------------------------
void Display::setOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   oLeft   = l;
   oRight  = r;
   oBottom = b;
   oTop    = t;
   oNear   = n;
   oFar    = f;
}

void Display::getOrtho(GLdouble &left, GLdouble &right, GLdouble &bottom, GLdouble &top, GLdouble &x, GLdouble &y) const
{
   left = oLeft;
   right = oRight;
   bottom = oBottom;
   top = oTop;
   x = oNear;
   y = oFar;
}

//-----------------------------------------------------------------------------
// forceOrtho()
//-----------------------------------------------------------------------------
void Display::forceOrtho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f)
{
   setOrtho(l,r,b,t,n,f);
   reshapeIt(vpWidth,vpHeight);
}

//-----------------------------------------------------------------------------
// drawIt() -- default function to draw the display
//-----------------------------------------------------------------------------
void Display::drawIt()
{
   select();

   glViewport(0,0,vpWidth,vpHeight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(oLeft, oRight, oBottom, oTop, oNear, oFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   configure();
   clear();

   if (getDisplayOrientation() != Orientation::NORMAL) {
      glPushMatrix();
      if (getDisplayOrientation() == Orientation::CW90)
         glRotated(-90.0, 0.0, 0.0, 1.0);
      else if (getDisplayOrientation() == Orientation::CCW90)
         glRotated(90.0, 0.0, 0.0, 1.0);
      else
         glRotated(180.0, 0.0, 0.0, 1.0);
   }

   // Draw the display
   draw();

   if (getDisplayOrientation() != Orientation::NORMAL) {
      glPopMatrix();
   }

   // Swap buffer
   if (okToSwap) swapBuffers();
}

//-----------------------------------------------------------------------------
// reshapeIt() -- default function to reshape the viewport
//-----------------------------------------------------------------------------
void Display::reshapeIt(int w, int h)
{
   vpWidth = w;
   vpHeight = h;

   if (oLeft == std::numeric_limits<double>::max()) {
      // DEFAULT values
      GLdouble a{static_cast<GLdouble>(vpHeight) / static_cast<GLdouble>(vpWidth)};
      oLeft   = -1.0;
      oRight  = 1.0;
      oBottom = (-1.0*a);
      oTop    = (1.0*a);
      oNear   = 0.0;
      oFar    = 10.0;
   }

   glViewport(0,0,vpWidth,vpHeight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(oLeft, oRight, oBottom, oTop, oNear, oFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//------------------------------------------------------------------------------
// getViewport() -- get the viewport parameters
// setViewport() -- set the viewport parameters
//------------------------------------------------------------------------------
void Display::getViewport(GLint* x, GLint* y, GLsizei* w, GLsizei* h) const
{
   *x = vpX;
   *y = vpY;
   *w = vpWidth;
   *h = vpHeight;
}

void Display::setViewport(const GLint x, const GLint y, const GLsizei w, const GLsizei h)
{
   vpX = x;
   vpY = y;
   vpWidth  = w;
   vpHeight = h;
}

//------------------------------------------------------------------------------
// getViewportOrigin() -- get the viewport origin
// setViewportOrigin() -- set the viewport origin
//------------------------------------------------------------------------------
void Display::getViewportOrigin(GLsizei* x, GLsizei* y) const
{
   *x = vpX;
   *y = vpY;
}

void Display::setViewportOrigin(const GLsizei x, const GLsizei y)
{
   vpX = x;
   vpY = y;
}


//------------------------------------------------------------------------------
// getViewportSize() -- get the viewport size
// setViewportSize() -- set the viewport size
//------------------------------------------------------------------------------
void Display::getViewportSize(GLsizei* w, GLsizei* h) const
{
   *w = vpWidth;
   *h = vpHeight;
}

void Display::setViewportSize(const GLsizei w, const GLsizei h)
{
   vpWidth  = w;
   vpHeight = h;
}

//-----------------------------------------------------------------------------
// setSubscreen() -- setup a temporary sub-viewport
//-----------------------------------------------------------------------------
void Display::setSubscreen(const GLdouble lf, const GLdouble rt, const GLdouble bt, const GLdouble tp)
{
   GLint nx{GLint( static_cast<GLdouble>(vpWidth) * ((lf - oLeft)/(oRight - oLeft)) )};
   GLint ny{GLint( static_cast<GLdouble>(vpHeight) * ((bt - oBottom)/(oTop - oBottom)) )};

   GLint nw{GLint( static_cast<GLdouble>(vpWidth) * ((rt - lf)/(oRight - oLeft)) )};
   GLint nh{GLint( static_cast<GLdouble>(vpHeight) * ((tp - bt)/(oTop - oBottom)) )};

   glViewport(nx,ny,nw,nh);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(lf, rt, bt, tp, oNear, oFar);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//-----------------------------------------------------------------------------
// clearSubscreen() -- clears the temporary sub-viewport
//-----------------------------------------------------------------------------
void Display::clearSubscreen()
{
   reshapeIt(vpWidth,vpHeight);
}

//-----------------------------------------------------------------------------
// setScissor() -- setup a scissor box and enable the test
//-----------------------------------------------------------------------------
void Display::setScissor(const GLdouble scissorLeft, const GLdouble scissorRight,
   const GLdouble sscissorBottom, const GLdouble scissorTop)
{
   // get our coordinates and transform them to window coordinates
   GLdouble objz{};

   // we have to get our model and
   GLdouble modelMatrix[16];
   glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
   GLdouble projMatrix[16];
   glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT, viewport);

   // these will hold our new values
   GLdouble winx0{};
   GLdouble winy0{};
   GLdouble winz0{};
   GLdouble winx1{};
   GLdouble winy1{};
   GLdouble winz1{};

   // ok, we have all of our data, send it to be converted to window coordinates
   gluProject(scissorLeft, sscissorBottom, objz, modelMatrix, projMatrix, viewport, &winx0, &winy0, &winz0);
   //if (isMessageEnabled(MSG_DEBUG)) {
   //std::cout << winx0 << " " << winy0 << " " << winz0 << std::endl;
   //}

   gluProject(scissorRight, scissorTop, objz, modelMatrix, projMatrix, viewport, &winx1, &winy1, &winz1);
   //if (isMessageEnabled(MSG_DEBUG)) {
   //std::cout << winx1 << " " << winy1 << " " << winz1 << std::endl;

   //std::cout << "X = " << x << "    Y = " << y << std::endl;
   //std::cout << "W = " << width << "    H = " << height << std::endl;
   //}

   // now we are going to scissor a small box using our window coordinates
   glEnable(GL_SCISSOR_TEST);
   if (getDisplayOrientation() == Orientation::CCW90) {
      // 90 degrees Counter-clockwise rotation
      GLint x{static_cast<GLint>(winx1)};
      GLint y{static_cast<GLint>(winy0)};
      GLsizei width{static_cast<GLsizei>(winx0 -  winx1)};
      GLsizei height{static_cast<GLsizei>(winy1 - winy0)};
      glScissor(x, y, width, height);
   }
   else if (getDisplayOrientation() == Orientation::CW90) {
      // 90 degrees Counter-clockwise rotation
      GLint x{static_cast<GLint>(winx0)};
      GLint y{static_cast<GLint>(winy1)};
      GLsizei width{static_cast<GLsizei>(winx1 -  winx0)};
      GLsizei height{static_cast<GLsizei>(winy0 - winy1)};
      glScissor(x, y, width, height);
   }
   else if (getDisplayOrientation() == Orientation::INVERTED) {
      // Normal
      GLint x{static_cast<GLint>(winx1)};
      GLint y{static_cast<GLint>(winy1)};
      GLsizei width{static_cast<GLsizei>(winx0 -  winx1)};
      GLsizei height{static_cast<GLsizei>(winy0 - winy1)};
      glScissor(x, y, width, height);
   }
   else {
      // Normal
      GLint x{static_cast<GLint>(winx0)};
      GLint y{static_cast<GLint>(winy0)};
      GLsizei width{static_cast<GLsizei>(winx1 -  winx0)};
      GLsizei height{static_cast<GLsizei>(winy1 - winy0)};
      glScissor(x, y, width, height);
   }
}

//-----------------------------------------------------------------------------
// clearScissor() -- clears (disables) the scissor test
//-----------------------------------------------------------------------------
void Display::clearScissor()
{
   glDisable(GL_SCISSOR_TEST);
}

//------------------------------------------------------------------------------
// setColor() -- change the current color vector
//------------------------------------------------------------------------------
void Display::setColor(const base::Vec4d& newColor)
{
   if (color != newColor) {
      color = newColor;
      colorName->empty();
      lcColor4v(color.ptr());
   }
}

void Display::setColor(const char* cname1)
{
   // Already set? Then leave
   if (*colorName == cname1) return;

   base::IColor* newColor{getColor(cname1)};
   if (newColor != nullptr) {
      colorName->setStr(cname1);
      color = *(newColor->getRGBA());
      lcColor4v(color.ptr());
   }
}

//------------------------------------------------------------------------------
// setClearColor() -- set the clear color (used by a screen clear)
//------------------------------------------------------------------------------
void Display::setClearColor(const base::IColor& ccolor)
{
   clearColor = *ccolor.getRGBA();
}

//------------------------------------------------------------------------------
// getTextureByName()  -- get a texture by name
//------------------------------------------------------------------------------
GLuint Display::getTextureByName(const base::Identifier* texName)
{
   GLuint tex{};
   if (texName != nullptr && textures != nullptr) {
      const base::Pair* pair{textures->findByName( (*texName).c_str() )};
      if (pair != nullptr) {
         const auto pt = dynamic_cast<const Texture*>( pair->object() );
         if (pt != nullptr) tex = pt->getTexture();
      }
   }
   if ( tex == 0 && isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Display::getTextureByName(): texture ";
      if (texName != nullptr) std::cerr << *texName;
      std::cerr << "  not found!" << std::endl;
   }
   return tex;
}

//------------------------------------------------------------------------------
// getMaterial()  -- gets a material by name
//------------------------------------------------------------------------------
Material* Display::getMaterial(const base::Identifier* name)
{
   Material* temp{};
   if (name !=nullptr && materials != nullptr) {
      const base::Pair* pair{materials->findByName( (*name).c_str() )};
      if (pair != nullptr) {
         const auto mat = dynamic_cast<const Material*>( pair->object() );
         if (mat != nullptr) temp = const_cast<Material*>(static_cast<const Material*>(mat));
      }
   }
   if (temp == nullptr && isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Display::getMaterial(): material \"";
      if (name != nullptr) std::cerr << *name;
      std::cerr << "\" not found!" << std::endl;
   }
   return temp;
}


//------------------------------------------------------------------------------
// setNormColor() &  setHighlightColor() -- set default text colors
//------------------------------------------------------------------------------
void Display::setNormColor(const base::IColor* const nc)
{
   if (normColor != nullptr) normColor->unref();
   normColor = nc;
   if (normColor != nullptr) normColor->ref();
}

void Display::setHighlightColor(const base::IColor* const nc)
{
   if (hiColor != nullptr) hiColor->unref();
   hiColor = nc;
   if (hiColor != nullptr) hiColor->ref();
}

//------------------------------------------------------------------------------
// setColorTable() -- set the color table
//------------------------------------------------------------------------------
bool Display::setColorTable(base::PairStream* const sctobj)
{
   bool ok{true};
   if (colorTable != nullptr) colorTable->unref();
   colorTable = sctobj;
   if (colorTable != nullptr) colorTable->ref();
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Display::setColorTable: No input available" << std::endl;
      }
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Functions to set the various fonts
//------------------------------------------------------------------------------

// setFontList() -- set the font list
bool Display::setFontList(base::PairStream* const obj)
{
   bool ok{true};
   if (fontList != nullptr) fontList->unref();
   fontList = obj;
   if (fontList != nullptr) fontList->ref();
   return ok;
}

// getFont() -- by name
IFont* Display::getFont(const char* const fontName)
{
   IFont* ft{};
   if (fontList != nullptr) {
      base::Pair* p{fontList->findByName(fontName)};
      if (p != nullptr) ft = static_cast<IFont*>(p->object());
   }
   return ft;
}
// getFont() -- by name (const version)
const IFont* Display::getFont(const char* const fontName) const
{
   const IFont* ft{};
   if (fontList != nullptr) {
      const base::Pair* p{fontList->findByName(fontName)};
      if (p != nullptr) ft = static_cast<const IFont*>(p->object());
   }
   return ft;
}

// getFont() -- by name
IFont* Display::getFont(const base::Identifier* const fontName)
{
   return getFont((*fontName).c_str());
}

// getFont() -- by name (const version)
const IFont* Display::getFont(const base::Identifier* const fontName) const
{
   return getFont((*fontName).c_str());
}

// getFont() -- by index
IFont* Display::getFont(const int index)
{
   IFont* ft {};
   if (fontList != nullptr) {
      base::Pair* p{fontList->getPosition(index+1)};
      if (p != nullptr) ft = static_cast<IFont*>(p->object());
   }
   return ft;
}

// getFont() -- by index (const version)
const IFont* Display::getFont(const int index) const
{
   const IFont* ft{};
   if (fontList != nullptr) {
      const base::Pair* p{fontList->getPosition(index+1)};
      if (p != nullptr) ft = static_cast<const IFont*>(p->object());
   }
   return ft;
}


// selectFont() -- select the current font based on mode flags
void Display::selectFont(const bool reversed, const bool underlined, IFont* newFont)
{
   // set our font initially to the one given to us
   IFont* font{newFont};
   if (font == nullptr) {
      // our font is default, since none was specified
      font = getNormalFont();
   }
   // now set this font, if we have one!
   if (font != nullptr) {
      setFont(font);
      // Save flags for reference
      reversedFlg   = reversed;
      underlinedFlg = underlined;
   }
}

// setFont() -- change the current font
void Display::setFont(IFont* newFont)
{
   if (currentFont != newFont) {
      if (currentFont != nullptr) currentFont->unref();
      currentFont = newFont;
      if (currentFont != nullptr) {
         currentFont->ref();
      }
   }
}

// getNormalFont() --- (look it up, if needed)
IFont* Display::getNormalFont()
{
   if (normalFont == nullptr && normalFontName != nullptr) {
      normalFont = getFont(normalFontName);
      if (normalFont == nullptr) {
         // Not found!
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Display::getNormalFont(): font name '" << (*normalFontName).c_str()
                      << "' not found!" << std::endl;
         }
         normalFontName->unref();
         normalFontName = nullptr;
      }
   }
   return normalFont;
}

// getNormalFont() --- (const version -- no lookup)
const IFont* Display::getNormalFont() const
{
   return normalFont;
}

// setNormalFont() ---
bool Display::setNormalFont(IFont* const f)
{
   bool ok{true};
   if (normalFont != nullptr) normalFont->unref();
   normalFont = f;
   if (normalFont != nullptr) normalFont->ref();
   return ok;
}

// setNormalFont() --- by name (character string)
bool Display::setNormalFont(const char* const fontName)
{
   if (fontName != nullptr) {
      if (normalFontName == nullptr) {
         normalFontName = new base::Identifier();
      }
      *normalFontName = fontName;
      setNormalFont( getFont(fontName) );
   } else {
      if (normalFontName != nullptr) { normalFontName->unref(); normalFontName = nullptr; }
      setNormalFont(static_cast<IFont*>(nullptr));
   }
   return true;
}

// setNormalFont() --- by name (base::Identifier)
bool Display::setNormalFont(const base::Identifier* const fontName)
{
   if (fontName != nullptr) {
      if (normalFontName == nullptr) {
         normalFontName = new base::Identifier();
      }
      *normalFontName = *fontName;
      setNormalFont( getFont(fontName) );
   } else {
      if (normalFontName != nullptr) { normalFontName->unref(); normalFontName = nullptr; }
      setNormalFont(static_cast<IFont*>(nullptr));
   }
   return true;
}

//------------------------------------------------------------------------------
// text output routines
//------------------------------------------------------------------------------

// outputTextLC() -- output a text at (Cp, Ln)
void Display::outputTextLC(const int ln, const int cp, const char* sp, const int n, const bool vf) const
{
   if (currentFont == nullptr || n <= 0) return;
   base::Vec4d ocolor{getCurrentColor()};

   const auto that = const_cast<Display*>(this);
   // If manual reverse text, draw a background polygon
   // Computer posiiton
   GLdouble x {};
   GLdouble y {};
   currentFont->position(ln,cp,x,y);
   GLdouble dx = (currentFont->getCharacterSpacing());
   GLdouble dy = (currentFont->getLineSpacing());
   std::size_t len {std::strlen(sp)};

   // If manual reverse text, draw a background polygon
   if (reversedFlg) {
      if (vf) {
         GLdouble x1 {}, y1 {};
         //currentFont->position(ln+1, cp, x1, y1);
         glPushMatrix();
         // we have to move over 1/2 a character for our xpos
         x1 = x + (dx / 2);
         y1 = y + dy;
         glTranslated(x1, y1, 0);

         GLdouble myX {(dx * len) * 0.2};
         GLdouble myY {(dy * len) + (dy / 2)};


         GLdouble vv[4][3] = {
            { -myX, -myY, -0.001 }, { -myX, 0, -0.001 }, { myX, 0, -0.001 }, { myX, -myY, -0.001 }
         };
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      } else {
         // Offsets to center to polygon
         dx *= 0.1;
         dy *= 0.2;

         GLdouble x0 {x - dx};
         GLdouble y0 {y - dy};
         GLdouble x1 {};
         GLdouble y1 {};
         currentFont->position((ln-1),(cp+n),x1,y1);
         x1 = x1 - dx;
         y1 = y1 - dy;
         GLdouble vv[4][3] = {
            { x0, y0, -0.001 }, { x1, y0, -0.001 }, { x1, y1, -0.001 }, { x0, y1, -0.001 }
         };
         glPushMatrix();
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      }

      that->setColor(getClearColor());
   }

   if (underlinedFlg) {
      GLdouble height {currentFont->getLineSpacing()};
      GLdouble width {currentFont->getCharacterSpacing()};

      // only come down about a third for underlining
      glPushMatrix();
      that->setColor(ocolor);

      GLdouble myY {}, myX {};
      if (vf) {
         // do nothing if we are vertical and underlined, because then it wouldn't be UNDERlined!
         //height /= 2;
         //myX = x - height;
         //myY = y - (width * len);
         //glBegin(GL_LINES);
         //    glVertex2f(myX, y + width);
         //    glVertex2f(myX, myY);
         //glEnd();
      } else {
         height /= 2;
         myY = y - height;
         myX = x + (width * len);
         //glTranslatef(x, y, 0);
         glBegin(GL_LINES);
         glVertex2d(x, myY);
         glVertex2d(myX, myY);
         glEnd();
      }

      glPopMatrix();
      if (reversedFlg) {
         that->setColor(getClearColor());
      }

   }

   // Output the text
   currentFont->outputText(x,y,sp,n,vf,reversedFlg);

   if (reversedFlg) {
      that->setColor(ocolor);
   }
}

// outputText() -- output a text at current position
void Display::outputText(const char* sp, const int n, const bool vf) const
{
   if (currentFont == nullptr || n <= 0) return;

   const auto that = const_cast<Display*>(this);
   base::Vec4d ocolor {getCurrentColor()};
   // If manual reverse text, draw a background polygon
   if (reversedFlg) {
      // Offsets to center to polygon
      GLdouble cSpace {(currentFont->getCharacterSpacing())};
      GLdouble lSpace {(currentFont->getLineSpacing())};
      std::size_t len {std::strlen(sp)};
      // are we vertical?
      if (vf) {
         GLdouble startX {cSpace / 2};
         GLdouble startY {};
         if (len > 1) startY = lSpace * (len / 2);
         else startY = lSpace * (len);

         startY -= (lSpace * 0.5);

         glPushMatrix();
         glTranslatef(static_cast<GLfloat>(startX), -static_cast<GLfloat>(startY), 0);

         // now add a buffer for around the edges
         GLdouble deltaX {startX + (cSpace * 0.1)};
         GLdouble deltaY {startY + (lSpace * 1.1)};

         GLdouble vv[4][3] = {
            { -deltaX, -deltaY, -0.001 }, { -deltaX, deltaY, -0.001 }, { deltaX, deltaY, -0.001 }, { deltaX, -deltaY, -0.001 }
         };
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      }
      else {
         GLdouble cSpace {currentFont->getCharacterSpacing()};
         GLdouble lSpace {currentFont->getLineSpacing()};
         std::size_t len {std::strlen(sp)};

         GLdouble startX {(cSpace * len / 2)};
         GLdouble startY {(lSpace / 2)};

         glPushMatrix();

         glTranslated(startX, startY, 0);

         GLdouble deltaX {startX + (cSpace * 0.1)};
         GLdouble deltaY {startY + (lSpace * 0.1)};

         GLdouble vv[4][3] = {
            { -deltaX, -deltaY, -0.001 }, { -deltaX, deltaY, -0.001 }, { deltaX, deltaY, -0.001 }, { deltaX, -deltaY, -0.001 }
         };
         glBegin(GL_POLYGON);
         for (int i = 0; i < 4; i++) {
            glVertex3dv( vv[i] );
         }
         glEnd();
         glPopMatrix();
      }
      that->setColor(getClearColor());
   }
   if (underlinedFlg) {
      GLdouble height {currentFont->getLineSpacing()};
      GLdouble width {currentFont->getCharacterSpacing()};
      std::size_t len {std::strlen(sp)};

      if (vf) {
         // do nothing if we are vertical and underlined, because then it wouldn't be UNDERlined!
         //height /= 2;
         //myX = x - height;
         //myY = y - (width * len);
         //glBegin(GL_LINES);
         //    glVertex2f(myX, y + width);
         //    glVertex2f(myX, myY);
         //glEnd();
      }
      else {
         // only come down about a third for underlining
         height /= 2;
         that->setColor(ocolor);
         glPushMatrix();
         glBegin(GL_LINES);
         glVertex2d(0, -height);
         glVertex2d(width * len, -height);
         glEnd();
         glPopMatrix();
         if (reversedFlg) {
            that->setColor(getClearColor());
         }

      }

   }

   currentFont->outputText(sp,n,vf,reversedFlg);

   // Switch back to the original color
   if (reversedFlg) that->setColor(ocolor);
}


//------------------------------------------------------------------------------
// draw the brackets
//------------------------------------------------------------------------------
void Display::drawLeftBracket(const int ln, const int cp)
{
   // Select reversed video?
   bool oReversedFlg {reversedFlg};
   if (getReverseVideoBrackets() && !oReversedFlg) {
      // select reverse font
      selectFont(true, underlinedFlg);
   }

   // Draw the left bracket
   char cbuf[2] {};
   cbuf[0] = getLeftBracketCharacter();
   cbuf[1] = '\0';
   outputTextLC(ln, cp, cbuf, 1, reversedFlg);

   // Switch back to the old font
   if (getReverseVideoBrackets() && !oReversedFlg) {
      selectFont(oReversedFlg, underlinedFlg);
   }
}

void Display::drawRightBracket(const int ln, const int cp)
{
   // Select reversed video?
   bool oReversedFlg {reversedFlg};
   if (getReverseVideoBrackets() && !oReversedFlg) {
      // select reverse font
      selectFont(true, underlinedFlg);
   }

   // Draw the right bracket
   char cbuf[2] {};
   cbuf[0] = getRightBracketCharacter();
   cbuf[1] = '\0';
   outputTextLC(ln, cp, cbuf, 1, reversedFlg);

   // Switch back to the old font
   if (getReverseVideoBrackets() && !oReversedFlg) {
      selectFont(oReversedFlg, underlinedFlg);
   }
}

//------------------------------------------------------------------------------
// getColor() -- get color by index or color name
//------------------------------------------------------------------------------
base::IColor* Display::getColor(const char* const colorName)
{
   base::IColor* cc {};
   if (colorTable != nullptr) {
      base::Pair* p = colorTable->findByName(colorName);
      if (p != nullptr) cc = static_cast<base::IColor*>(p->object());
   }
   return cc;
}

base::IColor* Display::getColor(const int index)
{
   base::IColor* cc {};
   if (colorTable != nullptr) {
      base::Pair* p = colorTable->getPosition(index+1);
      if (p != nullptr) cc = static_cast<base::IColor*>(p->object());
   }
   return cc;
}

//-----------------------------------------------------------------------------
// addColor() -- add a color to the color table.
//-----------------------------------------------------------------------------
void Display::addColor(base::IColor* cc)
{
   if (cc != nullptr && colorTable != nullptr) {
      int i = colorTable->entries();
      char cbuf[20];
      std::sprintf(cbuf,"%i",i);
      colorTable->put( new base::Pair(cbuf, cc) );
   }
}

void Display::addColor(base::Pair* pp)
{
   if (pp != nullptr && colorTable != nullptr) {
      base::Object* obj = pp->object();
      if (obj->isClassType(typeid(base::IColor))) {
         colorTable->put( pp );
      }
   }
}

//-----------------------------------------------------------------------------
// defaultColors() -- loads a pre-refed() Pairstream with default colors.
//-----------------------------------------------------------------------------
base::PairStream* Display::defaultColors()
{
   // allocate our new colortable
   const auto defColorTable{new base::PairStream()};

   // black
   {
      const auto color{new base::Rgba(0.0f, 0.0f, 0.0f, 1.0f)};
      const auto pair{new base::Pair("black", color)};
      defColorTable->put(pair);
      // now unref our local variables, because our pair ref()'d the Rgba object, and
      // PairStream ref()'d the pair.
      color->unref();
      pair->unref();
   }
   // red
   {
      const auto color{new base::Rgba(1.0f, 0.0f, 0.0f, 1.0f)};
      const auto pair{new base::Pair("red", color)};
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // green
   {
      const auto color{new base::Rgba(0.0f, 1.0f, 0.0f, 1.0f)};
      const auto pair{new base::Pair("green", color)};
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // yellow
   {
      const auto color{new base::Rgba(1.0f, 1.0f, 0.0f, 1.0f)};
      const auto pair{new base::Pair("yellow", color)};
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // blue
   {
      const auto color{new base::Rgba(0.0f, 0.0f, 1.0f, 1.0f)};
      const auto pair{new base::Pair("blue", color)};
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // magenta
   {
      const auto color{new base::Rgba(1.0f, 0.0f, 1.0f, 1.0f)};
      const auto pair{new base::Pair("magenta", color)};
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // cyan
   {
      const auto color{new base::Rgba(0.0f, 1.0f, 1.0f, 1.0f)};
      const auto pair{new base::Pair("cyan", color)};
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }
   // white
   {
      const auto color{new base::Rgba(1.0f, 1.0f, 1.0f, 1.0f)};
      const auto pair{new base::Pair("white", color)};
      defColorTable->put(pair);
      color->unref();
      pair->unref();
   }

   return defColorTable;
}


//------------------------------------------------------------------------------
// setName() -- set the display name
//------------------------------------------------------------------------------
bool Display::setName(const base::String* const x)
{
   name = x->c_str();
   return true;
}

bool Display::setSlotName(const base::String* const x)
{
   return setName(x);
}

bool Display::setSlotColorTable(base::PairStream* const x)
{
    return setColorTable(x);
}

bool Display::setSlotNormalFont(IFont* const x)
{
    return setNormalFont(x);
}

bool Display::setSlotNormalFont(const base::Identifier* const x)
{
    return setNormalFont(x);
}

//------------------------------------------------------------------------------
// setLeftOrthoBound() -- set left orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotLeftOrthoBound(const base::INumber* const x)
{
   oLeft = x->asDouble();
   return true;
}

//------------------------------------------------------------------------------
//  setSlotMaterials() -- set our list of materials
//------------------------------------------------------------------------------
bool Display::setSlotMaterials(base::PairStream* const x)
{
   if (materials != nullptr) materials->unref();
   materials = x;
   materials->ref();
   return processMaterials();
}

//------------------------------------------------------------------------------
// setSlotMaterials -- one material
//------------------------------------------------------------------------------
bool Display::setSlotMaterials(Material* const x)
{
   if (materials != nullptr) materials->unref();
   materials = new base::PairStream();
   materials->put(new base::Pair("1", x));
   return processMaterials();
}

//------------------------------------------------------------------------------
// setSlotAntialias -- turn on/off anti aliasing
//------------------------------------------------------------------------------
bool Display::setSlotAntialias(const base::Boolean* const x)
{
   return setAntialiasing(x->asBool());
}

//------------------------------------------------------------------------------
// setRightOrthoBound() -- set right orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotRightOrthoBound(const base::INumber* const x)
{
   oRight = x->asDouble();
   return true;
}

//------------------------------------------------------------------------------
// setBottomOrthoBound() -- set bottom orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotBottomOrthoBound(const base::INumber* const x)
{
   oBottom = x->asDouble();  // set bottom ortho bound
   return true;
}

//------------------------------------------------------------------------------
// setTopOrthoBound() --  set top orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotTopOrthoBound(const base::INumber* const x)
{
   oTop = x->asDouble();  // set top ortho bound
   return true;
}

//------------------------------------------------------------------------------
// setNearOrthoBound() -- set near orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotNearOrthoBound(const base::INumber* const x)
{
   oNear = x->asDouble();
   return true;
}


//------------------------------------------------------------------------------
// setFarOrthoBound() -- set far orthogonal bound
//------------------------------------------------------------------------------
bool Display::setSlotFarOrthoBound(const base::INumber* const x)
{
   oFar = x->asDouble();
   return true;
}

//------------------------------------------------------------------------------
// setViewportXOrigin() -- set viewport x origin
//------------------------------------------------------------------------------
bool Display::setSlotViewportXOrigin(const base::Integer* const x)
{
   vpX = x->asInt();
   return true;
}

//------------------------------------------------------------------------------
// setViewportYOrigin() -- set viewport y origin
//------------------------------------------------------------------------------
bool Display::setSlotViewportYOrigin(const base::Integer* const x)
{
   vpY = x->asInt();
   return true;
}

//------------------------------------------------------------------------------
// setViewportWidth() -- set viewport width
//------------------------------------------------------------------------------
bool Display::setSlotViewportWidth(const base::Integer* const x)
{
   vpWidth = x->asInt();
   return true;
}

//------------------------------------------------------------------------------
// setViewportHeight() -- set viewport height
//------------------------------------------------------------------------------
bool Display::setSlotViewportHeight(const base::Integer* const x)
{
   vpHeight = x->asInt();
   return true;
}

//------------------------------------------------------------------------------
//  setSlotSubdisplayStream() -- it takes a pair stream
//------------------------------------------------------------------------------
bool Display::setSlotSubdisplayStream (base::PairStream* const x)
{
   // When a PairStream (i.e., more than one, a list) of displays
   if (subdisplays != nullptr) subdisplays->unref();
   subdisplays = x;
   subdisplays->ref();
   return processSubdisplays();
}

//------------------------------------------------------------------------------
//  setSlotSubdisplaySingle() -- it takes a display
//------------------------------------------------------------------------------
bool Display::setSlotSubdisplaySingle(Display* const x)
{
   if (subdisplays != nullptr) subdisplays->unref();
   subdisplays = new base::PairStream();
   subdisplays->put(new base::Pair("1", x));
   return processSubdisplays();
}

//------------------------------------------------------------------------------
//  setSlotTexturesStream() -- it takes a pair stream
//------------------------------------------------------------------------------
bool Display::setSlotTexturesStream (base::PairStream* const obj)
{
   // When a PairStream (i.e., more than one, a list) of displays
   if (textures != nullptr) textures->unref();
   textures = obj;
   textures->ref();
   return processTextures();
}

//------------------------------------------------------------------------------
//  setSlotTexturesSingle() -- it takes a texture
//------------------------------------------------------------------------------
bool Display::setSlotTexturesSingle(Texture* const x)
{
   if (textures != nullptr) textures->unref();
   textures = new base::PairStream();
   textures->put(new base::Pair("1", x));
   return processTextures();
}

//------------------------------------------------------------------------------
//  setSlotStdLineWidth() -- sets the standard line width for the display
//------------------------------------------------------------------------------
bool Display::setSlotStdLineWidth(const base::INumber* const x)
{
   setStdLineWidth( static_cast<GLfloat>(x->asDouble()) );
   return true;
}

//------------------------------------------------------------------------------
// setSlotClearColor() -- sets the clear color slot
//------------------------------------------------------------------------------
bool Display::setSlotClearColor(const base::IColor* const x)
{
   setClearColor(*x);
   return true;
}

//------------------------------------------------------------------------------
// setSlotLeftBracketCharacter() -- sets the left bracket character
//------------------------------------------------------------------------------
bool Display::setSlotLeftBracketCharacter(const base::Integer* const x)
{
   int num {x->asInt()};
   if (std::isalnum(num)) {
      setLeftBracketCharacter(char(num));
   }
   return true;
}

bool Display::setSlotLeftBracketCharacter(const base::String* const x)
{
   const char* str {(*x).c_str()};
   if (str != nullptr) {
      setLeftBracketCharacter(str[0]);
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotRightBracketCharacter() -- sets the right bracket character
//------------------------------------------------------------------------------
bool Display::setSlotRightBracketCharacter(const base::Integer* const x)
{
   int num {x->asInt()};
   if (std::isalnum(num)) {
      setRightBracketCharacter(char(num));
   }
   return true;
}

bool Display::setSlotRightBracketCharacter(const base::String* const x)
{
   const char* str {(*x).c_str()};
   if (str != nullptr) {
      setRightBracketCharacter(str[0]);
   }
   return true;
}

//------------------------------------------------------------------------------
// setSlotReverseVideoBrackets() -- set reverse video brackets flag
//------------------------------------------------------------------------------
bool Display::setSlotReverseVideoBrackets(const base::Boolean* const x)
{
   return setReverseVideoBrackets(x->asBool());
}

//------------------------------------------------------------------------------
// setSlotClearDepth() -- sets the clear depth buffer slot
//------------------------------------------------------------------------------
bool Display::setSlotClearDepth(const base::INumber* const x)
{
   setClearDepth(x->asDouble());
   return true;
}

//------------------------------------------------------------------------------
// setSlotDisplayOrientation() -- sets display orientation slot
//                                 ( normal, cw90, ccw90, inverted }
//------------------------------------------------------------------------------
bool Display::setSlotDisplayOrientation(const base::Identifier* const x)
{
   bool ok {};
   if (*x == "normal")        { setDisplayOrientation(Orientation::NORMAL);   ok = true; }
   else if (*x == "cw90")     { setDisplayOrientation(Orientation::CW90);     ok = true; }
   else if (*x == "ccw90")    { setDisplayOrientation(Orientation::CCW90);    ok = true; }
   else if (*x == "inverted") { setDisplayOrientation(Orientation::INVERTED); ok = true; }
   return ok;
}

//------------------------------------------------------------------------------
// processSubdisplays() -- process our subdisplays; make sure they are all of
// type Display (or derived from it)and tell them that we are their
// container.
//------------------------------------------------------------------------------
bool Display::processSubdisplays()
{
   bool ok {true};
   if (subdisplays != nullptr) {
      const base::List::Item* item {subdisplays->getFirstItem()};
      while (ok && item != nullptr) {
         const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
         item = item->getNext();
         const auto g = dynamic_cast<Display*>(p->object());
         if (g != nullptr) {
            g->container(this);
            g->setSubdisplayFlag(true);
         }
         else {
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Display::processSubdisplays(): " << p->slot() << " is not a Display!" << std::endl;
            }
            ok = false;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// processTextures() -- process our textures; make sure they are all of
// type Texture (or derived from it)and tell them to load.
//------------------------------------------------------------------------------
bool Display::processTextures()
{
   bool ok {true};
   if (textures != nullptr) {
      const base::List::Item* item {textures->getFirstItem()};
      while (ok && item != nullptr) {
         const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
         item = item->getNext();
         const auto g = dynamic_cast<Texture*>(p->object());
         if (g == nullptr) {
            // It MUST be of type Texture
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Display::processTextures(): " << p->slot() << " is not a Texture!" << std::endl;
            }
            ok = false;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// processMaterials() -- process our materials, makes sure all of our items
// are of type Material or derived from it.
//------------------------------------------------------------------------------
bool Display::processMaterials()
{
   bool ok {true};
   if (materials != nullptr) {
      const base::List::Item* item {materials->getFirstItem()};
      while (ok && item != nullptr) {
         const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
         item = item->getNext();
         const auto g = dynamic_cast<Material*>(p->object());
         if (g == nullptr) {
            // It MUST be of type Material
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Display::processMaterials(): " << p->slot() << " is not a Material!" << std::endl;
            }
            ok = false;
         }
      }
   }
   return ok;
}


//-----------------------------------------------------------------------------
// configureIt() -- function to configure the display
//-----------------------------------------------------------------------------
void Display::configure()
{
   // Set antialiasing ON
   if (isAntialiasing()) {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
      glEnable(GL_ALPHA_TEST);
   }
   else {
      // Set antialiasing OFF
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
      glDisable(GL_BLEND);
      glBlendFunc(GL_ONE,  GL_ZERO);
      glDisable(GL_ALPHA_TEST);
   }

   // Depth checks
   if (getClearDepth() >= 0.0f) {
      glEnable(GL_DEPTH_TEST);
   }
   else {
      glDisable(GL_DEPTH_TEST);
   }

   // Disable other stuff
   glDisable(GL_DITHER);
   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);
   glDisable(GL_LOGIC_OP);
   glDisable(GL_STENCIL_TEST);
   glDisable(GL_TEXTURE_1D);
   glDisable(GL_TEXTURE_2D);

   // Default pixel transfer settings
   glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
   glPixelTransferi(GL_RED_SCALE, 1);
   glPixelTransferi(GL_RED_BIAS, 0);
   glPixelTransferi(GL_GREEN_SCALE, 1);
   glPixelTransferi(GL_GREEN_BIAS, 0);
   glPixelTransferi(GL_BLUE_SCALE, 1);
   glPixelTransferi(GL_BLUE_BIAS, 0);
   glPixelTransferi(GL_ALPHA_SCALE, 1);
   glPixelTransferi(GL_ALPHA_BIAS, 0);

#if GL_SGI_index_func
   glDisable(GL_INDEX_TEST_SGI);
#endif
}

//------------------------------------------------------------------------------
// loadTextures() -- This gets called when we want our list of textures
// to "load" themselves.  Not all textures load the same, from Bitmap textures
// to textures loaded by map software.  So we will leave it up to the texture
// itself to load itself appropriately.
//------------------------------------------------------------------------------
void Display::loadTextures()
{
   if (textures != nullptr) {
      const base::List::Item* item {textures->getFirstItem()};
      while (item != nullptr) {
         const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
         item = item->getNext();
         Texture* g {static_cast<Texture*>(p->object())};
         g->loadTexture();
      }
   }
}


//------------------------------------------------------------------------------
// Read all pixels from the frame buffer and return buffer pointer
//------------------------------------------------------------------------------
Image* Display::readFrameBuffer(unsigned int* const w, unsigned int* const h)
{
   // Get Viewport width and height
   GLsizei vpWidth {};
   GLsizei vpHeight {};
   getViewportSize(&vpWidth, &vpHeight);
   unsigned int width {static_cast<unsigned int>(vpWidth)};
   unsigned int height {static_cast<unsigned int>(vpHeight)};

   Image* image {readFrameBuffer(0, 0, width, height)};

   if (w != nullptr) *w = width;
   if (h != nullptr) *h = height;
   return image;
}

//------------------------------------------------------------------------------
// Get selected pixels from the frame buffer and return buffer pointer
//------------------------------------------------------------------------------
Image* Display::readFrameBuffer(const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height)
{
   static const unsigned int MAX_IMAGE_WIDTH  {2048}; // maximum image width
   static const unsigned int MAX_IMAGE_HEIGHT {2048}; // maximum image height
   static const unsigned int PIXEL_SIZE {3};          // pixel size in bytes { GL_BGR_EXT }

   // check input parameters
   if ((width < 1) || (height < 1) || (width > MAX_IMAGE_WIDTH) || (height > MAX_IMAGE_HEIGHT)) {
      std::cout << "Display::readGlBuffer(): Invalid screenshot dimensions" << std::endl;
      return nullptr;
   }

   // align width to 4-byte boundary:
   unsigned int size = {(width + 3) / 4};
   unsigned int w {size * 4};

   const auto pixelData = new GLubyte[PIXEL_SIZE * w * height];
   glReadPixels(x, y, w, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelData);

   const auto newImage = new Image(w, height, PIXEL_SIZE, GL_BGR_EXT, pixelData);

   return newImage;
}

}
}
