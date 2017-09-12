
#include "mixr/ig/viewpoint/OtwPC.hpp"

#include "mixr/ig/common/OtwModel.hpp"

#include "mixr/ig/viewpoint/EntityState.hpp"

#include "mixr/models/player/AirVehicle.hpp"
#include "mixr/models/player/GroundVehicle.hpp"
#include "mixr/models/player/Missile.hpp"
#include "mixr/models/player/Player.hpp"

#include "mixr/base/network/NetHandler.hpp"
#include "mixr/base/SlotTable.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace viewpoint {

IMPLEMENT_SUBCLASS(OtwPC, "OtwPC")

BEGIN_SLOTTABLE(OtwPC)
    "netOutput",        // 1) Network output handler
END_SLOTTABLE(OtwPC)

BEGIN_SLOT_MAP(OtwPC)
    ON_SLOT(1, setSlotNetOutput, base::NetHandler)
END_SLOT_MAP()

OtwPC::OtwPC()
{
    STANDARD_CONSTRUCTOR()
    setMaxRange(0);
    setMaxModels(0);
    setMaxElevations(0);
}

void OtwPC::copyData(const OtwPC& org, const bool cc)
{
    BaseClass::copyData(org);

    if (cc) {
        setMaxRange(0);
        setMaxModels(0);
        setMaxElevations(0);
    }
    netOutput = nullptr;
    netInit = org.netInit;
    netInitFail = org.netInitFail;
    scnt = 0;
}

void OtwPC::deleteData()
{
    netOutput = nullptr;
    netInit = false;
    netInitFail = false;
}

//------------------------------------------------------------------------------
// modelFactory() -- Create OtwModel objects unique to interface
//------------------------------------------------------------------------------
ig::OtwModel* OtwPC::modelFactory()
{
    return new ig::OtwModel();
}

//------------------------------------------------------------------------------
// hotFactory() -- Create OtwHot objects unique to interface
//------------------------------------------------------------------------------
ig::OtwModel* OtwPC::hotFactory()
{
    return new ig::OtwModel();
}

//------------------------------------------------------------------------------
// sendOwnshipAndModels() - Send state data for ownship and models
//------------------------------------------------------------------------------
void OtwPC::sendOwnshipAndModels()
{
    scnt--;
    if (scnt <= 0) {
        sendPcData();
        scnt = 5;   // send every 5th frame
    }
}

//------------------------------------------------------------------------------
// sends terrain height requests
//------------------------------------------------------------------------------
void OtwPC::sendElevationRequests()
{
}

//------------------------------------------------------------------------------
// receives terrain height data
//------------------------------------------------------------------------------
void OtwPC::recvElevations()
{
}

//------------------------------------------------------------------------------
// triggers the frame update
//------------------------------------------------------------------------------
void OtwPC::frameSync()
{
    // initialization
    if (!isNetworkInitialized() && !didInitializationFail()) {
        if (initNetwork()) {
            netInit = true;
            netInitFail = false;
        }
        else {
            netInit = false;
            netInitFail = true;
        }
    }
}

void OtwPC::reset()
{
    BaseClass::reset();
}

bool OtwPC::initNetwork()
{
    bool ok {true};

    // initialize network output handler
    if (netOutput != nullptr) {
        if (netOutput->initNetwork(true)) {
            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "netOutput Initialize OK" << std::endl;
            }
        }
    }
    else {
        if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "OtwPC::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
        }
        ok = false;
    }

    return ok;
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

bool OtwPC::setSlotNetOutput(base::NetHandler* const msg)
{
    netOutput = msg;
    return true;
}

void OtwPC::sendPcData()
{
    //const double DEG2MR = (PI / 180.0f * 1000.0f);

    // Ownship type air vehicle?
    const auto av = dynamic_cast<const models::AirVehicle*>(getOwnship());
    if (av != nullptr) {

        EntityState entityState;

        entityState.x_cg = 0;
        entityState.y_cg = 0;
        entityState.z_cg = 0;          // altitude

        const base::Vec3d pos = av->getPosition();
        entityState.pilot_eye_x = pos[0] * base::distance::M2FT;
        entityState.pilot_eye_y = pos[1] * base::distance::M2FT;
        entityState.pilot_eye_z = -pos[2] * base::distance::M2FT;   // altitude

        entityState.alphad = av->getAngleOfAttackD();
        entityState.betad = av->getSideSlipD();
        entityState.mach = av->getMach();
        entityState.runalt = 0.0;

        entityState.theta    = static_cast<float>(av->getPitchD());
        entityState.phi      = static_cast<float>(av->getRollD());
        entityState.psi      = static_cast<float>(av->getHeadingD());
        entityState.airspeed = static_cast<float>(av->getTotalVelocityKts());

        entityState.heading = static_cast<float>(av->getHeadingD());

        entityState.dlg = 0;           // landing gear position 90 is down (scaled to 0-1)
        entityState.dsb = static_cast<float>(av->getSpeedBrakePosition()/100.0f);   // speed break 60 is out (scaled to 0-1)
        entityState.nz  = static_cast<float>(av->getGload());

        entityState.aetrc = 0;         // Commanded throttle position
        entityState.afterburner = 0;   // logical, true in in A/B

        entityState.target_id = 0;

        entityState.id_self = 0;       // make use of a hole
        entityState.flags = 0;

        entityState.target_x = 0;
        entityState.target_y = 0;
        entityState.target_z = 0;

        entityState.target_theta = 0;
        entityState.target_phi = 0;
        entityState.target_psi = 0;

        entityState.target_uearth = 0;
        entityState.target_vearth = 0;
        entityState.target_wearth = 0;
        entityState.target_vcas = 0;

        if (netOutput != nullptr) {
           netOutput->sendData( reinterpret_cast<char*>(&entityState), sizeof(entityState) );
        }
    }
}

}
}

