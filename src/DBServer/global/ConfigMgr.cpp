#include "ConfigMgr.h"
#ifdef USE_LUA_CONFIG
#include "Config/ConfigLua.h"
#else
#include "Config/Config.h"
#endif
#include "Log.h"
#include "SystemConfig.h"
#include "dbserver/DBServer.h"
#include "Config\DefaultDef.h"
#include "Protocol\DBCEnums.h"
#include "Protocol\SharedDefines.h"
#include "Protocol\PlayerDef.h"
//#include "..\LogicServer\common\DBCEnums.h"
//#include "..\LogicServer\common\DefaultDef.h"
//#include "Object.h"
//#include "VMapFactory.h"
//#include "DBCEnums.h"
//#include "MapManager.h"
//#include "ObjectMgr.h"
//#include "MoveMap.h"


float ConfigMgr::m_MaxVisibleDistanceOnContinents = DEFAULT_VISIBILITY_DISTANCE;
float ConfigMgr::m_MaxVisibleDistanceInInstances = DEFAULT_VISIBILITY_INSTANCE;
float ConfigMgr::m_MaxVisibleDistanceInBG = DEFAULT_VISIBILITY_BG;

float ConfigMgr::m_MaxVisibleDistanceInFlight = DEFAULT_VISIBILITY_DISTANCE;
float ConfigMgr::m_VisibleUnitGreyDistance = 0;
float ConfigMgr::m_VisibleObjectGreyDistance = 0;

float  ConfigMgr::m_relocation_lower_limit_sq = 10.f * 10.f;
uint32 ConfigMgr::m_relocation_ai_notify_delay = 1000u;

ConfigMgr::ConfigMgr()
{
	for (int i = 0; i < G_CFG_UINT32_VALUE_COUNT; ++i)
		m_configUint32Values[i] = 0;

	for (int i = 0; i < G_CFG_INT32_VALUE_COUNT; ++i)
		m_configInt32Values[i] = 0;

	for (int i = 0; i < G_CFG_FLOAT_VALUE_COUNT; ++i)
		m_configFloatValues[i] = 0.0f;

	for (int i = 0; i < G_CFG_BOOL_VALUE_COUNT; ++i)
		m_configBoolValues[i] = false;
}


ConfigMgr::~ConfigMgr()
{
}


/// Initialize config values
void ConfigMgr::LoadConfigSettings(bool reload)
{
	if (reload)
	{
		if (!sConfig.Reload())
		{
			sLog.outError("DBServer settings reload fail: can't read settings from %s.", sConfig.GetFilename().c_str());
			return;
		}
	}

	///- Read the version of the configuration file and warn the user in case of emptiness or mismatch
	uint32 confVersion = sConfig.GetIntDefault("ConfVersion", 0);
	if (!confVersion)
	{
		sLog.outError("*****************************************************************************");
		sLog.outError(" WARNING: mangosd.conf does not include a ConfVersion variable.");
		sLog.outError("          Your configuration file may be out of date!");
		sLog.outError("*****************************************************************************");
		Log::WaitBeforeContinueIfNeed();
	}
	else
	{
		if (confVersion < _MANGOSDCONFVERSION)
		{
			sLog.outError("*****************************************************************************");
			sLog.outError(" WARNING: Your mangosd.conf version indicates your conf file is out of date!");
			sLog.outError("          Please check for updates, as your current default values may cause");
			sLog.outError("          unexpected behavior.");
			sLog.outError("*****************************************************************************");
			Log::WaitBeforeContinueIfNeed();
		}
	}

	//Read server name from the config file
	m_sServerName = sConfig.GetStringDefault("ServerName", "anonymity");

	///- Read the player limit and the Message of the day from the config file
	sDBServer.SetPlayerLimit(sConfig.GetIntDefault("PlayerLimit", DEFAULT_PLAYER_LIMIT), true);
	sDBServer.SetMotd(sConfig.GetStringDefault("Motd", "Welcome to the Massive Network Game Object Server."));

	///- Read all rates from the config file
	setConfigPos(G_CFG_FLOAT_RATE_HEALTH, "Rate.Health", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_POWER_MANA, "Rate.Mana", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_POWER_RAGE_INCOME, "Rate.Rage.Income", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_POWER_RAGE_LOSS, "Rate.Rage.Loss", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_POWER_FOCUS, "Rate.Focus", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_LOYALTY, "Rate.Loyalty", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_POWER_ENERGY, "Rate.Energy", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_SKILL_DISCOVERY, "Rate.Skill.Discovery", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_POOR, "Rate.Drop.Item.Poor", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_NORMAL, "Rate.Drop.Item.Normal", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_UNCOMMON, "Rate.Drop.Item.Uncommon", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_RARE, "Rate.Drop.Item.Rare", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_EPIC, "Rate.Drop.Item.Epic", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_LEGENDARY, "Rate.Drop.Item.Legendary", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_ARTIFACT, "Rate.Drop.Item.Artifact", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_REFERENCED, "Rate.Drop.Item.Referenced", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_ITEM_QUEST, "Rate.Drop.Item.Quest", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DROP_MONEY, "Rate.Drop.Money", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_PET_XP_KILL, "Rate.Pet.XP.Kill", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_XP_KILL, "Rate.XP.Kill", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_XP_QUEST, "Rate.XP.Quest", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_XP_EXPLORE, "Rate.XP.Explore", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_REPUTATION_GAIN, "Rate.Reputation.Gain", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_REPUTATION_LOWLEVEL_KILL, "Rate.Reputation.LowLevel.Kill", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_REPUTATION_LOWLEVEL_QUEST, "Rate.Reputation.LowLevel.Quest", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_NORMAL_DAMAGE, "Rate.Creature.Normal.Damage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_ELITE_DAMAGE, "Rate.Creature.Elite.Elite.Damage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_RAREELITE_DAMAGE, "Rate.Creature.Elite.RAREELITE.Damage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_WORLDBOSS_DAMAGE, "Rate.Creature.Elite.WORLDBOSS.Damage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_RARE_DAMAGE, "Rate.Creature.Elite.RARE.Damage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_NORMAL_HP, "Rate.Creature.Normal.HP", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_ELITE_HP, "Rate.Creature.Elite.Elite.HP", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_RAREELITE_HP, "Rate.Creature.Elite.RAREELITE.HP", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_WORLDBOSS_HP, "Rate.Creature.Elite.WORLDBOSS.HP", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_RARE_HP, "Rate.Creature.Elite.RARE.HP", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_NORMAL_SPELLDAMAGE, "Rate.Creature.Normal.SpellDamage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_ELITE_SPELLDAMAGE, "Rate.Creature.Elite.Elite.SpellDamage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_RAREELITE_SPELLDAMAGE, "Rate.Creature.Elite.RAREELITE.SpellDamage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_WORLDBOSS_SPELLDAMAGE, "Rate.Creature.Elite.WORLDBOSS.SpellDamage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_ELITE_RARE_SPELLDAMAGE, "Rate.Creature.Elite.RARE.SpellDamage", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CREATURE_AGGRO, "Rate.Creature.Aggro", 1.0f);
	setConfig(G_CFG_FLOAT_RATE_REST_INGAME, "Rate.Rest.InGame", 1.0f);
	setConfig(G_CFG_FLOAT_RATE_REST_OFFLINE_IN_TAVERN_OR_CITY, "Rate.Rest.Offline.InTavernOrCity", 1.0f);
	setConfig(G_CFG_FLOAT_RATE_REST_OFFLINE_IN_WILDERNESS, "Rate.Rest.Offline.InWilderness", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_DAMAGE_FALL, "Rate.Damage.Fall", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_AUCTION_TIME, "Rate.Auction.Time", 1.0f);
	setConfig(G_CFG_FLOAT_RATE_AUCTION_DEPOSIT, "Rate.Auction.Deposit", 1.0f);
	setConfig(G_CFG_FLOAT_RATE_AUCTION_CUT, "Rate.Auction.Cut", 1.0f);
	setConfig(G_CFG_UINT32_AUCTION_DEPOSIT_MIN, "Auction.Deposit.Min", 0);
	setConfig(G_CFG_FLOAT_RATE_HONOR, "Rate.Honor", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_MINING_AMOUNT, "Rate.Mining.Amount", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_MINING_NEXT, "Rate.Mining.Next", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_INSTANCE_RESET_TIME, "Rate.InstanceResetTime", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_TALENT, "Rate.Talent", 1.0f);
	setConfigPos(G_CFG_FLOAT_RATE_CORPSE_DECAY_LOOTED, "Rate.Corpse.Decay.Looted", 0.0f);

	setConfigMinMax(G_CFG_FLOAT_RATE_TARGET_POS_RECALCULATION_RANGE, "TargetPosRecalculateRange", 1.5f, CONTACT_DISTANCE, ATTACK_DISTANCE);

	setConfigPos(G_CFG_FLOAT_RATE_DURABILITY_LOSS_DAMAGE, "DurabilityLossChance.Damage", 0.5f);
	setConfigPos(G_CFG_FLOAT_RATE_DURABILITY_LOSS_ABSORB, "DurabilityLossChance.Absorb", 0.5f);
	setConfigPos(G_CFG_FLOAT_RATE_DURABILITY_LOSS_PARRY, "DurabilityLossChance.Parry", 0.05f);
	setConfigPos(G_CFG_FLOAT_RATE_DURABILITY_LOSS_BLOCK, "DurabilityLossChance.Block", 0.05f);

	setConfigPos(G_CFG_FLOAT_LISTEN_RANGE_SAY, "ListenRange.Say", 40.0f);
	setConfigPos(G_CFG_FLOAT_LISTEN_RANGE_YELL, "ListenRange.Yell", 300.0f);
	setConfigPos(G_CFG_FLOAT_LISTEN_RANGE_TEXTEMOTE, "ListenRange.TextEmote", 40.0f);

	setConfigPos(G_CFG_FLOAT_GROUP_XP_DISTANCE, "MaxGroupXPDistance", 74.0f);
	setConfigPos(G_CFG_FLOAT_SIGHT_GUARDER, "GuarderSight", 50.0f);
	setConfigPos(G_CFG_FLOAT_SIGHT_MONSTER, "MonsterSight", 50.0f);

	setConfigPos(G_CFG_FLOAT_CREATURE_FAMILY_ASSISTANCE_RADIUS, "CreatureFamilyAssistanceRadius", 10.0f);
	setConfigPos(G_CFG_FLOAT_CREATURE_FAMILY_FLEE_ASSISTANCE_RADIUS, "CreatureFamilyFleeAssistanceRadius", 30.0f);

	///- Read other configuration items from the config file
	setConfigMinMax(G_CFG_UINT32_COMPRESSION, "Compression", 1, 1, 9);
	setConfig(G_CFG_BOOL_ADDON_CHANNEL, "AddonChannel", true);
	setConfig(G_CFG_BOOL_CLEAN_CHARACTER_DB, "CleanCharacterDB", true);
	setConfig(G_CFG_BOOL_GRID_UNLOAD, "GridUnload", true);
	setConfig(G_CFG_UINT32_MAX_WHOLIST_RETURNS, "MaxWhoListReturns", 49);

	/*std::string forceLoadGridOnMaps = sConfig.GetStringDefault("LoadAllGridsOnMaps");
	if (!forceLoadGridOnMaps.empty())
	{
		unsigned int pos = 0;
		unsigned int id;
		VMAP::VMapFactory::chompAndTrim(forceLoadGridOnMaps);
		while (VMAP::VMapFactory::getNextId(forceLoadGridOnMaps, pos, id))
			m_configForceLoadMapIds.insert(id);
	}*/

	setConfig(G_CFG_UINT32_INTERVAL_SAVE, "PlayerSave.Interval", 15 * MINUTE * IN_MILLISECONDS);
	setConfigMinMax(G_CFG_UINT32_MIN_LEVEL_STAT_SAVE, "PlayerSave.Stats.MinLevel", 0, 0, MAX_LEVEL);
	setConfig(G_CFG_BOOL_STATS_SAVE_ONLY_ON_LOGOUT, "PlayerSave.Stats.SaveOnlyOnLogout", true);

	/*setConfigMin(G_CFG_UINT32_INTERVAL_GRIDCLEAN, "GridCleanUpDelay", 5 * MINUTE * IN_MILLISECONDS, MIN_GRID_DELAY);
	if (reload)
		sMapMgr.SetGridCleanUpDelay(getConfig(G_CFG_UINT32_INTERVAL_GRIDCLEAN));

	setConfigMin(G_CFG_UINT32_INTERVAL_MAPUPDATE, "MapUpdateInterval", 100, MIN_MAP_UPDATE_DELAY);
	if (reload)
		sMapMgr.SetMapUpdateInterval(getConfig(G_CFG_UINT32_INTERVAL_MAPUPDATE));*/

	setConfig(G_CFG_UINT32_INTERVAL_CHANGEWEATHER, "ChangeWeatherInterval", 10 * MINUTE * IN_MILLISECONDS);

	if (configNoReload(reload, G_CFG_UINT32_PORT_GATEWAY, "DBServerServerPort", DEFAULT_GATEWAYSERVER_PORT))
		setConfig(G_CFG_UINT32_PORT_GATEWAY, "DBServerServerPort", DEFAULT_GATEWAYSERVER_PORT);

	if (configNoReload(reload, G_CFG_UINT32_PORT_LOGIC, "LogicServerPort", DEFAULT_WORLDSERVER_PORT))
		setConfig(G_CFG_UINT32_PORT_LOGIC, "LogicServerPort", DEFAULT_WORLDSERVER_PORT);

	if (configNoReload(reload, G_CFG_UINT32_NODE_TYPE, "NodeType", 0))
		setConfig(G_CFG_UINT32_NODE_TYPE, "NodeType", 0);

	if (configNoReload(reload, G_CFG_UINT32_NODE_INDEX, "NodeIndex", 0))
		setConfig(G_CFG_UINT32_NODE_INDEX, "NodeIndex", 0);

	////logic server for gateway register
	//if (configNoReload(reload, G_CFG_UINT32_LOGICNODE_TYPE, "LogicNodeType", 0))
	//	setConfig(G_CFG_UINT32_LOGICNODE_TYPE, "NodeType", 0);

	//if (configNoReload(reload, G_CFG_UINT32_LOGICNODE_INDEX, "LogicNodeIndex", 0))
	//	setConfig(G_CFG_UINT32_LOGICNODE_INDEX, "NodeIndex", 0);

	if (configNoReload(reload, G_CFG_UINT32_GAME_TYPE, "GameType", 0))
		setConfig(G_CFG_UINT32_GAME_TYPE, "GameType", 0);

	if (configNoReload(reload, G_CFG_UINT32_REALM_ZONE, "RealmZone", REALM_ZONE_DEVELOPMENT))
		setConfig(G_CFG_UINT32_REALM_ZONE, "RealmZone", REALM_ZONE_DEVELOPMENT);

	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_ACCOUNTS, "AllowTwoSide.Accounts", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_INTERACTION_CHAT, "AllowTwoSide.Interaction.Chat", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_INTERACTION_CHANNEL, "AllowTwoSide.Interaction.Channel", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_INTERACTION_GROUP, "AllowTwoSide.Interaction.Group", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_INTERACTION_GUILD, "AllowTwoSide.Interaction.Guild", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_INTERACTION_TRADE, "AllowTwoSide.Interaction.Trade", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_INTERACTION_AUCTION, "AllowTwoSide.Interaction.Auction", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_INTERACTION_MAIL, "AllowTwoSide.Interaction.Mail", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_WHO_LIST, "AllowTwoSide.WhoList", false);
	setConfig(G_CFG_BOOL_ALLOW_TWO_SIDE_ADD_FRIEND, "AllowTwoSide.AddFriend", false);

	setConfig(G_CFG_UINT32_STRICT_PLAYER_NAMES, "StrictPlayerNames", 0);
	setConfig(G_CFG_UINT32_STRICT_CHARTER_NAMES, "StrictCharterNames", 0);
	setConfig(G_CFG_UINT32_STRICT_PET_NAMES, "StrictPetNames", 0);

	setConfigMinMax(G_CFG_UINT32_MIN_PLAYER_NAME, "MinPlayerName", 2, 1, MAX_PLAYER_NAME);
	setConfigMinMax(G_CFG_UINT32_MIN_CHARTER_NAME, "MinCharterName", 2, 1, MAX_CHARTER_NAME);
	setConfigMinMax(G_CFG_UINT32_MIN_PET_NAME, "MinPetName", 2, 1, MAX_PET_NAME);

	setConfig(G_CFG_UINT32_CHARACTERS_CREATING_DISABLED, "CharactersCreatingDisabled", 0);

	setConfigMinMax(G_CFG_UINT32_CHARACTERS_PER_REALM, "CharactersPerRealm", 10, 1, 10);

	// must be after G_CFG_UINT32_CHARACTERS_PER_REALM
	setConfigMin(G_CFG_UINT32_CHARACTERS_PER_ACCOUNT, "CharactersPerAccount", 50, getConfig(G_CFG_UINT32_CHARACTERS_PER_REALM));

	setConfigMinMax(G_CFG_UINT32_SKIP_CINEMATICS, "SkipCinematics", 0, 0, 2);

	if (configNoReload(reload, G_CFG_UINT32_MAX_PLAYER_LEVEL, "MaxPlayerLevel", DEFAULT_MAX_LEVEL))
		setConfigMinMax(G_CFG_UINT32_MAX_PLAYER_LEVEL, "MaxPlayerLevel", DEFAULT_MAX_LEVEL, 1, DEFAULT_MAX_LEVEL);

	setConfigMinMax(G_CFG_UINT32_START_PLAYER_LEVEL, "StartPlayerLevel", 1, 1, getConfig(G_CFG_UINT32_MAX_PLAYER_LEVEL));

	setConfigMinMax(G_CFG_UINT32_START_PLAYER_MONEY, "StartPlayerMoney", 0, 0, MAX_MONEY_AMOUNT);

	setConfig(G_CFG_UINT32_MAX_HONOR_POINTS, "MaxHonorPoints", 75000);

	setConfigMinMax(G_CFG_UINT32_START_HONOR_POINTS, "StartHonorPoints", 0, 0, getConfig(G_CFG_UINT32_MAX_HONOR_POINTS));

	setConfigMin(G_CFG_UINT32_MIN_HONOR_KILLS, "MinHonorKills", HONOR_STANDING_MIN_KILL, 1);

	setConfigMinMax(G_CFG_UINT32_MAINTENANCE_DAY, "MaintenanceDay", 4, 0, 6);

	setConfig(G_CFG_BOOL_ALL_TAXI_PATHS, "AllFlightPaths", false);

	setConfig(G_CFG_BOOL_INSTANCE_IGNORE_LEVEL, "Instance.IgnoreLevel", false);
	setConfig(G_CFG_BOOL_INSTANCE_IGNORE_RAID, "Instance.IgnoreRaid", false);

	setConfig(G_CFG_BOOL_CAST_UNSTUCK, "CastUnstuck", true);
	setConfig(G_CFG_UINT32_MAX_SPELL_CASTS_IN_CHAIN, "MaxSpellCastsInChain", 20);
	setConfig(G_CFG_UINT32_RABBIT_DAY, "RabbitDay", 0);

	setConfig(G_CFG_UINT32_INSTANCE_RESET_TIME_HOUR, "Instance.ResetTimeHour", 4);
	setConfig(G_CFG_UINT32_INSTANCE_UNLOAD_DELAY, "Instance.UnloadDelay", 30 * MINUTE * IN_MILLISECONDS);

	setConfigMinMax(G_CFG_UINT32_MAX_PRIMARY_TRADE_SKILL, "MaxPrimaryTradeSkill", 2, 0, 10);

	setConfigMinMax(G_CFG_UINT32_TRADE_SKILL_GMIGNORE_MAX_PRIMARY_COUNT, "TradeSkill.GMIgnore.MaxPrimarySkillsCount", SEC_CONSOLE, SEC_PLAYER, SEC_CONSOLE);
	setConfigMinMax(G_CFG_UINT32_TRADE_SKILL_GMIGNORE_LEVEL, "TradeSkill.GMIgnore.Level", SEC_CONSOLE, SEC_PLAYER, SEC_CONSOLE);
	setConfigMinMax(G_CFG_UINT32_TRADE_SKILL_GMIGNORE_SKILL, "TradeSkill.GMIgnore.Skill", SEC_CONSOLE, SEC_PLAYER, SEC_CONSOLE);

	setConfigMinMax(G_CFG_UINT32_MIN_PETITION_SIGNS, "MinPetitionSigns", 9, 0, 9);

	setConfig(G_CFG_UINT32_GM_LOGIN_STATE, "GM.LoginState", 2);
	setConfig(G_CFG_UINT32_GM_VISIBLE_STATE, "GM.Visible", 2);
	setConfig(G_CFG_UINT32_GM_ACCEPT_TICKETS, "GM.AcceptTickets", 2);
	setConfig(G_CFG_UINT32_GM_CHAT, "GM.Chat", 2);
	setConfig(G_CFG_UINT32_GM_WISPERING_TO, "GM.WhisperingTo", 2);

	setConfig(G_CFG_UINT32_GM_LEVEL_IN_GM_LIST, "GM.InGMList.Level", SEC_ADMINISTRATOR);
	setConfig(G_CFG_UINT32_GM_LEVEL_IN_WHO_LIST, "GM.InWhoList.Level", SEC_ADMINISTRATOR);
	setConfig(G_CFG_BOOL_GM_LOG_TRADE, "GM.LogTrade", false);

	setConfigMinMax(G_CFG_UINT32_START_GM_LEVEL, "GM.StartLevel", 1, getConfig(G_CFG_UINT32_START_PLAYER_LEVEL), MAX_LEVEL);
	setConfig(G_CFG_BOOL_GM_LOWER_SECURITY, "GM.LowerSecurity", false);
	setConfig(G_CFG_UINT32_GM_INVISIBLE_AURA, "GM.InvisibleAura", 31748);

	setConfig(G_CFG_UINT32_GROUP_VISIBILITY, "Visibility.GroupMode", 0);

	setConfig(G_CFG_UINT32_MAIL_DELIVERY_DELAY, "MailDeliveryDelay", HOUR);

	setConfigMin(G_CFG_UINT32_MASS_MAILER_SEND_PER_TICK, "MassMailer.SendPerTick", 10, 1);

	setConfig(G_CFG_UINT32_UPTIME_UPDATE, "UpdateUptimeInterval", 10);
	if (reload)
	{
		m_timers[GUPDATE_UPTIME].SetInterval(getConfig(G_CFG_UINT32_UPTIME_UPDATE)*MINUTE * IN_MILLISECONDS);
		m_timers[GUPDATE_UPTIME].Reset();
	}

	setConfig(G_CFG_UINT32_SKILL_CHANCE_ORANGE, "SkillChance.Orange", 100);
	setConfig(G_CFG_UINT32_SKILL_CHANCE_YELLOW, "SkillChance.Yellow", 75);
	setConfig(G_CFG_UINT32_SKILL_CHANCE_GREEN, "SkillChance.Green", 25);
	setConfig(G_CFG_UINT32_SKILL_CHANCE_GREY, "SkillChance.Grey", 0);

	setConfig(G_CFG_UINT32_SKILL_CHANCE_MINING_STEPS, "SkillChance.MiningSteps", 75);
	setConfig(G_CFG_UINT32_SKILL_CHANCE_SKINNING_STEPS, "SkillChance.SkinningSteps", 75);

	setConfig(G_CFG_UINT32_SKILL_GAIN_CRAFTING, "SkillGain.Crafting", 1);
	setConfig(G_CFG_UINT32_SKILL_GAIN_DEFENSE, "SkillGain.Defense", 1);
	setConfig(G_CFG_UINT32_SKILL_GAIN_GATHERING, "SkillGain.Gathering", 1);
	setConfig(G_CFG_UINT32_SKILL_GAIN_WEAPON, "SkillGain.Weapon", 1);

	setConfig(G_CFG_BOOL_SKILL_FAIL_LOOT_FISHING, "SkillFail.Loot.Fishing", false);
	setConfig(G_CFG_BOOL_SKILL_FAIL_GAIN_FISHING, "SkillFail.Gain.Fishing", false);
	setConfig(G_CFG_BOOL_SKILL_FAIL_POSSIBLE_FISHINGPOOL, "SkillFail.Possible.FishingPool", true);

	setConfig(G_CFG_UINT32_MAX_OVERSPEED_PINGS, "MaxOverspeedPings", 2);
	if (getConfig(G_CFG_UINT32_MAX_OVERSPEED_PINGS) != 0 && getConfig(G_CFG_UINT32_MAX_OVERSPEED_PINGS) < 2)
	{
		sLog.outError("MaxOverspeedPings (%i) must be in range 2..infinity (or 0 to disable check). Set to 2.", getConfig(G_CFG_UINT32_MAX_OVERSPEED_PINGS));
		setConfig(G_CFG_UINT32_MAX_OVERSPEED_PINGS, 2);
	}

	setConfig(G_CFG_BOOL_SAVE_RESPAWN_TIME_IMMEDIATELY, "SaveRespawnTimeImmediately", true);
	setConfig(G_CFG_BOOL_WEATHER, "ActivateWeather", true);

	setConfig(G_CFG_BOOL_ALWAYS_MAX_SKILL_FOR_LEVEL, "AlwaysMaxSkillForLevel", false);

	setConfig(G_CFG_UINT32_CHATFLOOD_MESSAGE_COUNT, "ChatFlood.MessageCount", 10);
	setConfig(G_CFG_UINT32_CHATFLOOD_MESSAGE_DELAY, "ChatFlood.MessageDelay", 1);
	setConfig(G_CFG_UINT32_CHATFLOOD_MUTE_TIME, "ChatFlood.MuteTime", 10);

	setConfig(G_CFG_BOOL_EVENT_ANNOUNCE, "Event.Announce", false);

	setConfig(G_CFG_UINT32_CREATURE_FAMILY_ASSISTANCE_DELAY, "CreatureFamilyAssistanceDelay", 1500);
	setConfig(G_CFG_UINT32_CREATURE_FAMILY_FLEE_DELAY, "CreatureFamilyFleeDelay", 7000);

	setConfig(G_CFG_UINT32_WORLD_BOSS_LEVEL_DIFF, "WorldBossLevelDiff", 3);

	setConfigMinMax(G_CFG_INT32_QUEST_LOW_LEVEL_HIDE_DIFF, "Quests.LowLevelHideDiff", 4, -1, MAX_LEVEL);
	setConfigMinMax(G_CFG_INT32_QUEST_HIGH_LEVEL_HIDE_DIFF, "Quests.HighLevelHideDiff", 7, -1, MAX_LEVEL);

	setConfig(G_CFG_BOOL_QUEST_IGNORE_RAID, "Quests.IgnoreRaid", false);

	setConfig(G_CFG_BOOL_DETECT_POS_COLLISION, "DetectPosCollision", true);

	setConfig(G_CFG_BOOL_RESTRICTED_LFG_CHANNEL, "Channel.RestrictedLfg", true);
	setConfig(G_CFG_BOOL_SILENTLY_GM_JOIN_TO_CHANNEL, "Channel.SilentlyGMJoin", false);

	setConfig(G_CFG_BOOL_CHAT_FAKE_MESSAGE_PREVENTING, "ChatFakeMessagePreventing", false);

	setConfig(G_CFG_UINT32_CHAT_STRICT_LINK_CHECKING_SEVERITY, "ChatStrictLinkChecking.Severity", 0);
	setConfig(G_CFG_UINT32_CHAT_STRICT_LINK_CHECKING_KICK, "ChatStrictLinkChecking.Kick", 0);

	setConfig(G_CFG_BOOL_CORPSE_EMPTY_LOOT_SHOW, "Corpse.EmptyLootShow", true);
	setConfig(G_CFG_UINT32_CORPSE_DECAY_NORMAL, "Corpse.Decay.NORMAL", 300);
	setConfig(G_CFG_UINT32_CORPSE_DECAY_RARE, "Corpse.Decay.RARE", 900);
	setConfig(G_CFG_UINT32_CORPSE_DECAY_ELITE, "Corpse.Decay.ELITE", 600);
	setConfig(G_CFG_UINT32_CORPSE_DECAY_RAREELITE, "Corpse.Decay.RAREELITE", 1200);
	setConfig(G_CFG_UINT32_CORPSE_DECAY_WORLDBOSS, "Corpse.Decay.WORLDBOSS", 3600);

	setConfig(G_CFG_INT32_DEATH_SICKNESS_LEVEL, "Death.SicknessLevel", 11);

	setConfig(G_CFG_BOOL_DEATH_CORPSE_RECLAIM_DELAY_PVP, "Death.CorpseReclaimDelay.PvP", true);
	setConfig(G_CFG_BOOL_DEATH_CORPSE_RECLAIM_DELAY_PVE, "Death.CorpseReclaimDelay.PvE", true);
	setConfig(G_CFG_BOOL_DEATH_BONES_WORLD, "Death.Bones.DBServer", true);
	setConfig(G_CFG_BOOL_DEATH_BONES_BG, "Death.Bones.Battleground", true);
	setConfigMinMax(G_CFG_FLOAT_GHOST_RUN_SPEED_WORLD, "Death.Ghost.RunSpeed.DBServer", 1.0f, 0.1f, 10.0f);
	setConfigMinMax(G_CFG_FLOAT_GHOST_RUN_SPEED_BG, "Death.Ghost.RunSpeed.Battleground", 1.0f, 0.1f, 10.0f);

	setConfig(G_CFG_FLOAT_THREAT_RADIUS, "ThreatRadius", 100.0f);
	setConfigMin(G_CFG_UINT32_CREATURE_RESPAWN_AGGRO_DELAY, "CreatureRespawnAggroDelay", 5000, 0);

	setConfig(G_CFG_BOOL_BATTLEGROUND_CAST_DESERTER, "Battleground.CastDeserter", true);
	setConfigMinMax(G_CFG_UINT32_BATTLEGROUND_QUEUE_ANNOUNCER_JOIN, "Battleground.QueueAnnouncer.Join", 0, 0, 2);
	setConfig(G_CFG_BOOL_BATTLEGROUND_QUEUE_ANNOUNCER_START, "Battleground.QueueAnnouncer.Start", false);
	setConfig(G_CFG_BOOL_BATTLEGROUND_SCORE_STATISTICS, "Battleground.ScoreStatistics", false);
	setConfig(G_CFG_UINT32_BATTLEGROUND_INVITATION_TYPE, "Battleground.InvitationType", 0);
	setConfig(G_CFG_UINT32_BATTLEGROUND_PREMATURE_FINISH_TIMER, "BattleGround.PrematureFinishTimer", 5 * MINUTE * IN_MILLISECONDS);
	setConfig(G_CFG_UINT32_BATTLEGROUND_PREMADE_GROUP_WAIT_FOR_MATCH, "BattleGround.PremadeGroupWaitForMatch", 0);
	setConfig(G_CFG_BOOL_OUTDOORPVP_SI_ENABLED, "OutdoorPvp.SIEnabled", true);
	setConfig(G_CFG_BOOL_OUTDOORPVP_EP_ENABLED, "OutdoorPvp.EPEnabled", true);

	setConfig(G_CFG_BOOL_KICK_PLAYER_ON_BAD_PACKET, "Network.KickOnBadPacket", false);

	setConfig(G_CFG_BOOL_PLAYER_COMMANDS, "PlayerCommands", true);

	setConfig(G_CFG_UINT32_INSTANT_LOGOUT, "InstantLogout", SEC_MODERATOR);

	setConfigMin(G_CFG_UINT32_GUILD_EVENT_LOG_COUNT, "Guild.EventLogRecordsCount", GUILD_EVENTLOG_MAX_RECORDS, GUILD_EVENTLOG_MAX_RECORDS);

	setConfig(G_CFG_UINT32_TIMERBAR_FATIGUE_GMLEVEL, "TimerBar.Fatigue.GMLevel", SEC_CONSOLE);
	setConfig(G_CFG_UINT32_TIMERBAR_FATIGUE_MAX, "TimerBar.Fatigue.Max", 60);
	setConfig(G_CFG_UINT32_TIMERBAR_BREATH_GMLEVEL, "TimerBar.Breath.GMLevel", SEC_CONSOLE);
	setConfig(G_CFG_UINT32_TIMERBAR_BREATH_MAX, "TimerBar.Breath.Max", 60);
	setConfig(G_CFG_UINT32_TIMERBAR_FIRE_GMLEVEL, "TimerBar.Fire.GMLevel", SEC_CONSOLE);
	setConfig(G_CFG_UINT32_TIMERBAR_FIRE_MAX, "TimerBar.Fire.Max", 1);

	setConfig(G_CFG_BOOL_PET_UNSUMMON_AT_MOUNT, "PetUnsummonAtMount", false);

	m_relocation_ai_notify_delay = sConfig.GetIntDefault("Visibility.AIRelocationNotifyDelay", 1000u);
	m_relocation_lower_limit_sq = pow(sConfig.GetFloatDefault("Visibility.RelocationLowerLimit", 10), 2);

	m_VisibleUnitGreyDistance = sConfig.GetFloatDefault("Visibility.Distance.Grey.Unit", 1);
	if (m_VisibleUnitGreyDistance > MAX_VISIBILITY_DISTANCE)
	{
		sLog.outError("Visibility.Distance.Grey.Unit can't be greater %f", MAX_VISIBILITY_DISTANCE);
		m_VisibleUnitGreyDistance = MAX_VISIBILITY_DISTANCE;
	}
	m_VisibleObjectGreyDistance = sConfig.GetFloatDefault("Visibility.Distance.Grey.Object", 10);
	if (m_VisibleObjectGreyDistance > MAX_VISIBILITY_DISTANCE)
	{
		sLog.outError("Visibility.Distance.Grey.Object can't be greater %f", MAX_VISIBILITY_DISTANCE);
		m_VisibleObjectGreyDistance = MAX_VISIBILITY_DISTANCE;
	}

	// visibility on continents
	m_MaxVisibleDistanceOnContinents = sConfig.GetFloatDefault("Visibility.Distance.Continents", DEFAULT_VISIBILITY_DISTANCE);
	if (m_MaxVisibleDistanceOnContinents < 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO))
	{
		sLog.outError("Visibility.Distance.Continents can't be less max aggro radius %f", 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO));
		m_MaxVisibleDistanceOnContinents = 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO);
	}
	else if (m_MaxVisibleDistanceOnContinents + m_VisibleUnitGreyDistance >  MAX_VISIBILITY_DISTANCE)
	{
		sLog.outError("Visibility.Distance.Continents can't be greater %f", MAX_VISIBILITY_DISTANCE - m_VisibleUnitGreyDistance);
		m_MaxVisibleDistanceOnContinents = MAX_VISIBILITY_DISTANCE - m_VisibleUnitGreyDistance;
	}

	// visibility in instances
	m_MaxVisibleDistanceInInstances = sConfig.GetFloatDefault("Visibility.Distance.Instances", DEFAULT_VISIBILITY_INSTANCE);
	if (m_MaxVisibleDistanceInInstances < 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO))
	{
		sLog.outError("Visibility.Distance.Instances can't be less max aggro radius %f", 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO));
		m_MaxVisibleDistanceInInstances = 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO);
	}
	else if (m_MaxVisibleDistanceInInstances + m_VisibleUnitGreyDistance >  MAX_VISIBILITY_DISTANCE)
	{
		sLog.outError("Visibility.Distance.Instances can't be greater %f", MAX_VISIBILITY_DISTANCE - m_VisibleUnitGreyDistance);
		m_MaxVisibleDistanceInInstances = MAX_VISIBILITY_DISTANCE - m_VisibleUnitGreyDistance;
	}

	// visibility in BG
	m_MaxVisibleDistanceInBG = sConfig.GetFloatDefault("Visibility.Distance.BG", DEFAULT_VISIBILITY_BG);
	if (m_MaxVisibleDistanceInBG < 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO))
	{
		sLog.outError("Visibility.Distance.BG can't be less max aggro radius %f", 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO));
		m_MaxVisibleDistanceInBG = 45 * getConfig(G_CFG_FLOAT_RATE_CREATURE_AGGRO);
	}
	else if (m_MaxVisibleDistanceInBG + m_VisibleUnitGreyDistance >  MAX_VISIBILITY_DISTANCE)
	{
		sLog.outError("Visibility.Distance.BG can't be greater %f", MAX_VISIBILITY_DISTANCE - m_VisibleUnitGreyDistance);
		m_MaxVisibleDistanceInBG = MAX_VISIBILITY_DISTANCE - m_VisibleUnitGreyDistance;
	}

	m_MaxVisibleDistanceInFlight = sConfig.GetFloatDefault("Visibility.Distance.InFlight", DEFAULT_VISIBILITY_DISTANCE);
	if (m_MaxVisibleDistanceInFlight + m_VisibleObjectGreyDistance > MAX_VISIBILITY_DISTANCE)
	{
		sLog.outError("Visibility.Distance.InFlight can't be greater %f", MAX_VISIBILITY_DISTANCE - m_VisibleObjectGreyDistance);
		m_MaxVisibleDistanceInFlight = MAX_VISIBILITY_DISTANCE - m_VisibleObjectGreyDistance;
	}

	///- Load the CharDelete related config options
	setConfigMinMax(G_CFG_UINT32_CHARDELETE_METHOD, "CharDelete.Method", 0, 0, 1);
	setConfigMinMax(G_CFG_UINT32_CHARDELETE_MIN_LEVEL, "CharDelete.MinLevel", 0, 0, getConfig(G_CFG_UINT32_MAX_PLAYER_LEVEL));
	setConfig(G_CFG_UINT32_CHARDELETE_KEEP_DAYS, "CharDelete.KeepDays", 30);

	if (configNoReload(reload, G_CFG_UINT32_GUID_RESERVE_SIZE_CREATURE, "GuidReserveSize.Creature", 100))
		setConfig(G_CFG_UINT32_GUID_RESERVE_SIZE_CREATURE, "GuidReserveSize.Creature", 100);
	if (configNoReload(reload, G_CFG_UINT32_GUID_RESERVE_SIZE_GAMEOBJECT, "GuidReserveSize.GameObject", 100))
		setConfig(G_CFG_UINT32_GUID_RESERVE_SIZE_GAMEOBJECT, "GuidReserveSize.GameObject", 100);

	///- Read the "Data" directory from the config file
	std::string dataPath = sConfig.GetStringDefault("DataDir", "./");

	// for empty string use current dir as for absent case
	if (dataPath.empty())
		dataPath = "./";
	// normalize dir path to path/ or path\ form
	else if (dataPath.at(dataPath.length() - 1) != '/' && dataPath.at(dataPath.length() - 1) != '\\')
		dataPath.append("/");

	if (reload)
	{
		if (dataPath != m_dataPath)
			sLog.outError("DataDir option can't be changed at mangosd.conf reload, using current value (%s).", m_dataPath.c_str());
	}
	else
	{
		m_dataPath = dataPath;
		sLog.outString("Using DataDir %s", m_dataPath.c_str());
	}

	setConfig(G_CFG_BOOL_VMAP_INDOOR_CHECK, "vmap.enableIndoorCheck", true);
	bool enableLOS = sConfig.GetBoolDefault("vmap.enableLOS", false);
	bool enableHeight = sConfig.GetBoolDefault("vmap.enableHeight", false);
	std::string ignoreSpellIds = sConfig.GetStringDefault("vmap.ignoreSpellIds");

	if (!enableHeight)
		sLog.outError("VMAP height use disabled! Creatures movements and other things will be in broken state.");

	/*VMAP::VMapFactory::createOrGetVMapManager()->setEnableLineOfSightCalc(enableLOS);
	VMAP::VMapFactory::createOrGetVMapManager()->setEnableHeightCalc(enableHeight);
	VMAP::VMapFactory::preventSpellsFromBeingTestedForLoS(ignoreSpellIds.c_str());
	sLog.outString("WORLD: VMap support included. LineOfSight:%i, getHeight:%i, indoorCheck:%i",
		enableLOS, enableHeight, getConfig(G_CFG_BOOL_VMAP_INDOOR_CHECK) ? 1 : 0);
	sLog.outString("WORLD: VMap data directory is: %svmaps", m_dataPath.c_str());

	setConfig(G_CFG_BOOL_MMAP_ENABLED, "mmap.enabled", true);
	std::string ignoreMapIds = sConfig.GetStringDefault("mmap.ignoreMapIds");
	MMAP::MMapFactory::preventPathfindingOnMaps(ignoreMapIds.c_str());
	sLog.outString("WORLD: MMap pathfinding %sabled", getConfig(G_CFG_BOOL_MMAP_ENABLED) ? "en" : "dis");*/

	sLog.outString();
}

void ConfigMgr::setConfig(gConfigUInt32Values index, char const* fieldname, uint32 defvalue)
{
	setConfig(index, sConfig.GetIntDefault(fieldname, defvalue));
	if (int32(getConfig(index)) < 0)
	{
		sLog.outError("%s (%i) can't be negative. Using %u instead.", fieldname, int32(getConfig(index)), defvalue);
		setConfig(index, defvalue);
	}
}

void ConfigMgr::setConfig(gConfigInt32Values index, char const* fieldname, int32 defvalue)
{
	setConfig(index, sConfig.GetIntDefault(fieldname, defvalue));
}

void ConfigMgr::setConfig(gConfigFloatValues index, char const* fieldname, float defvalue)
{
	setConfig(index, sConfig.GetFloatDefault(fieldname, defvalue));
}

void ConfigMgr::setConfig(gConfigBoolValues index, char const* fieldname, bool defvalue)
{
	setConfig(index, sConfig.GetBoolDefault(fieldname, defvalue));
}

void ConfigMgr::setConfigPos(gConfigFloatValues index, char const* fieldname, float defvalue)
{
	setConfig(index, fieldname, defvalue);
	if (getConfig(index) < 0.0f)
	{
		sLog.outError("%s (%f) can't be negative. Using %f instead.", fieldname, getConfig(index), defvalue);
		setConfig(index, defvalue);
	}
}

void ConfigMgr::setConfigMin(gConfigUInt32Values index, char const* fieldname, uint32 defvalue, uint32 minvalue)
{
	setConfig(index, fieldname, defvalue);
	if (getConfig(index) < minvalue)
	{
		sLog.outError("%s (%u) must be >= %u. Using %u instead.", fieldname, getConfig(index), minvalue, minvalue);
		setConfig(index, minvalue);
	}
}

void ConfigMgr::setConfigMin(gConfigInt32Values index, char const* fieldname, int32 defvalue, int32 minvalue)
{
	setConfig(index, fieldname, defvalue);
	if (getConfig(index) < minvalue)
	{
		sLog.outError("%s (%i) must be >= %i. Using %i instead.", fieldname, getConfig(index), minvalue, minvalue);
		setConfig(index, minvalue);
	}
}

void ConfigMgr::setConfigMin(gConfigFloatValues index, char const* fieldname, float defvalue, float minvalue)
{
	setConfig(index, fieldname, defvalue);
	if (getConfig(index) < minvalue)
	{
		sLog.outError("%s (%f) must be >= %f. Using %f instead.", fieldname, getConfig(index), minvalue, minvalue);
		setConfig(index, minvalue);
	}
}

void ConfigMgr::setConfigMinMax(gConfigUInt32Values index, char const* fieldname, uint32 defvalue, uint32 minvalue, uint32 maxvalue)
{
	setConfig(index, fieldname, defvalue);
	if (getConfig(index) < minvalue)
	{
		sLog.outError("%s (%u) must be in range %u...%u. Using %u instead.", fieldname, getConfig(index), minvalue, maxvalue, minvalue);
		setConfig(index, minvalue);
	}
	else if (getConfig(index) > maxvalue)
	{
		sLog.outError("%s (%u) must be in range %u...%u. Using %u instead.", fieldname, getConfig(index), minvalue, maxvalue, maxvalue);
		setConfig(index, maxvalue);
	}
}

void ConfigMgr::setConfigMinMax(gConfigInt32Values index, char const* fieldname, int32 defvalue, int32 minvalue, int32 maxvalue)
{
	setConfig(index, fieldname, defvalue);
	if (getConfig(index) < minvalue)
	{
		sLog.outError("%s (%i) must be in range %i...%i. Using %i instead.", fieldname, getConfig(index), minvalue, maxvalue, minvalue);
		setConfig(index, minvalue);
	}
	else if (getConfig(index) > maxvalue)
	{
		sLog.outError("%s (%i) must be in range %i...%i. Using %i instead.", fieldname, getConfig(index), minvalue, maxvalue, maxvalue);
		setConfig(index, maxvalue);
	}
}

void ConfigMgr::setConfigMinMax(gConfigFloatValues index, char const* fieldname, float defvalue, float minvalue, float maxvalue)
{
	setConfig(index, fieldname, defvalue);
	if (getConfig(index) < minvalue)
	{
		sLog.outError("%s (%f) must be in range %f...%f. Using %f instead.", fieldname, getConfig(index), minvalue, maxvalue, minvalue);
		setConfig(index, minvalue);
	}
	else if (getConfig(index) > maxvalue)
	{
		sLog.outError("%s (%f) must be in range %f...%f. Using %f instead.", fieldname, getConfig(index), minvalue, maxvalue, maxvalue);
		setConfig(index, maxvalue);
	}
}

bool ConfigMgr::configNoReload(bool reload, gConfigUInt32Values index, char const* fieldname, uint32 defvalue)
{
	if (!reload)
		return true;

	uint32 val = sConfig.GetIntDefault(fieldname, defvalue);
	if (val != getConfig(index))
		sLog.outError("%s option can't be changed at mangosd.conf reload, using current value (%u).", fieldname, getConfig(index));

	return false;
}

bool ConfigMgr::configNoReload(bool reload, gConfigInt32Values index, char const* fieldname, int32 defvalue)
{
	if (!reload)
		return true;

	int32 val = sConfig.GetIntDefault(fieldname, defvalue);
	if (val != getConfig(index))
		sLog.outError("%s option can't be changed at mangosd.conf reload, using current value (%i).", fieldname, getConfig(index));

	return false;
}

bool ConfigMgr::configNoReload(bool reload, gConfigFloatValues index, char const* fieldname, float defvalue)
{
	if (!reload)
		return true;

	float val = sConfig.GetFloatDefault(fieldname, defvalue);
	if (val != getConfig(index))
		sLog.outError("%s option can't be changed at mangosd.conf reload, using current value (%f).", fieldname, getConfig(index));

	return false;
}

bool ConfigMgr::configNoReload(bool reload, gConfigBoolValues index, char const* fieldname, bool defvalue)
{
	if (!reload)
		return true;

	bool val = sConfig.GetBoolDefault(fieldname, defvalue);
	if (val != getConfig(index))
		sLog.outError("%s option can't be changed at mangosd.conf reload, using current value (%s).", fieldname, getConfig(index) ? "'true'" : "'false'");

	return false;
}

IntervalTimer *ConfigMgr::GetTimers(uint32 nIndex)
{
	if (nIndex >= GUPDATE_COUNT)
		return NULL;
	else
		return &(m_timers[nIndex]);
}