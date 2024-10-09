
#include "mixr/models/player/LifeForm.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/player/weapon/Missile.hpp"

#include "mixr/models/system/StoresMgr.hpp"
#include "mixr/models/system/Gun.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/osg/Matrixd"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(LifeForm, "LifeForm")
EMPTY_SLOTTABLE(LifeForm)

LifeForm::LifeForm()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("LifeForm");
    setType_old(&generic);
    setType("LifeForm");

    setTerrainOffset(1.0);      // default offset from terrain to CG
}

void LifeForm::copyData(const LifeForm& org, const bool)
{
    BaseClass::copyData(org);
    actionState = org.actionState;
    tgtAquired = org.tgtAquired;
    if (tgtPlayer != nullptr) tgtPlayer->unref();
    tgtPlayer = nullptr;
    lockMode = org.lockMode;
    weaponSel = org.weaponSel;
}

void LifeForm::deleteData()
{
    if (tgtPlayer != nullptr) tgtPlayer->unref();
    tgtPlayer = nullptr;
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool LifeForm::shutdownNotification()
{
   if (tgtPlayer != nullptr) tgtPlayer->unref();
   tgtPlayer = nullptr;
   return BaseClass::shutdownNotification();
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int LifeForm::getMajorType() const
{
    return LIFE_FORM;
}

bool LifeForm::setActionState(const int x)
{
    actionState = x;
    return true;
}

void LifeForm::reset()
{
   // do our resetting first
   if (isLocalPlayer()) {
      actionState = UPRIGHT_STANDING;
      lookAngle = 0.0;
      tgtAquired = false;
      lockMode = SEARCHING;
      weaponSel = LF_GUN;
   }
   if (tgtPlayer != nullptr) tgtPlayer->unref();
   tgtPlayer = nullptr;

   BaseClass::reset();
}

void LifeForm::fire()
{
    const auto hdgObj = new base::Float(getHeadingR());
    const auto pitchObj = new base::Float(lookAngle * base::angle::D2RCC);
    StoresMgr* mgr{getStoresManagement()};
    if (mgr != nullptr) {
        if (getWorldModel() != nullptr) {
            if (weaponSel == LF_MISSILE) {
                mgr->setGunSelected(false);
                Missile* missile{mgr->getNextMissile()};
                if (missile != nullptr) {
                    missile->setInitPitch(pitchObj);
                    missile->setInitHeading(hdgObj);
                    missile->reset();
                    Missile* msl{mgr->releaseOneMissile()};
                    if (msl != nullptr) {
                        if (tgtAquired && tgtPlayer != nullptr) msl->setTargetPlayer(tgtPlayer, true);
                    }
                }
            } else if (weaponSel == LF_GUN) {
                mgr->setGunSelected(true);
                Gun* myGun{mgr->getGun()};
                if (myGun != nullptr) {
                    myGun->setGunArmed(true);
                    myGun->setPitch(lookAngle * base::angle::D2RCC);
                    myGun->fireControl(true);
                }
           }
        }
    }
    hdgObj->unref();
    pitchObj->unref();
}

// override our set velocity, so we can determine if we are walking, running, or standing
bool LifeForm::setVelocity(const double ue, const double ve, const double we)
{
    bool ok{BaseClass::setVelocity(ue, ve, we)};
    // based on our velocity, we will run or walk, or stand still
    const double tempX{std::fabs(ue)};
    const double tempY{std::fabs(ve)};

    // we only change our appearance bit if we are parachuting
    if (actionState != PARACHUTING) {
        // test for running and walking
        if (tempX == 0 && tempY == 0) actionState = UPRIGHT_STANDING;
        if (tempX > 0 || tempY > 0) actionState = UPRIGHT_WALKING;
        if (tempX > 8 || tempY > 8) actionState = UPRIGHT_RUNNING;
    }

    return ok;
}

void LifeForm::move(const double fwd, const double sdws)
{
    if (getDamage() < 1) {
        base::Vec3d old{getEulerAngles()};
        double hdg{old.z()};

        double tempFwd{fwd}, tempSdws{sdws};

        // our deadband (if we are barely moving, just stop)
        if (std::fabs(tempFwd) < 0.9f) tempFwd = 0;
        if (std::fabs(tempSdws) < 0.9f) tempSdws = 0;
        const double xVel{tempFwd * (std::cos(hdg))};
        const double yVel{tempFwd * (std::sin(hdg))};

        // now calculate our sideways velocity
        const double xxVel{tempSdws * (std::cos((hdg + (90 * static_cast<double>(base::angle::D2RCC)))))};
        const double yyVel{tempSdws * (std::sin((hdg + (90 * static_cast<double>(base::angle::D2RCC)))))};

        // now add the vectors
        const double newXVel{xVel + xxVel};
        const double newYVel{yVel + yyVel};

        double zVel{};
        setVelocity(static_cast<double>(newXVel), static_cast<double>(newYVel), zVel);
    } else {
        setVelocity(0, 0, 0);
    }
}

void LifeForm::look(const double up, const double sdws)
{
    if (getDamage() < 1) {
        if (lockMode != LOCKED) {
            lockMode = SEARCHING;
            // our up and sideways come in as -5 to 5, which is a rate to adjust heading
            const base::Vec3d old{getEulerAngles()};
            double hdg{old.z()};
            double ptc{lookAngle};
            double tempSdws{sdws};
            double tempUp{up};
            if (std::fabs(tempSdws) < 0.00005) tempSdws = 0;
            if (std::fabs(tempUp) < 0.05) tempUp = 0;
            hdg += tempSdws;
            hdg = base::angle::aepcdRad(hdg);
            // we don't change our pitch when we look up and down, we only change our look angle, so we have to keep
            // that separate.  WE do, however, change our heading based on where we are looking, so that is correct
            ptc += tempUp;
            if (ptc > 90) ptc = 90;
            else if (ptc < -90) ptc = -90;
            //std::cout << "HEADING = " << hdg << std::endl;
            setLookAngle(ptc);
            base::Vec3d eul(0, 0, hdg);
            setEulerAngles(eul);
            // now based on this we need to know if we have a target in our crosshairs...
            tgtAquired = false;
            if (tgtPlayer != nullptr) tgtPlayer->unref();
            tgtPlayer = nullptr;
            const base::Vec3d myPos{getPosition()};
            base::Vec3d tgtPos;
            base::Vec3d vecPos;
            double az{}, el{}, range{}, diffAz{}, diffEl{};
            const double maxAz{(0.7 * static_cast<double>(base::angle::D2RCC))};
            const double maxEl{(0.7 * static_cast<double>(base::angle::D2RCC))};
            //double maxRange = 1500.0f; // long range right now
            const double la{lookAngle * static_cast<double>(base::angle::D2RCC)};
            WorldModel* sim{getWorldModel()};
            if (sim != nullptr) {
                base::PairStream* players{sim->getPlayers()};
                if (players != nullptr) {
                    base::List::Item* item{players->getFirstItem()};
                    while (item != nullptr && !tgtAquired) {
                        const auto pair = static_cast<base::Pair*>(item->getValue());
                        if (pair != nullptr) {
                            const auto player = dynamic_cast<Player*>(pair->object());
                            if (player != nullptr && player != this && !player->isMajorType(WEAPON) && !player->isDestroyed()) {
                                // ok, calculate our position from this guy
                                tgtPos = player->getPosition();
                                vecPos = tgtPos - myPos;
                                az = std::atan2(vecPos.y(), vecPos.x());
                                range = (vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y());
                                range = std::sqrt(range);
                                // now get our elevation
                                el = std::atan2(-vecPos.z(), range);
                                diffAz = std::fabs(base::angle::aepcdRad(az - static_cast<double>(getHeadingR())));
                                diffEl = std::fabs(base::angle::aepcdRad(la - el));
                                if ((diffAz <= maxAz) && (diffEl <= maxEl)) {
                                    lockMode = TGT_IN_SIGHT;
                                    tgtAquired = true;
                                    if (tgtPlayer != player) {
                                        if (tgtPlayer != nullptr) tgtPlayer->unref();
                                        tgtPlayer = player;
                                        tgtPlayer->ref();
                                    }
                                }
                            }
                        }
                        item = item->getNext();
                    }
                    players->unref();
                    players = nullptr;
                }
            }
        }
        // else we are locking on target, and need to follow our target player
        else {
            if (tgtPlayer == nullptr) lockMode = SEARCHING;
            else {
                const base::Vec3d vecPos{tgtPlayer->getPosition() - getPosition()};
                const double az{std::atan2(vecPos.y(), vecPos.x())};
                double range{(vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y())};
                range = std::sqrt(range);
                // now get our elevation
                const double el{std::atan2(-vecPos.z(), range)};
                // now force that on us
                setLookAngle(el * static_cast<double>(base::angle::R2DCC));
                setEulerAngles(0, 0, az);
            }
        }
    }
}

/*
//==============================================================================
// class InfantryMan - default grunt
//==============================================================================
IMPLEMENT_SUBCLASS(InfantryMan, "InfantryMan")
EMPTY_SLOTTABLE(InfantryMan)
EMPTY_COPYDATA(InfantryMan)
EMPTY_DELETEDATA(InfantryMan)

InfantryMan::InfantryMan()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("InfantryMan");
    setType(&generic);
}

//==============================================================================
// class Parachutist - grunt who jumps out of planes!
//==============================================================================
IMPLEMENT_SUBCLASS(Parachutist, "Parachutist")
EMPTY_SLOTTABLE(Parachutist)
EMPTY_COPYDATA(Parachutist)
EMPTY_DELETEDATA(Parachutist)

Parachutist::Parachutist()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("Parachutist");
    setType(&generic);
}

*/
}
}
