
#include "mixr/graphics/readouts/AbstractField.hpp"

#include "mixr/graphics/Display.hpp"
#include "mixr/graphics/Page.hpp"
#include "mixr/graphics/Display.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/colors/Rgb.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(AbstractField, "AbstractField")

BEGIN_SLOTTABLE(AbstractField)
    "position",         // 1) Starting Position ( ln cp )
    "width",            // 2) Field width
    "highLight",        // 3) Highlight text flag
    "underline",        // 4) Underlined text flag
    "reversed",         // 5) Reversed text flag
    "justification",    // 6) "none", "left", "center", or "right"
    "vertical",         // 7) vertical field flag (text displayed vertically)
    "brackets",         // 8) Bracket flag
    "linked",           // 9) Linked flag (auto step to next input field)
    "inheritColor",     //10) Inherit color of our container (instead of using default color)
    "font",             //11) Type of font to use before drawing
    "startCharPos",     //12) Our starting character position (we may want to skip!)
END_SLOTTABLE(AbstractField)

BEGIN_SLOT_MAP(AbstractField)
    ON_SLOT(1, setSlotPosition,      base::List)
    ON_SLOT(2, setSlotWidth,         base::Number)
    ON_SLOT(3, setSlotHighlight,     base::Number)
    ON_SLOT(4, setSlotUnderline,     base::Number)
    ON_SLOT(5, setSlotReversed,      base::Number)
    ON_SLOT(6, setSlotJustification, base::String)
    ON_SLOT(7, setSlotVertical,      base::Number)
    ON_SLOT(8, setSlotBrackets,      base::Number)
    ON_SLOT(9, setSlotLinked,        base::Number)
    ON_SLOT(10, setSlotInheritColor, base::Number)
    ON_SLOT(11, setSlotFont,         base::String)
    ON_SLOT(12, setSlotStartCharPos, base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(AbstractField)
    if (mode == Mode::input) {
        bool kb {( _event >= 0x20 && _event <= 0x7f )};
        ON_EVENT(FORWARD_SPACE,onForwardSpace)
        ON_EVENT(BACK_SPACE,onBackSpace)
        // Keyboard Entry
        if (kb) {
            // Filter the input event -- that is, let a virtual member
            // function filter the input event using the current template
            // character.
            char nc {filterInputEvent(_event, inputExample.getChar(icp))};
            if (nc != '\0') {
                setChar(nc);
                _used = true;
            }
        }
    }
    ON_EVENT_OBJ(SET_POSITION,setPosition,base::List)
    ON_EVENT_OBJ(SET_LINE,onSetLine,base::Number)
    ON_EVENT_OBJ(SET_COLUMN,onSetColumn,base::Number)
    ON_EVENT_OBJ(SET_WIDTH,setSlotWidth,base::Number)
    ON_EVENT_OBJ(SET_HIGHLIGHT,setSlotHighlight,base::Number)
    ON_EVENT_OBJ(SET_UNDERLINE,setSlotUnderline,base::Number)
    ON_EVENT_OBJ(SET_REVERSED,setSlotReversed,base::Number)
    ON_EVENT_OBJ(SET_JUSTIFICATION,setSlotJustification,base::String)
END_EVENT_HANDLER()

AbstractField::AbstractField()
{
    STANDARD_CONSTRUCTOR()
    jmode = base::String::Justify::NONE;
}

void AbstractField::copyData(const AbstractField& org, const bool)
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
    setSlotFont(org.fontName);
}

void AbstractField::deleteData()
{
    origStr.empty();
    inputExample.empty();
    ln = 0;
    cp = 0;
    w  = 0;
    str.empty();
    dmode = 0;
    jmode = base::String::Justify::NONE;
    mode  = Mode::display;
    icp   = 0;
    inpDspMode = 0;
    inpModeHold = false;
    setSlotFont(nullptr);
}

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void AbstractField::updateData(const double dt)
{
    BaseClass::updateData(dt);

    // ---
    // Set display mode when this field is in 'input' mode
    // ---
    if (inpDspMode != 0) {
        // Auto switch ON input display mode?
        if ( (mode == Mode::input) &&  (icp > 0 || !inpModeHold) ) {
            setDisplayMode( inpDspMode );
        }

        // Auto swtich OFF input display mode?
        if ( (mode == Mode::display) || (inpModeHold && icp == 0) ) {
            // If it wasn't previously set by a setSlot() function then turn it off.
            if ( !isDisplayMode(0x1000 & inpDspMode) ) {
                clearDisplayMode( inpDspMode );
            }
        }
    }

    // ---
    // Update readout during input mode?
    // ---
    if ( (mode == Mode::input) && inpModeHold && icp == 0 ) {
        adjust();
    }
}

//------------------------------------------------------------------------------
// line() -- set the line number
// column() -- set the column number
//------------------------------------------------------------------------------
int AbstractField::line(const int ll)
{
    return (ln = ll);
}

int AbstractField::line() const
{
    return ln;
}

int AbstractField::column(const int cc)
{
    return (cp = cc);
}

int AbstractField::column() const
{
    return cp;
}

//------------------------------------------------------------------------------
// withinField() -- Return True/False if cp,ln is in the field space
//------------------------------------------------------------------------------
bool AbstractField::withinField(const int l, const int c) const
{
    bool stat {};
    if ( (l == ln) && (c >= cp) && (c <= (cp + static_cast<int>(w) - 1)) ) stat = true;
    return stat;
}

//------------------------------------------------------------------------------
// setText() -- set the field's text string
//------------------------------------------------------------------------------
void AbstractField::setText(const char s[])
{
    origStr = s;
    if (mode == Mode::display) {
        adjust();
    }
}

//------------------------------------------------------------------------------
// justification() --
//------------------------------------------------------------------------------
base::String::Justify AbstractField::justification() const
{
    return jmode;
}

base::String::Justify AbstractField::justification(const base::String::Justify t)
{
    jmode = t;
    if (mode == Mode::display) adjust();
    return jmode;
}

//------------------------------------------------------------------------------
// setMode() -- set the mode of the field (display, input)
//------------------------------------------------------------------------------
AbstractField::Mode AbstractField::setMode(const AbstractField::Mode nmode)
{
    Mode omode = mode;
    mode = nmode;

    if (nmode == Mode::input && omode == Mode::display) {

        // When we're entering the INPUT mode ...

        // Change the input focus to this field
        getDisplay()->focus(this);

        // Set the input chararcter pointer to the first character
        if (startCP > 0 && startCP < w) icp = startCP;
        else icp = 0;

    }
    else if (nmode == Mode::display && omode == Mode::input) {

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
bool AbstractField::isValidInputPosition(const int tc)
{
    return (tc == '+' || tc == '0' || tc == '#' ||
            tc == 'D' || tc == 'H' || tc == 'M' || tc == 'S');
}

// filterInputEvent() -- Filter input events using a template character (tc)
char AbstractField::filterInputEvent(const int event, const int tc)
{
    if (tc == '+') {
        // Default sign keys
        if (event == '7')
            return '-';
        else if (event == '9')
            return '+';
        else
            return '\0';
    }

    else if (tc == '0' || tc == '#') {
        // Default numeric keys
        if (event >= '0' && event <= '9')
            return char(event);
        else
            return '\0';
    }

    else
        return '0';

}

int AbstractField::setExample(const char* const example)
{
    inputExample = example;
    return static_cast<int>(inputExample.len());
}

void AbstractField::advanceSpace(const int ns)
{
    if (mode != Mode::input) return;
    icp += ns;
    while ( icp < static_cast<int>(w) && !isValidInputPosition(inputExample.getChar(icp)) ) icp++;
    if (icp >= static_cast<int>(w)) {
        icp = static_cast<int>(w)-1;
        while ( !isValidInputPosition(inputExample.getChar(icp)) ) icp--;
        event(INPUT_RIGHT_EDGE);
    }
}

void AbstractField::backSpace(const int ns)
{
   if (mode != Mode::input) return;
   // if we are backspacing, and we are at the starting character position that was set, we stay there!
   if (startCP > 0 && icp == static_cast<int>(startCP)) {
      event(INPUT_LEFT_EDGE);
   }
   else {
      icp -= ns;
      while ( icp >= 0 && !isValidInputPosition(inputExample.getChar(icp)) ) icp--;
      if (icp < 0) {
         icp = 0;
         while ( !isValidInputPosition(inputExample.getChar(icp)) ) icp++;
         event(INPUT_LEFT_EDGE);
      }
   }
}

bool AbstractField::setInputCharacterPosition(const unsigned int ii)
{
    if (startCP > 0) {
        if (ii >= startCP) icp = ii;
    }
    else if (ii < w) icp = ii;
    return true;
}


char AbstractField::getChar()
{
    if (mode == Mode::input)
        return str.getChar(icp);
    else
        return '\0';
}

void AbstractField::setChar(const char c)
{
    if (mode != Mode::input) return;
    str.setChar(icp,c);
    advanceSpace();
}

double AbstractField::getInputValue() const
{
    return 0.0;
}

bool AbstractField::isInputValueValid() const
{
    return true;
}


//------------------------------------------------------------------------------
// onForwardSpace() --
//------------------------------------------------------------------------------
bool AbstractField::onForwardSpace()
{
    advanceSpace();
    return true;
}

//------------------------------------------------------------------------------
// onBackSpace() --
//------------------------------------------------------------------------------
bool AbstractField::onBackSpace()
{
    backSpace();
    return true;
}

//------------------------------------------------------------------------------
// cursor() -- Returns true if text cursor should be seen within this
//             object and the position of the cursor.
//------------------------------------------------------------------------------
bool AbstractField::cursor(int* l, int* c) const
{
    if (mode == Mode::input) {
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
void AbstractField::drawFunc()
{
    // Get a pointer to the current display
    graphics::Display* dsp {getDisplay()};
    if (dsp == nullptr) return;

    // ---
    // When our container is also a Field, get a pointer to it.
    // ---
    graphics::AbstractField* parent {};
    if (container() != nullptr) {
        const auto fp = dynamic_cast<graphics::AbstractField*>(container());
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
    if (fontName != nullptr) dsp->selectFont(isReversed(), isUnderlined(), dsp->getFont(fontName->getString()));
    else dsp->selectFont(isReversed(), isUnderlined());


    // ---
    // Set the color
    // ---
    bool restoreColor {};
    base::Vec4d ocolor {dsp->getCurrentColor()};
    // only use default colors if we aren't inheriting our container's colors

    if (!isInheritColor()) {
        if (getColorName() == nullptr && getColor() == nullptr) {
            const base::Color* cc = nullptr;
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
        const char* sp {str};
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
        }
        else {
            // Position for normal text
            dsp->drawLeftBracket(ll, cc-1);
            dsp->drawRightBracket(ll, cc + static_cast<int>(width()));
        }
    }

    // ---
    // If we used default colors, restore the old value
    // ---
    if (restoreColor) dsp->setColor(ocolor);

}

//------------------------------------------------------------------------------
// setPosition() -- set position: [ Line Column ]
//------------------------------------------------------------------------------
bool AbstractField::setPosition(const base::List* const spobj)
{
    bool ok {true};
    if (spobj != nullptr) {
        int values[2] {};
        int n {static_cast<int>(spobj->getNumberList(values, 2))};
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

bool AbstractField::onSetLine(const base::Number* const oslobj)
{
    if (oslobj != nullptr) line(oslobj->getInt());
    return true;
}

bool AbstractField::onSetColumn(const base::Number* const oscobj)
{
   if (oscobj != nullptr) column(oscobj->getInt());
   return true;
}

bool AbstractField::setSlotPosition(const base::List* const spobj)
{
   return setPosition(spobj);
}

bool AbstractField::setSlotWidth(const base::Number* const swobj)
{

    if (swobj != nullptr) width(swobj->getInt());
    return true;
}

bool AbstractField::setSlotHighlight(const base::Number* const shobj)
{
    if (shobj != nullptr) {
        // Set our mode
        if (shobj->getBoolean()) {
            setDisplayMode(highlight);
            setDisplayMode(highlight1);
        } else {
            clearDisplayMode(highlight);
            clearDisplayMode(highlight1);
        }

        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {
            const base::List::Item* item {subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
                const auto child = dynamic_cast<AbstractField*>(p->object());
                if (child != nullptr) child->setSlotHighlight(shobj); //changed from obj
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return true;
}

bool AbstractField::setSlotUnderline(const base::Number* const suobj)
{
    if (suobj != nullptr) {

        // Set our mode
        if (suobj->getBoolean()) {
            setDisplayMode(underline);
            setDisplayMode(underline1);
        }
        else {
            clearDisplayMode(underline);
            clearDisplayMode(underline1);
        }

        // Set our children's mode
        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {

            const base::List::Item* item {subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
                const auto child = dynamic_cast<AbstractField*>(p->object());
                if (child != nullptr) child->setSlotUnderline(suobj);
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return true;
}

bool AbstractField::setSlotReversed(const base::Number* const srobj)
{
    if (srobj != nullptr) {

        // Set our mode
        if (srobj->getBoolean()) {
            setDisplayMode(reversed);
            setDisplayMode(reversed1);
        }
        else {
            clearDisplayMode(reversed);
            clearDisplayMode(reversed1);
        }

        // Set our children's mode
        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {

            const base::List::Item* item {subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
                const auto child = dynamic_cast<AbstractField*>(p->object());
                if (child != nullptr) child->setSlotReversed(srobj);
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return true;
}

bool AbstractField::setSlotVertical(const base::Number* const ssobj)
{
    if (ssobj != nullptr) {
        // Set our mode
        if (ssobj->getBoolean()) {
            setDisplayMode(vertical);
            setDisplayMode(vertical1);
        }
        else {
            clearDisplayMode(vertical);
            clearDisplayMode(vertical1);
        }
    }
    return true;
}

bool AbstractField::setSlotBrackets(const base::Number* const ssobj)
{
    if (ssobj != nullptr) {
        // Set our mode
        if (ssobj->getBoolean()) {
            setDisplayMode(brackets);
            setDisplayMode(brackets1);
        }
        else {
            clearDisplayMode(brackets);
            clearDisplayMode(brackets1);
        }
    }
    return true;
}

bool AbstractField::setSlotLinked(const base::Number* const msg)
{
    if (msg != nullptr) {
        setLinked( msg->getBoolean() );
    }
    return true;
}

bool AbstractField::setSlotInheritColor(const base::Number* const ic)
{
    bool ok {};
    if (ic != nullptr) {
        ok = setInheritColor(ic->getBoolean());
    }
    return ok;
}

bool AbstractField::setSlotJustification(const base::String* const sjobj)
{
    bool ok {true};
    if (sjobj != nullptr) {

        // Set our justification
        if ( *sjobj == "none" )
            justification(base::String::Justify::NONE);
        else if ( *sjobj == "left" )
            justification(base::String::Justify::LEFT);
        else if ( *sjobj == "center" )
            justification(base::String::Justify::CENTER);
        else if ( *sjobj == "right" )
            justification(base::String::Justify::RIGHT);
        else {
              if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "AbstractField::setJustification: No proper inputs" << std::endl;
              }
            ok = false;
        }

        // Set our children's justification
        base::PairStream* subcomponents {getComponents()};
        if (subcomponents != nullptr) {

            const base::List::Item* item = subcomponents->getFirstItem();
            while (item != nullptr) {
                const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
                const auto child = dynamic_cast<AbstractField*>(p->object());
                if (child != nullptr) child->setSlotJustification(sjobj);
                item = item->getNext();
            }

            subcomponents->unref();
            subcomponents = nullptr;
        }
    }
    return ok;
}

bool AbstractField::setSlotFont(const base::String* const font)
{
    bool ok {};
    if (fontName != nullptr) fontName->unref();
    fontName = nullptr;
    if (font != nullptr) {
        fontName = font->clone();
        ok = true;
    }

    return ok;
}

bool AbstractField::setSlotStartCharPos(const base::Number* const msg)
{
    bool ok {};
    if (msg != nullptr) {
        int ii {msg->getInt()};
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
