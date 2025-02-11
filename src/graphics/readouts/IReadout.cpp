
#include "mixr/graphics/readouts/IReadout.hpp"

#include "mixr/graphics/Display.hpp"
#include "mixr/graphics/Page.hpp"
#include "mixr/graphics/Display.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/colors/Rgb.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_ABSTRACT_SUBCLASS(IReadout, "IReadout")

BEGIN_SLOTTABLE(IReadout)
    "position",         // 1) Starting position ( ln cp )
    "width",            // 2) Field width
    "highLight",        // 3) Highlight text flag
    "underline",        // 4) Underlined text flag
    "reversed",         // 5) Reversed text flag
    "justification",    // 6) { none, left, center, right }
    "vertical",         // 7) vertical field flag (text displayed vertically)
    "brackets",         // 8) Bracket flag
    "linked",           // 9) Linked flag (auto step to next input field)
    "inheritColor",     //10) Inherit color of our container (instead of using default color)
    "font",             //11) Type of font to use before drawing
    "startCharPos",     //12) Our starting character position (we may want to skip!)
END_SLOTTABLE(IReadout)

BEGIN_SLOT_MAP(IReadout)
    ON_SLOT(1,  setSlotPosition,      base::List)
    ON_SLOT(2,  setSlotWidth,         base::Integer)
    ON_SLOT(3,  setSlotHighlight,     base::Boolean)
    ON_SLOT(4,  setSlotUnderline,     base::Boolean)
    ON_SLOT(5,  setSlotReversed,      base::Boolean)
    ON_SLOT(6,  setSlotJustification, base::Identifier)
    ON_SLOT(7,  setSlotVertical,      base::Boolean)
    ON_SLOT(8,  setSlotBrackets,      base::Boolean)
    ON_SLOT(9,  setSlotLinked,        base::Boolean)
    ON_SLOT(10, setSlotInheritColor,  base::Boolean)
    ON_SLOT(11, setSlotFont,          base::Identifier)
    ON_SLOT(12, setSlotStartCharPos,  base::Integer)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(IReadout)
    if (mode == Mode::Input) {
        bool kb {( _event >= 0x20 && _event <= 0x7f )};
        ON_EVENT(FORWARD_SPACE,onForwardSpace)
        ON_EVENT(BACK_SPACE,onBackSpace)
        // Keyboard Entry
        if (kb) {
            // Filter the input event -- that is, let a virtual member
            // function filter the input event using the current template
            // character.
            char nc{filterInputEvent(_event, inputExample[icp])};
            if (nc != '\0') {
                setChar(nc);
                _used = true;
            }
        }
    }
    ON_EVENT_OBJ(SET_POSITION,      setPosition,          base::List)
    ON_EVENT_OBJ(SET_LINE,          onSetLine,            base::Integer)
    ON_EVENT_OBJ(SET_COLUMN,        onSetColumn,          base::Integer)
    ON_EVENT_OBJ(SET_WIDTH,         setSlotWidth,         base::Integer)
    ON_EVENT_OBJ(SET_HIGHLIGHT,     setSlotHighlight,     base::Boolean)
    ON_EVENT_OBJ(SET_UNDERLINE,     setSlotUnderline,     base::Boolean)
    ON_EVENT_OBJ(SET_REVERSED,      setSlotReversed,      base::Boolean)
    ON_EVENT_OBJ(SET_JUSTIFICATION, setSlotJustification, base::Identifier)
END_EVENT_HANDLER()

IReadout::IReadout()
{
    STANDARD_CONSTRUCTOR()
    jmode = base::Justify::None;
}

void IReadout::copyData(const IReadout& org, const bool)
{
    BaseClass::copyData(org);
    origStr = org.origStr;
    inputExample = org.inputExample;
    ln      = org.ln;
    cp      = org.cp;
    w       = org.w;
    str     = org.str;
    dmode   = org.dmode;
    jmode   = org.jmode;
    mode    = org.mode;
    icp     = org.icp;
    inpDspMode = org.inpDspMode;
    inpModeHold = org.inpModeHold;
    linked = org.linked;
    inheritColor = org.inheritColor;
    startCP = org.startCP;
    fontName = org.fontName;
}

void IReadout::deleteData()
{
    origStr.clear();
    inputExample.clear();
    ln = 0;
    cp = 0;
    w  = 0;
    str.clear();
    dmode = 0;
    jmode = base::Justify::None;
    mode  = Mode::Display;
    icp   = 0;
    inpDspMode = 0;
    inpModeHold = false;
    fontName.clear();
}

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void IReadout::updateData(const double dt)
{
    BaseClass::updateData(dt);

    // ---
    // Set display mode when this field is in 'input' mode
    // ---
    if (inpDspMode != 0) {
        // Auto switch ON input display mode?
        if ( (mode == Mode::Input) &&  (icp > 0 || !inpModeHold) ) {
            setDisplayMode( inpDspMode );
        }

        // Auto swtich OFF input display mode?
        if ( (mode == Mode::Display) || (inpModeHold && icp == 0) ) {
            // If it wasn't previously set by a setSlot() function then turn it off.
            if ( !isDisplayMode(0x1000 & inpDspMode) ) {
                clearDisplayMode( inpDspMode );
            }
        }
    }

    // ---
    // Update readout during input mode?
    // ---
    if ( (mode == Mode::Input) && inpModeHold && icp == 0 ) {
        adjust();
    }
}

//------------------------------------------------------------------------------
// withinField() -- Return True/False if cp,ln is in the field space
//------------------------------------------------------------------------------
bool IReadout::withinField(const int l, const int c) const
{
    bool stat{};
    if ( (l == ln) && (c >= cp) && (c <= (cp + static_cast<int>(w) - 1)) ) stat = true;
    return stat;
}

//------------------------------------------------------------------------------
// setText() -- set the field's text string
//------------------------------------------------------------------------------
void IReadout::setText(const char s[])
{
    origStr = s;
    if (mode == Mode::Display) {
        adjust();
    }
}

//------------------------------------------------------------------------------
// justification() --
//------------------------------------------------------------------------------
base::Justify IReadout::justification() const
{
    return jmode;
}

base::Justify IReadout::justification(const base::Justify t)
{
    jmode = t;
    if (mode == Mode::Display) {
        adjust();
    }
    return jmode;
}

//------------------------------------------------------------------------------
// setMode() -- set the mode of the field (display, input)
//------------------------------------------------------------------------------
IReadout::Mode IReadout::setMode(const IReadout::Mode nmode)
{
    Mode omode = mode;
    mode = nmode;

    if (nmode == Mode::Input && omode == Mode::Display) {

        // When we're entering the INPUT mode ...

        // Change the input focus to this field
        getDisplay()->focus(this);

        // Set the input chararcter pointer to the first character
        if (startCP > 0 && startCP < w) icp = startCP;
        else icp = 0;

    } else if (nmode == Mode::Display && omode == Mode::Input) {

        // When we're leaving the INPUT mode ...

        // Change the input focus to our container page (Page)
        if (getDisplay()->focus() == this) {
            Page* page = static_cast<Page*>(findContainerByType(typeid(Page)));
            if (page != nullptr)
                getDisplay()->focus(page);
            else
                getDisplay()->focus(nullptr);

            // Reset text string
            adjust();

        }
    }
    return omode;
}


//------------------------------------------------------------------------------
// input mode functions --
//------------------------------------------------------------------------------

// isValidInputPosition() -- Makes sure the position is valid for input
bool IReadout::isValidInputPosition(const int tc)
{
    return (tc == '+' || tc == '0' || tc == '#' ||
            tc == 'D' || tc == 'H' || tc == 'M' || tc == 'S');
}

// filterInputEvent() -- Filter input events using a template character (x)
char IReadout::filterInputEvent(const int event, const char x)
{
    const char tc{static_cast<int>(x)};
    if (tc == '+') {
        // Default sign keys
        if (event == '7') {
            return '-';
        } else if (event == '9') {
            return '+';
        } else {
            return '\0';
        }
    } else if (tc == '0' || tc == '#') {
        // Default numeric keys
        if (event >= '0' && event <= '9') {
            return char(event);
        } else {
            return '\0';
        }
    } else {
        return '0';
    }
}

std::size_t IReadout::setExample(const char* const example)
{
    inputExample = example;
    return static_cast<int>(inputExample.length());
}

void IReadout::advanceSpace(const int ns)
{
    if (mode != Mode::Input) return;
    icp += ns;
    while ( icp < static_cast<int>(w) && !isValidInputPosition(inputExample[icp]) ) icp++;
    if (icp >= static_cast<int>(w)) {
        icp = static_cast<int>(w)-1;
        while ( !isValidInputPosition(inputExample[icp]) ) icp--;
        event(INPUT_RIGHT_EDGE);
    }
}

void IReadout::backSpace(const int ns)
{
   if (mode != Mode::Input) return;
   // if we are backspacing, and we are at the starting character position that was set, we stay there!
   if (startCP > 0 && icp == static_cast<int>(startCP)) {
      event(INPUT_LEFT_EDGE);
   }
   else {
      icp -= ns;
      while ( icp >= 0 && !isValidInputPosition(inputExample[icp]) ) icp--;
      if (icp < 0) {
         icp = 0;
         while ( !isValidInputPosition(inputExample[icp]) ) icp++;
         event(INPUT_LEFT_EDGE);
      }
   }
}

bool IReadout::setInputCharacterPosition(const unsigned int ii)
{
    if (startCP > 0) {
        if (ii >= startCP) icp = ii;
    }
    else if (ii < w) icp = ii;
    return true;
}


char IReadout::getChar()
{
    if (mode == Mode::Input)
        return str.getChar(icp);
    else
        return '\0';
}

void IReadout::setChar(const char c)
{
    if (mode != Mode::Input) return;
    str.setChar(icp,c);
    advanceSpace();
}

double IReadout::getInputValue() const
{
    return 0.0;
}

bool IReadout::isInputValueValid() const
{
    return true;
}

bool IReadout::onForwardSpace()
{
    advanceSpace();
    return true;
}

bool IReadout::onBackSpace()
{
    backSpace();
    return true;
}

//------------------------------------------------------------------------------
// cursor() -- Returns true if text cursor should be seen within this
//             object and the position of the cursor.
//------------------------------------------------------------------------------
bool IReadout::cursor(int* l, int* c) const
{
    if (mode == Mode::Input) {
        *l = ln;
        *c = cp + icp;
        return true;
    }
    else {
        *l = 0;
        *c = 0;
        return false;
    }
}

//------------------------------------------------------------------------------
// draw this text field
//------------------------------------------------------------------------------
void IReadout::drawFunc()
{
    // Get a pointer to the current display
    graphics::Display* dsp {getDisplay()};
    if (dsp == nullptr) return;

    // ---
    // When our container is also a Field, get a pointer to it.
    // ---
    IReadout* parent {};
    if (container() != nullptr) {
        const auto fp = dynamic_cast<IReadout*>(container());
        if (fp != nullptr) parent = fp;
    }

    // ---
    // If we don't have a position, try to get one from our container
    // ---
    int ll {line()};
    int cc {column()};
    if (ll == 0 && parent != nullptr) {
        ll = parent->line();
        cc = parent->column();
    }

    // ---
    // Select the correct font based on font name if there is one, and if not, then do it normally
    // ---
    if (!fontName.empty()) dsp->selectFont(isReversed(), isUnderlined(), dsp->getFont(fontName.c_str()));
    else dsp->selectFont(isReversed(), isUnderlined());


    // ---
    // Set the color
    // ---
    bool restoreColor {};
    base::Vec4d ocolor {dsp->getCurrentColor()};
    // only use default colors if we aren't inheriting our container's colors

    if (!isInheritColor()) {
        if (getColorName() == nullptr && getColor() == nullptr) {
            const base::IColor* cc = nullptr;
            if (isHighLighted()) cc = dsp->getHighlightColor();
            else cc = dsp->getNormColor();
            if (cc != nullptr) {
                const base::Vec4d* p = cc->getRGBA();
                dsp->setColor(*p);
                restoreColor = true;
            }
        }
    }

    // ---
    // draw the string
    // ---

    if (str.len() > 0) {
        // Draw the text string
        const char* sp {str.c_str()};
        if (ll > 0 && cc > 0)
            dsp->outputTextLC(ll, cc, sp, static_cast<int>(width()), isVertical());
        else
            dsp->outputText(sp, static_cast<int>(width()), isVertical());
    }

    // ---
    // draw the brackets
    // ---
    if (areBracketsOn() && ll > 0 && cc > 0) {
        if (isVertical()) {
            // Position for vertical text
            dsp->drawLeftBracket(ll-1, cc);
            dsp->drawRightBracket(ll + static_cast<int>(width()), cc);
        } else {
            // Position for normal text
            dsp->drawLeftBracket(ll, cc-1);
            dsp->drawRightBracket(ll, cc + static_cast<int>(width()));
        }
    }

    // ---
    // If we used default colors, restore the old value
    // ---
    if (restoreColor)
        dsp->setColor(ocolor);
}

//------------------------------------------------------------------------------
// setPosition() -- set position: [ Line Column ]
//------------------------------------------------------------------------------
bool IReadout::setPosition(const base::List* const x)
{
    bool ok {true};
    if (x != nullptr) {
        int values[2] {};
        std::size_t n {x->getNumberList(values, 2)};
        if (n >= 2) {
            line(values[0]);
            column(values[1]);
        } else {
             if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "AbstractField::setPosition: not enough data to process list" << std::endl;
             }
             ok = false;
        }
    }
    return ok;
}

bool IReadout::onSetLine(const base::Integer* const x)
{
    if (x != nullptr) line(x->asInt());
    return true;
}

bool IReadout::onSetColumn(const base::Integer* const x)
{
   if (x != nullptr) column(x->asInt());
   return true;
}

bool IReadout::setSlotPosition(const base::List* const x)
{
   return setPosition(x);
}

bool IReadout::setSlotWidth(const base::Integer* const x)
{
    if (x != nullptr) width(x->asInt());
    return true;
}

bool IReadout::setSlotHighlight(const base::Boolean* const x)
{
    if (x != nullptr) {
        // Set our mode
        if (x->asBool()) {
            setDisplayMode(highlight);
        } else {
            clearDisplayMode(highlight);
        }

        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {
            const base::List::Item* item {subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto p{const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()))};
                const auto child{dynamic_cast<IReadout*>(p->object())};
                if (child != nullptr) child->setSlotHighlight(x); //changed from obj
                item = item->getNext();
            }
            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return true;
}

bool IReadout::setSlotUnderline(const base::Boolean* const x)
{
    if (x != nullptr) {

        // Set our mode
        if (x->asBool()) {
            setDisplayMode(underline);
        } else {
            clearDisplayMode(underline);
        }

        // Set our children's mode
        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {
            const base::List::Item* item {subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto p{const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()))};
                const auto child{dynamic_cast<IReadout*>(p->object())};
                if (child != nullptr) child->setSlotUnderline(x);
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return true;
}

bool IReadout::setSlotReversed(const base::Boolean* const x)
{
    if (x != nullptr) {

        // Set our mode
        if (x->asBool()) {
            setDisplayMode(reversed);
        } else {
            clearDisplayMode(reversed);
        }

        // Set our children's mode
        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {
            const base::List::Item* item {subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto p{const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()))};
                const auto child{dynamic_cast<IReadout*>(p->object())};
                if (child != nullptr) child->setSlotReversed(x);
                item = item->getNext();
            }
            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return true;
}

bool IReadout::setSlotVertical(const base::Boolean* const x)
{
    if (x != nullptr) {
        // Set our mode
        if (x->asBool()) {
            setDisplayMode(vertical);
        } else {
            clearDisplayMode(vertical);
        }
    }
    return true;
}

bool IReadout::setSlotBrackets(const base::Boolean* const x)
{
    if (x != nullptr) {
        // Set our mode
        if (x->asBool()) {
            setDisplayMode(brackets);
        } else {
            clearDisplayMode(brackets);
        }
    }
    return true;
}

bool IReadout::setSlotLinked(const base::Boolean* const x)
{
    if (x != nullptr) {
        setLinked(x->asBool());
    }
    return true;
}

bool IReadout::setSlotInheritColor(const base::Boolean* const x)
{
    bool ok {};
    if (x != nullptr) {
        ok = setInheritColor(x->asBool());
    }
    return ok;
}

bool IReadout::setSlotJustification(const base::Identifier* const x)
{
    bool ok {true};
    if (x != nullptr) {

        // Set our justification
        if ( *x == "none" ) {
            justification(base::Justify::None);
        } else if ( *x == "left" ) {
            justification(base::Justify::Left);
        } else if ( *x == "center" ) {
            justification(base::Justify::Center);
        } else if ( *x == "right" ) {
            justification(base::Justify::Right);
        } else {
            if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "AbstractField::setJustification: No proper inputs" << std::endl;
            }
            ok = false;
        }

        // Set our children's justification
        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {
            const base::List::Item* item{subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto p{const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()))};
                const auto child{dynamic_cast<IReadout*>(p->object())};
                if (child != nullptr) child->setSlotJustification(x);
                item = item->getNext();
            }
            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return ok;
}

bool IReadout::setSlotFont(const base::Identifier* const x)
{
    bool ok{};
    if (x == nullptr) {
        fontName.clear();
    }
    if (x != nullptr) {
        fontName = x->asString();
        ok = true;
    }

    return ok;
}

bool IReadout::setSlotStartCharPos(const base::Integer* const x)
{
    bool ok{};
    if (x != nullptr) {
        const int ii{x->asInt()};
        if (ii > 0) {
           // come in as 1 based, convert to 0 based
           startCP = static_cast<unsigned int>(ii - 1);
           ok = true;
        }
    }
    return ok;
}

}
}
