
#ifndef __mixr_models_common_ITrackMgr_HPP__
#define __mixr_models_common_ITrackMgr_HPP__

#include "mixr/models/system/ISystem.hpp"
#include "mixr/base/safe_queue.hpp"

namespace mixr {
namespace base { class Boolean; class Integer; class INumber; class ITime; }
namespace models {
class Emission;
class Player;
class Track;

//------------------------------------------------------------------------------
// Class: ITrackMgr
//
// Description: Interface class for all track managers
//
// Factory name: ITrackMgr
// Slots:
//    maxTracks       <Integer>  ! Maximum number of tracks (default: MAX_TRKS)
//
//    maxTrackAge     <Time>     ! Maximum track age (default: 3) ### NES: the comment in the src says 2 sec
//    maxTrackAge     <INumber>  ! Maximum track age (seconds)
//
//    firstTrackId    <Integer>  ! First track ID (default: 1000)
//
//    alpha           <INumber>  ! Alpha (default: 1.0)
//    beta            <INumber>  ! Beta  (default: 0.0)
//    gamma           <INumber>  ! Gamma (default: 0.0)
//
//    logTrackUpdates <Boolean>  ! True to log all updates to tracks (default: true)
//
//------------------------------------------------------------------------------
class ITrackMgr : public ISystem
{
   DECLARE_SUBCLASS(ITrackMgr, ISystem)

public:
   ITrackMgr();

   virtual double getMaxTrackAge() const;
   virtual bool setMaxTrackAge(const double sec);

   virtual unsigned int getMaxTracks() const;
   virtual unsigned int getNumTracks() const;

   virtual int getTrackList(base::safe_ptr<Track>* const slist, const unsigned int max) const;
   virtual int getTrackList(base::safe_ptr<const Track>* const slist, const unsigned int max) const;

   // Note: Tracks have been ref() before being returned and need to
   // be unref() by the user.
   virtual int getTrackList(Track* tlist[], const unsigned int max);
   virtual int getTrackList(const Track* tlist[], const unsigned int max) const;

   // Type of tracks managed (see enum TypeBits in Track.hpp)
   virtual bool isType(const short t) const;
   virtual short getType() const;
   virtual void setType(const short t);
   virtual void setSubtype(const short t);

   virtual bool getLogTrackUpdates() const;
   virtual bool setLogTrackUpdates(const bool b);

   // Add a track
   virtual bool addTrack(Track* const t);

   // Clear all tracks and queues
   virtual void clearTracksAndQueues();

   // Add a new emission report (RF track managers only)
   virtual void newReport(Emission* em, double snDbl);

   bool killedNotification(Player* const killedBy = nullptr) override;

   void reset() override;

protected:
   static const unsigned int MAX_TRKS{MIXR_CONFIG_MAX_TRACKS};         // Max tracks
   static const unsigned int MAX_REPORTS{MIXR_CONFIG_MAX_REPORTS};     // Max number of reports

   unsigned int getNewTrackID()                             { return nextTrkId++; }

   virtual void processTrackList(const double dt) =0;                   // Derived class unique

   virtual Emission* getReport(double* const sn);                       // Get the next 'new' report from the queue

   // Track List
   Track*       tracks[MAX_TRKS] {};   // Tracks
   unsigned int nTrks {};              // Number of tracks
   unsigned int maxTrks {MAX_TRKS};    // Max number of tracks (input)
   mutable long trkListLock {};        // Semaphore to protect the track list

   // Prediction parameters
   void makeMatrixA(const double dt);
   double A[3][3] {};            // A Matrix
   bool   haveMatrixA {};        // Matrix A has be initialized
   double alpha {1.0};           // Alpha parameter
   double beta {};               // Beta parameter
   double gamma {};              // Gamma parameter

   unsigned int nextTrkId {1000};          // Next track ID
   unsigned int firstTrkId {1000};         // First (starting) track ID

   base::safe_queue<Emission*> emQueue {MAX_TRKS}; // Emission input queue
   base::safe_queue<double>    snQueue {MAX_TRKS}; // S/N input queue.
   mutable long queueLock {};                      // Semaphore to protect both emQueue and snQueue

   // System class Interface -- phase() callbacks
   void process(const double dt) override;     // Phase 3

   // base::Component protected interface
   bool shutdownNotification() override;

private:
   double maxTrackAge {3.0};      // Max Track age (sec)
   short  type {};                // Track type: the bit-wise OR of various type bits (see enum TypeBits in Track.hpp)
   bool   logTrackUpdates {true}; // input slot; if false, updates to tracks are not logged.

private:
   bool setSlotMaxTracks(const base::Integer* const);       // Sets the maximum number of track files
   bool setSlotMaxTrackAge(const base::INumber* const);      // Sets the maximum age of tracks
   bool setSlotMaxTrackAge(const base::ITime* const);
   bool setSlotFirstTrackId(const base::Integer* const);    // Sets the first (starting) track id number
   virtual bool setSlotAlpha(const base::INumber* const);    // Sets alpha
   virtual bool setSlotBeta(const base::INumber* const);     // Sets beta
   bool setSlotGamma(const base::INumber* const);            // Sets gamma
   bool setSlotLogTrackUpdates(const base::Boolean* const); // Sets logTrackUpdates
};

}
}

#endif
