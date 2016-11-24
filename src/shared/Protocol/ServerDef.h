#ifndef __SERVERDEF_H
#define __SERVERDEF_H


// ServerMessages.dbc
enum ServerMessageType
{
	SERVER_MSG_SHUTDOWN_TIME = 1,
	SERVER_MSG_RESTART_TIME = 2,
	SERVER_MSG_CUSTOM = 3,
	SERVER_MSG_SHUTDOWN_CANCELLED = 4,
	SERVER_MSG_RESTART_CANCELLED = 5,
};

enum ShutdownMask
{
	SHUTDOWN_MASK_RESTART = 1,
	SHUTDOWN_MASK_IDLE = 2,
};

enum ShutdownExitCode
{
	SHUTDOWN_EXIT_CODE = 0,
	ERROR_EXIT_CODE = 1,
	RESTART_EXIT_CODE = 2,
};

/// Type of server
enum RealmType
{
	REALM_TYPE_NORMAL = 0,
	REALM_TYPE_PVP = 1,
	REALM_TYPE_NORMAL2 = 4,
	REALM_TYPE_RP = 6,
	REALM_TYPE_RPPVP = 8,
	REALM_TYPE_FFA_PVP = 16                                 // custom, free for all pvp mode like arena PvP in all zones except rest activated places and sanctuaries
	// replaced by REALM_PVP in realm list
};

// [-ZERO] Need drop not existed cases
enum RealmZone
{
	REALM_ZONE_UNKNOWN = 0,                           // any language
	REALM_ZONE_DEVELOPMENT = 1,                           // any language
	REALM_ZONE_UNITED_STATES = 2,                           // extended-Latin
	REALM_ZONE_OCEANIC = 3,                           // extended-Latin
	REALM_ZONE_LATIN_AMERICA = 4,                           // extended-Latin
	REALM_ZONE_TOURNAMENT_5 = 5,                           // basic-Latin at create, any at login
	REALM_ZONE_KOREA = 6,                           // East-Asian
	REALM_ZONE_TOURNAMENT_7 = 7,                           // basic-Latin at create, any at login
	REALM_ZONE_ENGLISH = 8,                           // extended-Latin
	REALM_ZONE_GERMAN = 9,                           // extended-Latin
	REALM_ZONE_FRENCH = 10,                          // extended-Latin
	REALM_ZONE_SPANISH = 11,                          // extended-Latin
	REALM_ZONE_RUSSIAN = 12,                          // Cyrillic
	REALM_ZONE_TOURNAMENT_13 = 13,                          // basic-Latin at create, any at login
	REALM_ZONE_TAIWAN = 14,                          // East-Asian
	REALM_ZONE_TOURNAMENT_15 = 15,                          // basic-Latin at create, any at login
	REALM_ZONE_CHINA = 16,                          // East-Asian
	REALM_ZONE_CN1 = 17,                          // basic-Latin at create, any at login
	REALM_ZONE_CN2 = 18,                          // basic-Latin at create, any at login
	REALM_ZONE_CN3 = 19,                          // basic-Latin at create, any at login
	REALM_ZONE_CN4 = 20,                          // basic-Latin at create, any at login
	REALM_ZONE_CN5 = 21,                          // basic-Latin at create, any at login
	REALM_ZONE_CN6 = 22,                          // basic-Latin at create, any at login
	REALM_ZONE_CN7 = 23,                          // basic-Latin at create, any at login
	REALM_ZONE_CN8 = 24,                          // basic-Latin at create, any at login
	REALM_ZONE_TOURNAMENT_25 = 25,                          // basic-Latin at create, any at login
	REALM_ZONE_TEST_SERVER = 26,                          // any language
	REALM_ZONE_TOURNAMENT_27 = 27,                          // basic-Latin at create, any at login
	REALM_ZONE_QA_SERVER = 28,                          // any language
	REALM_ZONE_CN9 = 29                           // basic-Latin at create, any at login
};

#endif //__COMMDEF_H
