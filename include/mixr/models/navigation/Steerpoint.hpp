
#ifndef __mixr_models_common_Steerpoint_HPP__
#define __mixr_models_common_Steerpoint_HPP__

#include "mixr/base/IComponent.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace base { class IAngle; class Identifier; class Integer; class ILength; class Latitude;
                 class List; class Longitude; class Identifier; class INumber; class String; class ITime; }
namespace terrain { class ITerrain; }
namespace models {
class INavigation;
class IAction;

//------------------------------------------------------------------------------
// Class: Steerpoint
//
// Description: Generic steerpoint; contains positional and navigational data;
//              component list contains associated steerpoints (FIX, OAP, etc.)
//
// Factory name: Steerpoint
// Slots:
//   stptType  <base::Identifier>     ! Steerpoint type; default: DEST
//                                    ! { ROUTE, DEST, MARK, FIX, OAP, TGT }
//
//   latitude  <base::Latitude>       ! Steerpoint latitude  (default: 0)
//             <base::INumber>        ! (or as degrees decimal)
//
//   longitude <base::Longitude>      ! Steerpoint longitude (default: 0)
//             <base::INumber>        ! (or as degrees decimal)
//
//   xPos      <base::ILength>        ! X (north) distance from gaming area reference point
//   yPos      <base::ILength>        ! Y (east) distance from gaming area reference point
//                                    ! (default: 0, 0)
//   elevation <base::ILength>        ! Terrain elevation at steerpoint (default: 0)
//             <base::INumber>        ! (or as meters)
//
//   altitude  <base::ILength>        ! Commanded altitude to steerpoint (default: 0)
//             <base::INumber>        ! (or as meters)
//
//   airspeed  <base::INumber>        ! Commanded true airspeed (Kts) (default: 0)
//
//   pta       <base::ILength>        ! Planned Time of Arrival (PTA)  (default: 0)
//             <base::INumber>        ! (or as seconds)
//
//   sca       <base::ILength>        ! Safe Clearance Altitude (SCA) (default: 0)
//             <base::INumber>        ! (or as feet)
//
//   description <base::String>       ! Description of steerpoint (default: 0)
//
//   magvar    <base::Angle>          ! Magnetic Var at steerpoint (default: 0)
//             <base::INumber>        ! (or as degrees)
//
//   next      <base::Integer>        ! "Next" steerpoint number (default: 0)
//             <base::Identifier>     ! or by steerpoint name
//
//   action    <IAction>              ! Action to be triggered at steerpoint crossing (auto only)
//                                    !  Note: the "to" steerpoint will have sequenced to the
//                                    !  next steerpoint when action is triggered. (default: 0)
//
//------------------------------------------------------------------------------
class Steerpoint : public base::IComponent
{
   DECLARE_SUBCLASS(Steerpoint, base::IComponent)

public:
   enum class StptType { DEST, MARK, FIX, OAP, IP, TGT, TGT_GRP };

public:
    Steerpoint();

    StptType getSteerpointType() const          { return stptType; }

    // Valid data?
    bool isPosVecValid() const                  { return !needPosVec; }
    bool isLatLonValid() const                  { return !needLL; }
    bool isNavDataValid() const                 { return navDataValid; }
    bool isWarnSCA() const                      { return scaWarn; }
    bool isCmdAltValid() const                  { return haveCmdAlt; }
    bool isCmdAirspeedValid() const             { return haveCmdAs; }

    // Steerpoint parameters
    const base::Vec3d& getPosition() const      { return posVec; }
    double getLatitude() const;
    double getLongitude() const;
    double getElevation() const                 { return elevation; }
    double getElevationM() const                { return elevation; }
    double getElevationFt() const;
    double getSCA() const                       { return sca; }     // Feet
    double getPTA() const                       { return pta; }     // Seconds
    const char* getDescription() const;
    double getMagVarDeg() const                 { return magvar; }
    double getCmdAltitude() const               { return cmdAlt; }
    double getCmdAltitudeM() const              { return cmdAlt; }
    double getCmdAltitudeFt() const;
    double getCmdAirspeedKts() const            { return cmdAirspeed; }

    // Nav Steering: 'direct-to' data
    double getTrueBrgDeg() const                { return tbrg; }
    double getMagBrgDeg() const                 { return mbrg; }
    double getDistNM() const                    { return dst; }
    double getTTG() const                       { return ttg; }
    double getCrossTrackErrNM() const           { return xte; }

    // Nav Steering: 'leg' data
    double getTrueCrsDeg() const                { return tcrs; }
    double getMagCrsDeg() const                 { return mcrs; }
    double getLegDistNM() const                 { return tld; }
    double getLegTime() const                   { return tlt; }

    // Nav Steering: 'enroute' data
    double getDistEnrouteNM() const             { return tde; }
    double getETE() const                       { return ete; }
    double getETA() const                       { return eta; }
    double getELT() const                       { return elt; }

    // Set the ground elevation at the steerpoint from this terrain database
    // Interpolate between elevation posts if the optional 'interp' flag is true.
    // Returns true if successful.
    virtual bool setElevation(const terrain::ITerrain* const terrain, const bool interp = false);

    // Set parameters
    virtual void setSteerpointType(const StptType t)    { stptType = t; }
    virtual void setPosition(const double x, const double y, const double z);
    virtual void setElevation(const double);
    virtual void setPosition(const base::Vec3d&);
    virtual void setLatitude(const double);
    virtual void setLongitude(const double);
    virtual void setPTA(const double v)                 { pta = v; }
    virtual void setSCA(const double v)                 { sca = v; }
    virtual void setDescription(const base::String* const);
    virtual void setCmdAltitude(const double);
    virtual void setCmdAirspeedKts(const double);

    // Set nav data
    virtual void setTrueBrgDeg(const double v)           { tbrg = v; }
    virtual void setMagBrgDeg(const double v)            { mbrg = v; }
    virtual void setDistNM(const double v)               { dst = v; }
    virtual void setTTG(const double v)                  { ttg = v; }
    virtual void setCrossTrackErrNM(const double v)      { xte = v; }
    virtual void setTrueCrsDeg(const double v)           { tcrs = v; }
    virtual void setMagCrsDeg(const double v)            { mcrs = v; }
    virtual void setLegDistNM(const double v)            { tld = v; }
    virtual void setLegTime(const double v)              { tlt = v; }
    virtual void setDistEnrouteNM(const double v)        { tde = v; }
    virtual void setETE(const double v)                  { ete = v; }
    virtual void setETA(const double v)                  { eta = v; }
    virtual void setELT(const double v)                  { elt = v; }

    // Sets the initial lat/lon (reset()) values
    virtual void setInitLatitude(const double lat)    { initLatitude = lat; }
    virtual void setInitLongitude(const double lon)   { initLongitude = lon; }

    // Get/set steerpoint action
    IAction* getAction() { return action; }              // Action to be performed
    const IAction* getAction() const { return action; }  // Action to be performed
    virtual bool setAction(IAction* const act);          // Sets the action to be performed

   // Compute have data 'to' this point
   virtual bool compute(const INavigation* const nav, const Steerpoint* const from = nullptr);
   virtual void clearNavData();

   void reset() override;

protected:
    virtual void processComponents(                 // Process our subcomponent list (which should be other steerpoints)
         base::PairStream* const list,              // Source list of components
         const std::type_info& filter,              // Type filter
         base::Pair* const add = nullptr,           // Optional pair to add
         base::IComponent* const remove = nullptr   // Optional subcomponent to remove
       ) override;

private:
    // Steerpoint parameters
    double      latitude{};                 // latitude
    double      longitude{};                // Longitude
    double      elevation{};                // Elevation                (m)
    base::Vec3d posVec;                     // Position vector          (m)           [ x, y, z ] NED
    StptType    stptType{StptType::DEST};   // Steerpoint type
    double      pta{};                      // Planned Time of Arrival  (sec)
    double      sca{};                      // Safe Clearance Alt       (ft)
    double      magvar{};                   // Mag Var                  (degs)
    base::safe_ptr<const base::String> description; // Description
    bool        needPosVec{true};           // Request calucaltion of 'posVec' from Lat/Lon
    bool        needLL{true};               // Request calucaltion of Lat/Lon from 'posVec'
    double      cmdAlt{};                   // Commanded Altitude       (m)
    bool        haveCmdAlt{};               // Have commanded altitude
    double      cmdAirspeed{};              // Commanded Airspeed       (kts)
    bool        haveCmdAs{};                // Have commanded airspeed
    base::safe_ptr<base::Pair> next;        // "next" Steerpoint pair [ name steerpoint ]

    // Steerpoint action
    base::safe_ptr<IAction> action;   // Action to be performed at this steerpoint

    // Initial (reset) data
    double      initLatitude{};       // latitude
    double      initLongitude{};      // Longitude
    base::Vec3d  initPosVec;          // Init Position vector (m)           [ x, y, z ] NED
    double      initMagVar{};         // Mag Var entered for this point  (deg)
    bool        haveInitLat{};        // Have initial latitude
    bool        haveInitLon{};        // Have initial longitude
    bool        haveInitPos{};        // Have initial pos vector
    bool        haveInitMagVar{};     // Have initial mag var
    double      initElev{};           // Elevation                  (m)
    bool        haveInitElev{};       // Have initial elevation
    double      initCmdAlt{};         // Initial commanded altitude (m)
    bool        haveInitCmdAlt{};     // Have initial cmd altitude
    double      initCmdAirspeed{};    // Initial commanded airspeed (kts)
    bool        haveInitCmdAs{};      // Have commanded airspeed
    base::safe_ptr<const base::Identifier> initNextStptName; // Name of the inital "next" steerpoint
    int         initNextStptIdx{};    // Index of the initial "next" steerpoint

    // Computed data
    double tbrg {};           // True bearing direct-to point (deg)
    double mbrg {};           // Mag bearing direct-to point  (deg)
    double dst {};            // Distance direct-to point (nm)
    double ttg {};            // Time-To-Go (direct)      (sec)
    double xte {};            // Cross-Track Error        (nm)

    double tcrs {};           // TRUE Course to point     (degs)
    double mcrs {};           // Mag Course to point      (degs)
    double tlt {};            // Total Time this Leg      (sec)
    double tld {};            // Total Leg Distance       (nm)

    double tde {};            // Total Distance Enroute   (nm)
    double ete {};            // Est Time Enroute         (sec)
    double eta {};            // Est Time of Arrival (UTC)(sec)
    double elt {};            // Early/Late time          (sec)
    bool scaWarn {};          // Safe clearance Alt warning flag
    bool navDataValid {};     // Nav data is valid

private:
   // slot table helper methods
   bool setSlotSteerpointType(const base::Identifier* const);
   bool setSlotLatitude(const base::Latitude* const);
   bool setSlotLatitude(const base::INumber* const);
   bool setSlotLongitude(const base::Longitude* const);
   bool setSlotLongitude(const base::INumber* const);
   bool setSlotXPos(const base::ILength* const);
   bool setSlotYPos(const base::ILength* const);
   bool setSlotElevation(const base::ILength* const);
   bool setSlotElevation(const base::INumber* const);
   bool setSlotCmdAltitude(const base::ILength* const);
   bool setSlotCmdAltitude(const base::INumber* const);
   bool setSlotCmdAirspeed(const base::INumber* const);
   bool setSlotPTA(const base::ITime* const);
   bool setSlotPTA(const base::INumber* const);
   bool setSlotSCA(const base::ILength* const);
   bool setSlotSCA(const base::INumber* const);
   bool setSlotDescription(const base::String* const);
   bool setSlotMagVar(const base::IAngle* const);
   bool setSlotMagVar(const base::INumber* const);
   bool setSlotNext(const base::Identifier* const);
   bool setSlotNext(const base::Integer* const);
   bool setSlotAction(IAction* const x)                   { return setAction(x); }

   //bool setSlotPosition(const base::List* const);
};

}
}

#endif
