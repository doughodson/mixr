
#ifndef __mixr_instruments_Button_HPP__
#define __mixr_instruments_Button_HPP__

#include "mixr/graphics/Graphic.hpp"

namespace mixr {
namespace base { class INumber; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: Button
// Description: Generic button that knows when it is clicked
//------------------------------------------------------------------------------
class Button : public graphics::Graphic
{
    DECLARE_SUBCLASS(Button,graphics::Graphic)

public:
    Button();

    // Returns the event ID.
    virtual int getEventId() const               { return eventId; }

    // Sets the Event ID to newID.
    virtual bool setEventId(const int id)     { eventId = id; return true; };

    // Right now, this function merely detects when the button has been
    // clicked, but it can be overridden to do anything.
    virtual bool onSingleClick();

    // Cancel
    virtual bool onCancel();

    bool event(const int event, base::IObject* const obj = nullptr) override;

private:
    int eventId {-1};        // holds our event number

private:
    // slot table helper methods
    bool setSlotEventId(const base::INumber* const);
};

}
}

#endif
