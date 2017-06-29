// =============================================================================
// Main configuration file for setting framework parameters
//
// Edit as needed, or redefine using compiler command line options
//
// Used within C++ source code and EDL/EPP files
// Important: only define C pre-processor directives in this file
//
// =============================================================================
#ifndef __mixr_config_H__
#define __mixr_config_H__

// MIXR release version number
#ifndef MIXR_VERSION
#define MIXR_VERSION                         1706c
#endif

// Max number of interval timers (see Timers.hpp)
#ifndef MIXR_CONFIG_MAX_INTERVAL_TIMERS
#define MIXR_CONFIG_MAX_INTERVAL_TIMERS      500
#endif

// Max number of "player's of interest" (see Gimbal.hpp)
#ifndef MIXR_CONFIG_MAX_PLAYERS_OF_INTEREST
#define MIXR_CONFIG_MAX_PLAYERS_OF_INTEREST  4000
#endif

// Max size of the RF emission queues (see RfSystem.hpp)
#ifndef MIXR_CONFIG_RF_MAX_EMISSIONS
#define MIXR_CONFIG_RF_MAX_EMISSIONS         800
#endif

// Max tracks (see TrackManager.hpp)
#ifndef MIXR_CONFIG_MAX_TRACKS
#define MIXR_CONFIG_MAX_TRACKS               200
#endif

// Max number of radar reports (see Radar.hpp and TrackManager.hpp)
#ifndef MIXR_CONFIG_MAX_REPORTS
#define MIXR_CONFIG_MAX_REPORTS              200
#endif

// Max number of networked entites (see NetIO.hpp)
#ifndef MIXR_CONFIG_MAX_NETIO_ENTITIES
#define MIXR_CONFIG_MAX_NETIO_ENTITIES       5000
#endif

// Max number of networked entity types (see NetIO.hpp)
#ifndef MIXR_CONFIG_MAX_NETIO_ENTITY_TYPES
#define MIXR_CONFIG_MAX_NETIO_ENTITY_TYPES   1000
#endif

//  Max number of new, outgoing players published per frame (see NetIO.hpp)
#ifndef MIXR_CONFIG_MAX_NETIO_NEW_OUTGOING
#define MIXR_CONFIG_MAX_NETIO_NEW_OUTGOING   150
#endif

#endif
