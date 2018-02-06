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
#define MIXR_VERSION                         1801
#endif

// Max number of interval timers (see Timers.hpp)
constexpr int MIXR_CONFIG_MAX_INTERVAL_TIMERS{500};

// Max number of "player's of interest" (see Gimbal.hpp)
constexpr int MIXR_CONFIG_MAX_PLAYERS_OF_INTEREST{4000};

// Max size of the RF emission queues (see RfSystem.hpp)
constexpr int MIXR_CONFIG_RF_MAX_EMISSIONS{800};

// Max tracks (see TrackManager.hpp)
constexpr int MIXR_CONFIG_MAX_TRACKS{200};

// Max number of radar reports (see Radar.hpp and TrackManager.hpp)
constexpr int MIXR_CONFIG_MAX_REPORTS{200};

// Max number of networked entities (see NetIO.hpp)
constexpr int MIXR_CONFIG_MAX_NETIO_ENTITIES{5000};

// Max number of networked entity types (see NetIO.hpp)
constexpr int MIXR_CONFIG_MAX_NETIO_ENTITY_TYPES{1000};

//  Max number of new, outgoing players published per frame (see NetIO.hpp)
constexpr int MIXR_CONFIG_MAX_NETIO_NEW_OUTGOING{150};

#endif
