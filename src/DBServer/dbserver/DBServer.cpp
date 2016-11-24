/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file
	\ingroup DBServer
	*/

#include "DBServer.h"
#include "Database/DatabaseEnv.h"
#ifdef USE_LUA_CONFIG
#include "Config/ConfigLua.h"
#else
#include "Config/Config.h"
#endif
#include "Platform/Define.h"
#include "SystemConfig.h"
#include "Log.h"
//#include "Opcodes.h"
#include "Network/DBServerSession.h"
#include "Util.h"


#include <algorithm>
#include <mutex>
#include <cstdarg>
#include "global/ConfigMgr.h"
#include "global/SessionMgr.h"

INSTANTIATE_SINGLETON_1(DBServer);

extern void LoadGameObjectModelList();

volatile bool DBServer::m_stopEvent = false;
uint8 DBServer::m_ExitCode = SHUTDOWN_EXIT_CODE;
volatile uint32 DBServer::m_gatewayLoopCounter = 0;


/// DBServer constructor
DBServer::DBServer() 
	: mail_timer(0)
	, mail_timer_expires(0)
{
	m_playerLimit = 0;
	m_allowMovement = true;
	m_ShutdownMask = 0;
	m_ShutdownTimer = 0;
	m_gameTime = time(nullptr);
	m_startTime = m_gameTime;
	
	m_defaultDbcLocale = LOCALE_enUS;
	m_availableDbcLocaleMask = 0;

	
}

/// DBServer destructor
DBServer::~DBServer()
{
	// it is assumed that no other thread is accessing this data when the destructor is called.  therefore, no locks are necessary

	///- Empty the kicked session set
	/*for (auto const session : m_sessions)
		delete session.second;*/

	/*for (auto const cliCommand : m_cliCommandQueue)
		delete cliCommand;

	for (auto const session : m_sessionAddQueue)
		delete session;

	VMAP::VMapFactory::clear();
	MMAP::MMapFactory::clear();*/
}

/// Cleanups before DBServer stop
void DBServer::CleanupsBeforeStop()
{
	sSessionMgr.KickAll();                                       // save and kick all players
	sSessionMgr.UpdateSessions();                               // real players unload required UpdateSessions call
	//sBattleGroundMgr.DeleteAllBattleGrounds();       // unload battleground templates before different singletons destroyed
}

/// Initialize the DBServer
void DBServer::SetInitialWorldSettings()
{
	///- Initialize the random number generator
	srand((unsigned int)time(nullptr));

	///- Time server startup
	uint32 uStartTime = WorldTimer::getMSTime();

	///- Initialize detour memory management
	//dtAllocSetCustom(dtCustomAlloc, dtCustomFree);

	///- Initialize config settings
	sConfigMgr.LoadConfigSettings();

	///- Check the existence of the map files for all races start areas.
	//if (!MapManager::ExistMapAndVMap(0, -6240.32f, 331.033f) ||                     // Dwarf/ Gnome
	//	!MapManager::ExistMapAndVMap(0, -8949.95f, -132.493f) ||                // Human
	//	!MapManager::ExistMapAndVMap(1, -618.518f, -4251.67f) ||                // Orc
	//	!MapManager::ExistMapAndVMap(0, 1676.35f, 1677.45f) ||                  // Scourge
	//	!MapManager::ExistMapAndVMap(1, 10311.3f, 832.463f) ||                  // NightElf
	//	!MapManager::ExistMapAndVMap(1, -2917.58f, -257.98f))                   // Tauren
	//{
	//	sLog.outError("Correct *.map files not found in path '%smaps' or *.vmtree/*.vmtile files in '%svmaps'. Please place *.map and vmap files in appropriate directories or correct the DataDir value in the mangosd.conf file.", m_dataPath.c_str(), m_dataPath.c_str());
	//	Log::WaitBeforeContinueIfNeed();
	//	exit(1);
	//}

	/////- Loading strings. Getting no records means core load has to be canceled because no error message can be output.
	//sLog.outString("Loading MaNGOS strings...");
	//if (!sObjectMgr.LoadMangosStrings())
	//{
	//	Log::WaitBeforeContinueIfNeed();
	//	exit(1);                                            // Error message displayed in function already
	//}

	/////- Update the realm entry in the database with the realm type from the config file
	//// No SQL injection as values are treated as integers

	//// not send custom type REALM_FFA_PVP to realm list
	//uint32 server_type = IsFFAPvPRealm() ? uint32(REALM_TYPE_PVP) : getConfig(G_CFG_UINT32_GAME_TYPE);
	//uint32 realm_zone = getConfig(G_CFG_UINT32_REALM_ZONE);
	////LoginDatabase.PExecute("UPDATE realmlist SET icon = %u, timezone = %u WHERE id = '%u'", server_type, realm_zone, realmID);

	/////- Remove the bones (they should not exist in DB though) and old corpses after a restart
	////CharacterDatabase.PExecute("DELETE FROM corpse WHERE corpse_type = '0' OR time < (UNIX_TIMESTAMP()-'%u')", 3 * DAY);

	/////- Load the DBC files
	//sLog.outString("Initialize DBC data stores...");
	//LoadDBCStores(m_dataPath);
	//DetectDBCLang();
	//sObjectMgr.SetDBCLocaleIndex(GetDefaultDbcLocale());    // Get once for all the locale index of DBC language (console/broadcasts)

	//sLog.outString("Loading Script Names...");
	//sScriptMgr.LoadScriptNames();

	//sLog.outString("Loading WorldTemplate...");
	//sObjectMgr.LoadWorldTemplate();

	//sLog.outString("Loading InstanceTemplate...");
	//sObjectMgr.LoadInstanceTemplate();

	//sLog.outString("Loading SkillLineAbilityMultiMap Data...");
	//sSpellMgr.LoadSkillLineAbilityMap();

	//sLog.outString("Loading SkillRaceClassInfoMultiMap Data...");
	//sSpellMgr.LoadSkillRaceClassInfoMap();

	/////- Clean up and pack instances
	//sLog.outString("Cleaning up instances...");
	//sMapPersistentStateMgr.CleanupInstances();              // must be called before `creature_respawn`/`gameobject_respawn` tables

	//sLog.outString("Packing instances...");
	//sMapPersistentStateMgr.PackInstances();

	//sLog.outString("Packing groups...");
	//sObjectMgr.PackGroupIds();                              // must be after CleanupInstances

	/////- Init highest guids before any guid using table loading to prevent using not initialized guids in some code.
	//sObjectMgr.SetHighestGuids();                           // must be after packing instances
	//sLog.outString();

	//sLog.outString("Loading Page Texts...");
	//sObjectMgr.LoadPageTexts();

	//sLog.outString("Loading Game Object Templates...");     // must be after LoadPageTexts
	//sObjectMgr.LoadGameobjectInfo();

	//sLog.outString("Loading GameObject models...");
	//LoadGameObjectModelList();
	//sLog.outString();

	//sLog.outString("Loading Spell Chain Data...");
	//sSpellMgr.LoadSpellChains();

	//sLog.outString("Loading Spell Elixir types...");
	//sSpellMgr.LoadSpellElixirs();

	//sLog.outString("Loading Spell Facing Flags...");
	//sSpellMgr.LoadFacingCasterFlags();

	//sLog.outString("Loading Spell Learn Skills...");
	//sSpellMgr.LoadSpellLearnSkills();                       // must be after LoadSpellChains

	//sLog.outString("Loading Spell Learn Spells...");
	//sSpellMgr.LoadSpellLearnSpells();

	//sLog.outString("Loading Spell Proc Event conditions...");
	//sSpellMgr.LoadSpellProcEvents();

	//sLog.outString("Loading Spell Bonus Data...");
	//sSpellMgr.LoadSpellBonuses();

	//sLog.outString("Loading Spell Proc Item Enchant...");
	//sSpellMgr.LoadSpellProcItemEnchant();                   // must be after LoadSpellChains

	//sLog.outString("Loading Aggro Spells Definitions...");
	//sSpellMgr.LoadSpellThreats();

	//sLog.outString("Loading NPC Texts...");
	//sObjectMgr.LoadGossipText();

	//sLog.outString("Loading Item Random Enchantments Table...");
	//LoadRandomEnchantmentsTable();

	//sLog.outString("Loading Item Templates...");            // must be after LoadRandomEnchantmentsTable and LoadPageTexts
	//sObjectMgr.LoadItemPrototypes();

	//sLog.outString("Loading Item Texts...");
	//sObjectMgr.LoadItemTexts();

	//sLog.outString("Loading Creature Model Based Info Data...");
	//sObjectMgr.LoadCreatureModelInfo();

	//sLog.outString("Loading Equipment templates...");
	//sObjectMgr.LoadEquipmentTemplates();

	//sLog.outString("Loading Creature Stats...");
	//sObjectMgr.LoadCreatureClassLvlStats();

	//sLog.outString("Loading Creature templates...");
	//sObjectMgr.LoadCreatureTemplates();

	//sLog.outString("Loading Creature template spells...");
	//sObjectMgr.LoadCreatureTemplateSpells();

	//sLog.outString("Loading SpellsScriptTarget...");
	//sSpellMgr.LoadSpellScriptTarget();                      // must be after LoadCreatureTemplates and LoadGameobjectInfo

	//sLog.outString("Loading ItemRequiredTarget...");
	//sObjectMgr.LoadItemRequiredTarget();

	//sLog.outString("Loading Reputation Reward Rates...");
	//sObjectMgr.LoadReputationRewardRate();

	//sLog.outString("Loading Creature Reputation OnKill Data...");
	//sObjectMgr.LoadReputationOnKill();

	//sLog.outString("Loading Reputation Spillover Data...");
	//sObjectMgr.LoadReputationSpilloverTemplate();

	//sLog.outString("Loading Points Of Interest Data...");
	//sObjectMgr.LoadPointsOfInterest();

	//sLog.outString("Loading Pet Create Spells...");
	//sObjectMgr.LoadPetCreateSpells();

	//sLog.outString("Loading Creature Data...");
	//sObjectMgr.LoadCreatures();

	//sLog.outString("Loading Creature Addon Data...");
	//sObjectMgr.LoadCreatureAddons();                        // must be after LoadCreatureTemplates() and LoadCreatures()
	//sLog.outString(">>> Creature Addon Data loaded");
	//sLog.outString();

	//sLog.outString("Loading Gameobject Data...");
	//sObjectMgr.LoadGameObjects();

	//sLog.outString("Loading CreatureLinking Data...");      // must be after Creatures
	//sCreatureLinkingMgr.LoadFromDB();

	//sLog.outString("Loading Objects Pooling Data...");
	//sPoolMgr.LoadFromDB();

	//sLog.outString("Loading Weather Data...");
	//sWeatherMgr.LoadWeatherZoneChances();

	//sLog.outString("Loading Quests...");
	//sObjectMgr.LoadQuests();                                // must be loaded after DBCs, creature_template, item_template, gameobject tables

	//sLog.outString("Loading Quests Relations...");
	//sObjectMgr.LoadQuestRelations();                        // must be after quest load
	//sLog.outString(">>> Quests Relations loaded");
	//sLog.outString();

	//sLog.outString("Loading Game Event Data...");           // must be after sPoolMgr.LoadFromDB and quests to properly load pool events and quests for events
	//sGameEventMgr.LoadFromDB();
	//sLog.outString(">>> Game Event Data loaded");
	//sLog.outString();

	//// Load Conditions
	//sLog.outString("Loading Conditions...");
	//sObjectMgr.LoadConditions();

	//sLog.outString("Creating map persistent states for non-instanceable maps...");     // must be after PackInstances(), LoadCreatures(), sPoolMgr.LoadFromDB(), sGameEventMgr.LoadFromDB();
	//sMapPersistentStateMgr.InitWorldMaps();
	//sLog.outString();

	//sLog.outString("Loading Creature Respawn Data...");     // must be after LoadCreatures(), and sMapPersistentStateMgr.InitWorldMaps()
	//sMapPersistentStateMgr.LoadCreatureRespawnTimes();

	//sLog.outString("Loading Gameobject Respawn Data...");   // must be after LoadGameObjects(), and sMapPersistentStateMgr.InitWorldMaps()
	//sMapPersistentStateMgr.LoadGameobjectRespawnTimes();

	//sLog.outString("Loading SpellArea Data...");            // must be after quest load
	//sSpellMgr.LoadSpellAreas();

	//sLog.outString("Loading AreaTrigger definitions...");
	//sObjectMgr.LoadAreaTriggerTeleports();                  // must be after item template load

	//sLog.outString("Loading Quest Area Triggers...");
	//sObjectMgr.LoadQuestAreaTriggers();                     // must be after LoadQuests

	//sLog.outString("Loading Tavern Area Triggers...");
	//sObjectMgr.LoadTavernAreaTriggers();

	//sLog.outString("Loading AreaTrigger script names...");
	//sScriptMgr.LoadAreaTriggerScripts();

	//sLog.outString("Loading event id script names...");
	//sScriptMgr.LoadEventIdScripts();

	//sLog.outString("Loading Graveyard-zone links...");
	//sObjectMgr.LoadGraveyardZones();

	//sLog.outString("Loading spell target destination coordinates...");
	//sSpellMgr.LoadSpellTargetPositions();

	//sLog.outString("Loading SpellAffect definitions...");
	//sSpellMgr.LoadSpellAffects();

	//sLog.outString("Loading spell pet auras...");
	//sSpellMgr.LoadSpellPetAuras();

	//sLog.outString("Loading Player Create Info & Level Stats...");
	//sObjectMgr.LoadPlayerInfo();
	//sLog.outString(">>> Player Create Info & Level Stats loaded");
	//sLog.outString();

	//sLog.outString("Loading Exploration BaseXP Data...");
	//sObjectMgr.LoadExplorationBaseXP();

	//sLog.outString("Loading Pet Name Parts...");
	//sObjectMgr.LoadPetNames();

	////CharacterDatabaseCleaner::CleanDatabase();
	//sLog.outString();

	//sLog.outString("Loading the max pet number...");
	//sObjectMgr.LoadPetNumber();

	//sLog.outString("Loading pet level stats...");
	//sObjectMgr.LoadPetLevelInfo();

	//sLog.outString("Loading Player Corpses...");
	//sObjectMgr.LoadCorpses();

	//sLog.outString("Loading Loot Tables...");
	//LoadLootTables();
	//sLog.outString(">>> Loot Tables loaded");
	//sLog.outString();

	//sLog.outString("Loading Skill Fishing base level requirements...");
	//sObjectMgr.LoadFishingBaseSkillLevel();

	//sLog.outString("Loading Npc Text Id...");
	//sObjectMgr.LoadNpcGossips();                            // must be after load Creature and LoadGossipText

	//sLog.outString("Loading Gossip scripts...");
	//sScriptMgr.LoadGossipScripts();                         // must be before gossip menu options

	//sObjectMgr.LoadGossipMenus();

	//sLog.outString("Loading Vendors...");
	//sObjectMgr.LoadVendorTemplates();                       // must be after load ItemTemplate
	//sObjectMgr.LoadVendors();                               // must be after load CreatureTemplate, VendorTemplate, and ItemTemplate

	//sLog.outString("Loading Trainers...");
	//sObjectMgr.LoadTrainerTemplates();                      // must be after load CreatureTemplate
	//sObjectMgr.LoadTrainers();                              // must be after load CreatureTemplate, TrainerTemplate

	//sLog.outString("Loading Waypoint scripts...");          // before loading from creature_movement
	//sScriptMgr.LoadCreatureMovementScripts();

	//sLog.outString("Loading Waypoints...");
	//sWaypointMgr.Load();

	//sLog.outString("Loading ReservedNames...");
	//sObjectMgr.LoadReservedPlayersNames();

	//sLog.outString("Loading GameObjects for quests...");
	//sObjectMgr.LoadGameObjectForQuests();

	//sLog.outString("Loading BattleMasters...");
	//sBattleGroundMgr.LoadBattleMastersEntry();

	//sLog.outString("Loading BattleGround event indexes...");
	//sBattleGroundMgr.LoadBattleEventIndexes();

	//sLog.outString("Loading GameTeleports...");
	//sObjectMgr.LoadGameTele();

	/////- Loading localization data
	//sLog.outString("Loading Localization strings...");
	//sObjectMgr.LoadCreatureLocales();                       // must be after CreatureInfo loading
	//sObjectMgr.LoadGameObjectLocales();                     // must be after GameobjectInfo loading
	//sObjectMgr.LoadItemLocales();                           // must be after ItemPrototypes loading
	//sObjectMgr.LoadQuestLocales();                          // must be after QuestTemplates loading
	//sObjectMgr.LoadGossipTextLocales();                     // must be after LoadGossipText
	//sObjectMgr.LoadPageTextLocales();                       // must be after PageText loading
	//sObjectMgr.LoadGossipMenuItemsLocales();                // must be after gossip menu items loading
	//sObjectMgr.LoadPointOfInterestLocales();                // must be after POI loading
	//sLog.outString(">>> Localization strings loaded");
	//sLog.outString();

	/////- Load dynamic data tables from the database
	//sLog.outString("Loading Auctions...");
	//sAuctionMgr.LoadAuctionItems();
	//sAuctionMgr.LoadAuctions();
	//sLog.outString(">>> Auctions loaded");
	//sLog.outString();

	//sLog.outString("Loading Guilds...");
	//sGuildMgr.LoadGuilds();

	//sLog.outString("Loading Groups...");
	//sObjectMgr.LoadGroups();

	//sLog.outString("Returning old mails...");
	//sObjectMgr.ReturnOrDeleteOldMails(false);

	//sLog.outString("Loading GM tickets...");
	//sTicketMgr.LoadGMTickets();

	/////- Load and initialize DBScripts Engine
	//sLog.outString("Loading DB-Scripts Engine...");
	//sScriptMgr.LoadQuestStartScripts();                     // must be after load Creature/Gameobject(Template/Data) and QuestTemplate
	//sScriptMgr.LoadQuestEndScripts();                       // must be after load Creature/Gameobject(Template/Data) and QuestTemplate
	//sScriptMgr.LoadSpellScripts();                          // must be after load Creature/Gameobject(Template/Data)
	//sScriptMgr.LoadGameObjectScripts();                     // must be after load Creature/Gameobject(Template/Data)
	//sScriptMgr.LoadGameObjectTemplateScripts();             // must be after load Creature/Gameobject(Template/Data)
	//sScriptMgr.LoadEventScripts();                          // must be after load Creature/Gameobject(Template/Data)
	//sScriptMgr.LoadCreatureDeathScripts();                  // must be after load Creature/Gameobject(Template/Data)
	//sLog.outString(">>> Scripts loaded");
	//sLog.outString();

	//sLog.outString("Loading Scripts text locales...");      // must be after Load*Scripts calls
	//sScriptMgr.LoadDbScriptStrings();

	/////- Load and initialize EventAI Scripts
	//sLog.outString("Loading CreatureEventAI Texts...");
	//sEventAIMgr.LoadCreatureEventAI_Texts(false);           // false, will checked in LoadCreatureEventAI_Scripts

	//sLog.outString("Loading CreatureEventAI Summons...");
	//sEventAIMgr.LoadCreatureEventAI_Summons(false);         // false, will checked in LoadCreatureEventAI_Scripts

	//sLog.outString("Loading CreatureEventAI Scripts...");
	//sEventAIMgr.LoadCreatureEventAI_Scripts();

	/////- Load and initialize scripting library
	//sLog.outString("Initializing Scripting Library...");
	//switch (sScriptMgr.LoadScriptLibrary(MANGOS_SCRIPT_NAME))
	//{
	//case SCRIPT_LOAD_OK:
	//	sLog.outString("Scripting library loaded.");
	//	break;
	//case SCRIPT_LOAD_ERR_NOT_FOUND:
	//	sLog.outError("Scripting library not found or not accessible.");
	//	break;
	//case SCRIPT_LOAD_ERR_WRONG_API:
	//	sLog.outError("Scripting library has wrong list functions (outdated?).");
	//	break;
	//case SCRIPT_LOAD_ERR_OUTDATED:
	//	sLog.outError("Scripting library build for old mangosd revision. You need rebuild it.");
	//	break;
	//}
	//sLog.outString();

	/////- Initialize game time and timers
	//sLog.outString("Initialize game time and timers");
	//m_gameTime = time(nullptr);
	//m_startTime = m_gameTime;

	//tm local;
	//time_t curr;
	//time(&curr);
	//local = *(localtime(&curr));                            // dereference and assign
	//char isoDate[128];
	//sprintf(isoDate, "%04d-%02d-%02d %02d:%02d:%02d",
	//	local.tm_year + 1900, local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);

	//LoginDatabase.PExecute("INSERT INTO uptime (realmid, starttime, startstring, uptime) VALUES('%u', " UI64FMTD ", '%s', 0)",
	//	realmID, uint64(m_startTime), isoDate);

	//m_timers[GUPDATE_AUCTIONS].SetInterval(MINUTE * IN_MILLISECONDS);
	//m_timers[GUPDATE_UPTIME].SetInterval(getConfig(G_CFG_UINT32_UPTIME_UPDATE)*MINUTE * IN_MILLISECONDS);
	//// Update "uptime" table based on configuration entry in minutes.
	//m_timers[GUPDATE_CORPSES].SetInterval(20 * MINUTE * IN_MILLISECONDS);
	//m_timers[GUPDATE_DELETECHARS].SetInterval(DAY * IN_MILLISECONDS); // check for chars to delete every day

	//// for AhBot
	//m_timers[GUPDATE_AHBOT].SetInterval(20 * IN_MILLISECONDS); // every 20 sec

	//// to set mailtimer to return mails every day between 4 and 5 am
	//// mailtimer is increased when updating auctions
	//// one second is 1000 -(tested on win system)
	//mail_timer = uint32((((localtime(&m_gameTime)->tm_hour + 20) % 24) * HOUR * IN_MILLISECONDS) / m_timers[GUPDATE_AUCTIONS].GetInterval());
	//// 1440
	//mail_timer_expires = uint32((DAY * IN_MILLISECONDS) / (m_timers[GUPDATE_AUCTIONS].GetInterval()));
	//DEBUG_LOG("Mail timer set to: %u, mail return is called every %u minutes", mail_timer, mail_timer_expires);

	/////- Initialize static helper structures
	//AIRegistry::Initialize();
	//Player::InitVisibleBits();

	/////- Initialize MapManager
	//sLog.outString("Starting Map System");
	//sMapMgr.Initialize();
	//sLog.outString();

	/////- Initialize Battlegrounds
	//sLog.outString("Starting BattleGround System");
	//sBattleGroundMgr.CreateInitialBattleGrounds();

	/////- Initialize Outdoor PvP
	//sLog.outString("Starting Outdoor PvP System");
	//sOutdoorPvPMgr.InitOutdoorPvP();

	//// Not sure if this can be moved up in the sequence (with static data loading) as it uses MapManager
	//sLog.outString("Loading Transports...");
	//sMapMgr.LoadTransports();

	//sLog.outString("Deleting expired bans...");
	//LoginDatabase.Execute("DELETE FROM ip_banned WHERE unbandate<=UNIX_TIMESTAMP() AND unbandate<>bandate");
	//sLog.outString();

	//sLog.outString("Starting server Maintenance system...");
	//InitServerMaintenanceCheck();

	//sLog.outString("Loading Honor Standing list...");
	//sObjectMgr.LoadStandingList();

	//sLog.outString("Starting Game Event system...");
	//uint32 nextGameEvent = sGameEventMgr.Initialize();
	//m_timers[GUPDATE_EVENTS].SetInterval(nextGameEvent);    // depend on next event
	//sLog.outString();

	//sLog.outString("Loading grids for active creatures or transports...");
	//sObjectMgr.LoadActiveEntities(nullptr);
	//sLog.outString();

	//// Delete all characters which have been deleted X days before
	//Player::DeleteOldCharacters();

	//sLog.outString("Initialize AuctionHouseBot...");
	//sAuctionBot.Initialize();
	//sLog.outString();

	sLog.outString("---------------------------------------");
	sLog.outString("      CMANGOS: DBServer initialized       ");
	sLog.outString("---------------------------------------");
	sLog.outString();

	uint32 uStartInterval = WorldTimer::getMSTimeDiff(uStartTime, WorldTimer::getMSTime());
	sLog.outString("SERVER STARTUP TIME: %i minutes %i seconds", uStartInterval / 60000, (uStartInterval % 60000) / 1000);
	sLog.outString();
}

void DBServer::DetectDBCLang()
{
	uint32 m_lang_confid = sConfig.GetIntDefault("DBC.Locale", 255);

	if (m_lang_confid != 255 && m_lang_confid >= MAX_LOCALE)
	{
		sLog.outError("Incorrect DBC.Locale! Must be >= 0 and < %d (set to 0)", MAX_LOCALE);
		m_lang_confid = LOCALE_enUS;
	}

	/*ChrRacesEntry const* race = sChrRacesStore.LookupEntry(RACE_HUMAN);
	MANGOS_ASSERT(race);*/

	std::string availableLocalsStr;

	//uint32 default_locale = MAX_LOCALE;
	//for (int i = MAX_LOCALE - 1; i >= 0; --i)
	//{
	//	if (strlen(race->name[i]) > 0)                      // check by race names
	//	{
	//		default_locale = i;
	//		m_availableDbcLocaleMask |= (1 << i);
	//		availableLocalsStr += localeNames[i];
	//		availableLocalsStr += " ";
	//	}
	//}

	/*if (default_locale != m_lang_confid && m_lang_confid < MAX_LOCALE &&
		(m_availableDbcLocaleMask & (1 << m_lang_confid)))
		{
		default_locale = m_lang_confid;
		}

		if (default_locale >= MAX_LOCALE)
		{
		sLog.outError("Unable to determine your DBC Locale! (corrupt DBC?)");
		Log::WaitBeforeContinueIfNeed();
		exit(1);
		}

		m_defaultDbcLocale = LocaleConstant(default_locale);

		sLog.outString("Using %s DBC Locale as default. All available DBC locales: %s", localeNames[m_defaultDbcLocale], availableLocalsStr.empty() ? "<none>" : availableLocalsStr.c_str());
		sLog.outString();*/
}

/// Update the DBServer !
void DBServer::Update(uint32 diff)
{
	///- Update the different timers
	for (int i = 0; i < GUPDATE_COUNT; ++i)
	{
		IntervalTimer * ITime = sConfigMgr.GetTimers(i);
		if (ITime == NULL)
			continue;

		if (ITime->GetCurrent() >= 0)
			ITime->Update(diff);
		else
			ITime->SetCurrent(0);
	}

	///- Update the game time and check for shutdown time
	_UpdateGameTime();

	///-Update mass mailer tasks if any
	//sMassMailMgr.Update();

	/// <ul><li> Handle auctions when the timer has passed
	//if (m_timers[GUPDATE_AUCTIONS].Passed())
	//{
	//	m_timers[GUPDATE_AUCTIONS].Reset();

	//	///- Update mails (return old mails with item, or delete them)
	//	//(tested... works on win)
	//	if (++mail_timer > mail_timer_expires)
	//	{
	//		mail_timer = 0;
	//		//sObjectMgr.ReturnOrDeleteOldMails(true);
	//	}

	//	///- Handle expired auctions
	//	sAuctionMgr.Update();
	//}

	/// <li> Handle AHBot operations
	/*if (m_timers[GUPDATE_AHBOT].Passed())
	{
		sAuctionBot.Update();
		m_timers[GUPDATE_AHBOT].Reset();
	}*/

	/// <li> Handle session updates
	sSessionMgr.UpdateSessions();

	/// <li> Update uptime table
	//if (m_timers[GUPDATE_UPTIME].Passed())
	//{
	//	uint32 tmpDiff = uint32(m_gameTime - m_startTime);
	//	uint32 maxClientsNum = GetMaxActiveSessionCount();

	//	m_timers[GUPDATE_UPTIME].Reset();
	//	//LoginDatabase.PExecute("UPDATE uptime SET uptime = %u, maxplayers = %u WHERE realmid = %u AND starttime = " UI64FMTD, tmpDiff, maxClientsNum, realmID, uint64(m_startTime));
	//}

	/// <li> Handle all other objects
	///- Update objects (maps, transport, creatures,...)
	//sMapMgr.Update(diff);
	//sBattleGroundMgr.Update(diff);
	//sOutdoorPvPMgr.Update(diff);

	///- Delete all characters which have been deleted X days before
	/*if (m_timers[GUPDATE_DELETECHARS].Passed())
	{
	m_timers[GUPDATE_DELETECHARS].Reset();
	Player::DeleteOldCharacters();
	}*/

	// execute callbacks from sql queries that were queued recently
	UpdateResultQueue();

	///- Erase corpses once every 20 minutes
	/*if (m_timers[GUPDATE_CORPSES].Passed())
	{
	m_timers[GUPDATE_CORPSES].Reset();

	sObjectAccessor.RemoveOldCorpses();
	}*/

	///- Process Game events when necessary
	//if (m_timers[GUPDATE_EVENTS].Passed())
	//{
	//	m_timers[GUPDATE_EVENTS].Reset();                   // to give time for Update() to be processed
	//	uint32 nextGameEvent = sGameEventMgr.Update();
	//	m_timers[GUPDATE_EVENTS].SetInterval(nextGameEvent);
	//	m_timers[GUPDATE_EVENTS].Reset();
	//}

	/// </ul>
	///- Move all creatures with "delayed move" and remove and delete all objects with "delayed remove"
	//sMapMgr.RemoveAllObjectsInRemoveList();

	// update the instance reset times
	//sMapPersistentStateMgr.Update();

	if (m_MaintenanceTimeChecker < diff)
	{
		/*if (GetDateToday() >= m_NextMaintenanceDate)
		{
		ServerMaintenanceStart();
		sObjectMgr.LoadStandingList();
		}*/
		m_MaintenanceTimeChecker = 600000; // check 10 minutes
	}
	else
		m_MaintenanceTimeChecker -= diff;

	// And last, but not least handle the issued cli commands
	//ProcessCliCommands();

	// cleanup unused GridMap objects as well as VMaps
	//sTerrainMgr.Update(diff);

	AddHeartTime(diff); //todo:hunter refresh per second
}

namespace MaNGOS
{
	class WorldWorldTextBuilder
	{
	public:
		typedef std::vector<WorldPacket*> WorldPacketList;
		explicit WorldWorldTextBuilder(int32 textId, va_list* args = nullptr) : i_textId(textId), i_args(args) {}
		void operator()(WorldPacketList& data_list, int32 loc_idx)
		{
			//char const* text = sObjectMgr.GetMangosString(i_textId, loc_idx);

			//if (i_args)
			//{
			//	// we need copy va_list before use or original va_list will corrupted
			//	va_list ap;
			//	va_copy(ap, *i_args);

			//	char str[2048];
			//	vsnprintf(str, 2048, text, ap);
			//	va_end(ap);

			//	do_helper(data_list, &str[0]);
			//}
			//else
			//	do_helper(data_list, (char*)text);
		}
	private:
		char* lineFromMessage(char*& pos) { char* start = strtok(pos, "\n"); pos = nullptr; return start; }
		void do_helper(WorldPacketList& data_list, char* text)
		{
			/*char* pos = text;

			while (char* line = lineFromMessage(pos))
			{
				WorldPacket* data = new WorldPacket();
				ChatHandler::BuildChatPacket(*data, CHAT_MSG_SYSTEM, line);
				data_list.push_back(data);
			}*/
		}

		int32 i_textId;
		va_list* i_args;
	};
}                                                           // namespace MaNGOS

/// Sends a system message to all players
void DBServer::SendWorldText(int32 string_id, ...)
{
	va_list ap;
	va_start(ap, string_id);

	MaNGOS::WorldWorldTextBuilder wt_builder(string_id, &ap);
	/*MaNGOS::LocalizedPacketListDo<MaNGOS::WorldWorldTextBuilder> wt_do(wt_builder);
	for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
	{
		if (DBServerSession* session = itr->second)
		{
			Player* player = session->GetPlayer();
			if (player && player->IsInWorld())
				wt_do(player);
		}
	}*/

	va_end(ap);
}

/// Sends a packet to all players with optional team and instance restrictions
void DBServer::SendGlobalMessage(WorldPacket* packet)
{
	//for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
	{
		/*if (DBServerSession* session = itr->second)
		{
			Player* player = session->GetPlayer();
			if (player && player->IsInWorld())
				session->SendPacket(packet);
		}*/
	}
}

/// Sends a server message to the specified or all players
void DBServer::SendServerMessage(ServerMessageType type, const char* text /*=""*/, Player* player /*= nullptr*/)
{
	//WorldPacket data(SMSG_SERVER_MESSAGE, 50);              // guess size
	//data << uint32(type);
	//data << text;

	//if (player)
	//	player->GetSession()->SendPacket(&data);
	//else
	//	SendGlobalMessage(&data);
}

/// Sends a zone under attack message to all players not in an instance
void DBServer::SendZoneUnderAttackMessage(uint32 zoneId, Team team)
{
	/*WorldPacket data(SMSG_ZONE_UNDER_ATTACK, 4);
	data << uint32(zoneId);

	for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
	{
		if (DBServerSession* session = itr->second)
		{
			Player* player = session->GetPlayer();
			if (player && player->IsInWorld() && player->GetTeam() == team && !player->GetMap()->Instanceable())
				itr->second->SendPacket(&data);
		}
	}*/
}

/// Sends a DBServer defense message to all players not in an instance
void DBServer::SendDefenseMessage(uint32 zoneId, int32 textId)
{
	/*for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
	{
		if (DBServerSession* session = itr->second)
		{
			Player* player = session->GetPlayer();
			if (player && player->IsInWorld() && !player->GetMap()->Instanceable())
			{
				char const* message = session->GetMangosString(textId);
				uint32 messageLength = strlen(message) + 1;

				WorldPacket data(SMSG_DEFENSE_MESSAGE, 4 + 4 + messageLength);
				data << uint32(zoneId);
				data << uint32(messageLength);
				data << message;
				session->SendPacket(&data);
			}
		}
	}*/
}

/// Kick (and save) all players
//void DBServer::KickAll()
//{
//	m_QueuedSessions.clear();                               // prevent send queue update packet and login queued sessions
//
//	// session not removed at kick and will removed in next update tick
//	for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
//		itr->second->KickPlayer();
//}

/// Kick (and save) all players with security level less `sec`
void DBServer::KickAllLess(AccountTypes sec)
{
	// session not removed at kick and will removed in next update tick
	/*for (SessionMap::const_iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
		if (DBServerSession* session = itr->second)
		if (session->GetSecurity() < sec)
		session->KickPlayer();*/
}

/// Ban an account or ban an IP address, duration_secs if it is positive used, otherwise permban
BanReturn DBServer::BanAccount(BanMode mode, std::string nameOrIP, uint32 duration_secs, std::string reason, const std::string& author)
{
	//LoginDatabase.escape_string(nameOrIP);
	//LoginDatabase.escape_string(reason);
	//std::string safe_author = author;
	//LoginDatabase.escape_string(safe_author);

	//QueryResult* resultAccounts;                            // used for kicking

	/////- Update the database with ban information
	//switch (mode)
	//{
	//case BAN_IP:
	//	// No SQL injection as strings are escaped
	//	//resultAccounts = LoginDatabase.PQuery("SELECT id FROM account WHERE last_ip = '%s'", nameOrIP.c_str());
	//	//LoginDatabase.PExecute("INSERT INTO ip_banned VALUES ('%s',UNIX_TIMESTAMP(),UNIX_TIMESTAMP()+%u,'%s','%s')", nameOrIP.c_str(), duration_secs, safe_author.c_str(), reason.c_str());
	//	break;
	//case BAN_ACCOUNT:
	//	// No SQL injection as string is escaped
	//	//resultAccounts = LoginDatabase.PQuery("SELECT id FROM account WHERE username = '%s'", nameOrIP.c_str());
	//	break;
	//case BAN_CHARACTER:
	//	// No SQL injection as string is escaped
	//	//resultAccounts = CharacterDatabase.PQuery("SELECT account FROM characters WHERE name = '%s'", nameOrIP.c_str());
	//	break;
	//default:
	//	return BAN_SYNTAX_ERROR;
	//}

	//if (!resultAccounts)
	//{
	//	if (mode == BAN_IP)
	//		return BAN_SUCCESS;                             // ip correctly banned but nobody affected (yet)
	//	else
	//		return BAN_NOTFOUND;                            // Nobody to ban
	//}

	/////- Disconnect all affected players (for IP it can be several)
	//do
	//{
	//	Field* fieldsAccount = resultAccounts->Fetch();
	//	uint32 account = fieldsAccount->GetUInt32();

	//	if (mode != BAN_IP)
	//	{
	//		// No SQL injection as strings are escaped
	//		LoginDatabase.PExecute("INSERT INTO account_banned VALUES ('%u', UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+%u, '%s', '%s', '1')",
	//			account, duration_secs, safe_author.c_str(), reason.c_str());
	//	}

	//	if (DBServerSession* sess = FindSession(account))
	//		if (std::string(sess->GetPlayerName()) != author)
	//			sess->KickPlayer();
	//} while (resultAccounts->NextRow());

	//delete resultAccounts;
	return BAN_SUCCESS;
}

/// Remove a ban from an account or IP address
bool DBServer::RemoveBanAccount(BanMode mode, std::string nameOrIP)
{
	//if (mode == BAN_IP)
	//{
	//	LoginDatabase.escape_string(nameOrIP);
	//	LoginDatabase.PExecute("DELETE FROM ip_banned WHERE ip = '%s'", nameOrIP.c_str());
	//}
	//else
	//{
	//	uint32 account = 0;
	//	if (mode == BAN_ACCOUNT)
	//		account = sAccountMgr.GetId(nameOrIP);
	//	else if (mode == BAN_CHARACTER)
	//		account = sObjectMgr.GetPlayerAccountIdByPlayerName(nameOrIP);

	//	if (!account)
	//		return false;

	//	// NO SQL injection as account is uint32
	//	LoginDatabase.PExecute("UPDATE account_banned SET active = '0' WHERE id = '%u'", account);
	//}
	return true;
}

/// Update the game time
void DBServer::_UpdateGameTime()
{
	///- update the time
	time_t thisTime = time(nullptr);
	uint32 elapsed = uint32(thisTime - m_gameTime);
	m_gameTime = thisTime;

	///- if there is a shutdown timer
	if (!m_stopEvent && m_ShutdownTimer > 0 && elapsed > 0)
	{
		///- ... and it is overdue, stop the DBServer (set m_stopEvent)
		if (m_ShutdownTimer <= elapsed)
		{
			if (!(m_ShutdownMask & SHUTDOWN_MASK_IDLE) || sSessionMgr.GetActiveAndQueuedSessionCount() == 0)
				m_stopEvent = true;                         // exist code already set
			else
				m_ShutdownTimer = 1;                        // minimum timer value to wait idle state
		}
		///- ... else decrease it and if necessary display a shutdown countdown to the users
		else
		{
			m_ShutdownTimer -= elapsed;

			ShutdownMsg();
		}
	}
}

/// Shutdown the server
void DBServer::ShutdownServ(uint32 time, uint32 options, uint8 exitcode)
{
	// ignore if server shutdown at next tick
	if (m_stopEvent)
		return;

	m_ShutdownMask = options;
	m_ExitCode = exitcode;

	///- If the shutdown time is 0, set m_stopEvent (except if shutdown is 'idle' with remaining sessions)
	if (time == 0)
	{
		if (!(options & SHUTDOWN_MASK_IDLE) || sSessionMgr.GetActiveAndQueuedSessionCount() == 0)
			m_stopEvent = true;                             // exist code already set
		else
			m_ShutdownTimer = 1;                            // So that the session count is re-evaluated at next DBServer tick
	}
	///- Else set the shutdown timer and warn users
	else
	{
		m_ShutdownTimer = time;
		ShutdownMsg(true);
	}
}

/// Display a shutdown message to the user(s)
void DBServer::ShutdownMsg(bool show /*= false*/, Player* player /*= nullptr*/)
{
	// not show messages for idle shutdown mode
	if (m_ShutdownMask & SHUTDOWN_MASK_IDLE)
		return;

	///- Display a message every 12 hours, 1 hour, 5 minutes, 1 minute and 15 seconds
	if (show ||
		(m_ShutdownTimer < 5 * MINUTE && (m_ShutdownTimer % 15) == 0) ||            // < 5 min; every 15 sec
		(m_ShutdownTimer < 15 * MINUTE && (m_ShutdownTimer % MINUTE) == 0) ||       // < 15 min; every 1 min
		(m_ShutdownTimer < 30 * MINUTE && (m_ShutdownTimer % (5 * MINUTE)) == 0) || // < 30 min; every 5 min
		(m_ShutdownTimer < 12 * HOUR && (m_ShutdownTimer % HOUR) == 0) ||           // < 12 h; every 1 h
		(m_ShutdownTimer >= 12 * HOUR && (m_ShutdownTimer % (12 * HOUR)) == 0))     // >= 12 h; every 12 h
	{
		std::string str = secsToTimeString(m_ShutdownTimer);

		ServerMessageType msgid = (m_ShutdownMask & SHUTDOWN_MASK_RESTART) ? SERVER_MSG_RESTART_TIME : SERVER_MSG_SHUTDOWN_TIME;

		SendServerMessage(msgid, str.c_str(), player);
		DEBUG_LOG("Server is %s in %s", (m_ShutdownMask & SHUTDOWN_MASK_RESTART ? "restart" : "shutting down"), str.c_str());
	}
}

/// Cancel a planned server shutdown
void DBServer::ShutdownCancel()
{
	// nothing to cancel or too late
	if (!m_ShutdownTimer || m_stopEvent)
		return;

	ServerMessageType msgid = (m_ShutdownMask & SHUTDOWN_MASK_RESTART) ? SERVER_MSG_RESTART_CANCELLED : SERVER_MSG_SHUTDOWN_CANCELLED;

	m_ShutdownMask = 0;
	m_ShutdownTimer = 0;
	m_ExitCode = SHUTDOWN_EXIT_CODE;                       // to default value
	SendServerMessage(msgid);

	DEBUG_LOG("Server %s cancelled.", (m_ShutdownMask & SHUTDOWN_MASK_RESTART ? "restart" : "shutdown"));
}


void DBServer::ServerMaintenanceStart()
{
	uint32 LastWeekEnd = GetDateLastMaintenanceDay();
	m_NextMaintenanceDate = LastWeekEnd + 7; // next maintenance begin

	if (m_NextMaintenanceDate <= GetDateToday())            // avoid loop in manually case, maybe useless
		m_NextMaintenanceDate += 7;

	// flushing rank points list ( standing must be reloaded after server maintenance )
	//sObjectMgr.FlushRankPoints(LastWeekEnd);

	// save and update all online players
	/*for (SessionMap::iterator itr = m_sessions.begin(); itr != m_sessions.end(); ++itr)
		if (itr->second->GetPlayer() && itr->second->GetPlayer()->IsInWorld())
		itr->second->GetPlayer()->SaveToDB();*/

	//CharacterDatabase.PExecute("UPDATE saved_variables SET NextMaintenanceDate = '" UI64FMTD "'", uint64(m_NextMaintenanceDate));
}

void DBServer::InitServerMaintenanceCheck()
{
	/*QueryResult* result = CharacterDatabase.Query("SELECT NextMaintenanceDate FROM saved_variables");
	if (!result)
	{
		DEBUG_LOG("Maintenance date not found in SavedVariables, reseting it now.");
		uint32 mDate = GetDateLastMaintenanceDay();
		m_NextMaintenanceDate = mDate == GetDateToday() ? mDate : mDate + 7;
		CharacterDatabase.PExecute("INSERT INTO saved_variables (NextMaintenanceDate) VALUES ('" UI64FMTD "')", uint64(m_NextMaintenanceDate));
	}
	else
	{
		m_NextMaintenanceDate = (*result)[0].GetUInt64();
		delete result;
	}

	if (m_NextMaintenanceDate <= GetDateToday())
		ServerMaintenanceStart();*/

	DEBUG_LOG("Server maintenance check initialized.");
}

// This handles the issued and queued CLI/RA commands
void DBServer::ProcessCliCommands()
{
	std::lock_guard<std::mutex> guard(m_cliCommandQueueLock);

	/*while (!m_cliCommandQueue.empty())
	{
		auto const command = m_cliCommandQueue.front();
		m_cliCommandQueue.pop_front();

		DEBUG_LOG("CLI command under processing...");

		CliHandler handler(command->m_cliAccountId, command->m_cliAccessLevel, command->m_print);
		handler.ParseCommands(&command->m_command[0]);

		if (command->m_commandFinished)
			command->m_commandFinished(!handler.HasSentErrorMessage());

		delete command;
	}*/
}

void DBServer::InitResultQueue()
{
}

void DBServer::UpdateResultQueue()
{
	// process async result queues
	//CharacterDatabase.ProcessResultQueue();
	WorldDatabase.ProcessResultQueue();
	//LoginDatabase.ProcessResultQueue();
}

void DBServer::UpdateRealmCharCount(uint32 accountId)
{
	/*CharacterDatabase.AsyncPQuery(this, &DBServer::_UpdateRealmCharCount, accountId,
		"SELECT COUNT(guid) FROM characters WHERE account = '%u'", accountId);*/
}

void DBServer::_UpdateRealmCharCount(QueryResult* resultCharCount, uint32 accountId)
{
	if (resultCharCount)
	{
		Field* fields = resultCharCount->Fetch();
		uint32 charCount = fields[0].GetUInt32();
		delete resultCharCount;

		//LoginDatabase.BeginTransaction();
		//LoginDatabase.PExecute("DELETE FROM realmcharacters WHERE acctid= '%u' AND realmid = '%u'", accountId, realmID);
		//LoginDatabase.PExecute("INSERT INTO realmcharacters (numchars, acctid, realmid) VALUES (%u, %u, %u)", charCount, accountId, realmID);
		//LoginDatabase.CommitTransaction();
	}
}

void DBServer::SetPlayerLimit(int32 limit, bool needUpdate)
{
	if (limit < -SEC_ADMINISTRATOR)
		limit = -SEC_ADMINISTRATOR;

	// lock update need
	bool db_update_need = needUpdate || (limit < 0) != (m_playerLimit < 0) || (limit < 0 && m_playerLimit < 0 && limit != m_playerLimit);

	m_playerLimit = limit;

	/*if (db_update_need)
		LoginDatabase.PExecute("UPDATE realmlist SET allowedSecurityLevel = '%u' WHERE id = '%u'",
		uint32(GetPlayerSecurityLimit()), realmID);*/
}

//void DBServer::UpdateMaxSessionCounters()
//{
//	m_maxActiveSessionCount = std::max(m_maxActiveSessionCount, uint32(m_sessions.size() - m_QueuedSessions.size()));
//	m_maxQueuedSessionCount = std::max(m_maxQueuedSessionCount, uint32(m_QueuedSessions.size()));
//}

void DBServer::LoadDBVersion()
{
	QueryResult* result = WorldDatabase.Query("SELECT version, creature_ai_version FROM db_version LIMIT 1");
	if (result)
	{
		Field* fields = result->Fetch();

		m_DBVersion = fields[0].GetCppString();
		m_CreatureEventAIVersion = fields[1].GetCppString();

		delete result;
	}

	if (m_DBVersion.empty())
		m_DBVersion = "Unknown DBServer database.";

	if (m_CreatureEventAIVersion.empty())
		m_CreatureEventAIVersion = "Unknown creature EventAI.";
}

void DBServer::AddHeartTime(uint32 diff)
{
	//Ã¿ÃëË¢ÐÂ
	static uint8 index = 0;

	m_heartTime[index++ % 10] = diff;
}
