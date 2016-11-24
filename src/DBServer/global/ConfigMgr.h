#ifndef __CONFIGMGR_H
#define __CONFIGMGR_H

#include "common\DBServerDef.h"
#include "Platform\Define.h"
#include "Policies\Singleton.h"
#include <set>
#include "Timer.h"

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();

	void LoadConfigSettings(bool reload = false);
	/// Get a server configuration element (see #dConfigFloatValues)
	void setConfig(dConfigFloatValues index, float value) { m_configFloatValues[index] = value; }
	/// Get a server configuration element (see #dConfigFloatValues)
	float getConfig(dConfigFloatValues rate) const { return m_configFloatValues[rate]; }

	/// Set a server configuration element (see #dConfigUInt32Values)
	void setConfig(dConfigUInt32Values index, uint32 value) { m_configUint32Values[index] = value; }
	/// Get a server configuration element (see #dConfigUInt32Values)
	uint32 getConfig(dConfigUInt32Values index) const { return m_configUint32Values[index]; }

	/// Set a server configuration element (see #dConfigInt32Values)
	void setConfig(dConfigInt32Values index, int32 value) { m_configInt32Values[index] = value; }
	/// Get a server configuration element (see #dConfigInt32Values)
	int32 getConfig(dConfigInt32Values index) const { return m_configInt32Values[index]; }

	/// Set a server configuration element (see #dConfigBoolValues)
	void setConfig(dConfigBoolValues index, bool value) { m_configBoolValues[index] = value; }
	/// Get a server configuration element (see #dConfigBoolValues)
	bool getConfig(dConfigBoolValues index) const { return m_configBoolValues[index]; }
	
	/// Get configuration about force-loaded maps
	bool isForceLoadMap(uint32 id) const { return m_configForceLoadMapIds.find(id) != m_configForceLoadMapIds.end(); }

	std::string GetDataPath() const { return m_dataPath; }
	std::string GetServerName() const { return m_sServerName; }

	IntervalTimer *GetTimers(uint32 nIndex);
	// for max speed access
	static float GetMaxVisibleDistanceOnContinents()    { return m_MaxVisibleDistanceOnContinents; }
	static float GetMaxVisibleDistanceInInstances()     { return m_MaxVisibleDistanceInInstances; }
	static float GetMaxVisibleDistanceInBG()            { return m_MaxVisibleDistanceInBG; }

	static float GetMaxVisibleDistanceInFlight()        { return m_MaxVisibleDistanceInFlight; }
	static float GetVisibleUnitGreyDistance()           { return m_VisibleUnitGreyDistance; }
	static float GetVisibleObjectGreyDistance()         { return m_VisibleObjectGreyDistance; }

	static float GetRelocationLowerLimitSq()            { return m_relocation_lower_limit_sq; }
	static uint32 GetRelocationAINotifyDelay()          { return m_relocation_ai_notify_delay; }


private:
	void setConfig(dConfigUInt32Values index, char const* fieldname, uint32 defvalue);
	void setConfig(dConfigInt32Values index, char const* fieldname, int32 defvalue);
	void setConfig(dConfigFloatValues index, char const* fieldname, float defvalue);
	void setConfig(dConfigBoolValues index, char const* fieldname, bool defvalue);
	void setConfigPos(dConfigFloatValues index, char const* fieldname, float defvalue);
	void setConfigMin(dConfigUInt32Values index, char const* fieldname, uint32 defvalue, uint32 minvalue);
	void setConfigMin(dConfigInt32Values index, char const* fieldname, int32 defvalue, int32 minvalue);
	void setConfigMin(dConfigFloatValues index, char const* fieldname, float defvalue, float minvalue);
	void setConfigMinMax(dConfigUInt32Values index, char const* fieldname, uint32 defvalue, uint32 minvalue, uint32 maxvalue);
	void setConfigMinMax(dConfigInt32Values index, char const* fieldname, int32 defvalue, int32 minvalue, int32 maxvalue);
	void setConfigMinMax(dConfigFloatValues index, char const* fieldname, float defvalue, float minvalue, float maxvalue);
	bool configNoReload(bool reload, dConfigUInt32Values index, char const* fieldname, uint32 defvalue);
	bool configNoReload(bool reload, dConfigInt32Values index, char const* fieldname, int32 defvalue);
	bool configNoReload(bool reload, dConfigFloatValues index, char const* fieldname, float defvalue);
	bool configNoReload(bool reload, dConfigBoolValues index, char const* fieldname, bool defvalue);

protected:
	uint32 m_configUint32Values[D_CFG_UINT32_VALUE_COUNT];
	int32 m_configInt32Values[D_CFG_INT32_VALUE_COUNT];
	float m_configFloatValues[D_CFG_FLOAT_VALUE_COUNT];
	bool m_configBoolValues[D_CFG_BOOL_VALUE_COUNT];

	// List of Maps that should be force-loaded on startup
	std::set<uint32> m_configForceLoadMapIds;

	// for max speed access
	static float m_MaxVisibleDistanceOnContinents;
	static float m_MaxVisibleDistanceInInstances;
	static float m_MaxVisibleDistanceInBG;

	static float m_MaxVisibleDistanceInFlight;
	static float m_VisibleUnitGreyDistance;
	static float m_VisibleObjectGreyDistance;

	static float  m_relocation_lower_limit_sq;
	static uint32 m_relocation_ai_notify_delay;

	IntervalTimer m_timers[DUPDATE_COUNT];
	std::string m_dataPath;
	std::string m_sServerName;
};

#define sConfigMgr MaNGOS::Singleton<ConfigMgr>::Instance()

#endif //__CONFIGMGR_H