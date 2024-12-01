#include "Hook.h"
#include <SimpleIni.h>


bool debugLog = false;

void LoadINI()
{
	const auto path = std::format("Data/SKSE/Plugins/{}.ini", Plugin::NAME);

	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(path.c_str());

	debugLog = ini.GetBoolValue("Debug", "EnableDebugLog");
}

#define DLLEXPORT __declspec(dllexport)
extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []()
	{
		SKSE::PluginVersionData v;
		v.PluginName(Plugin::NAME);
		v.AuthorName("Maxsu and SkyHorizon");
		v.PluginVersion(Plugin::VERSION);
		v.UsesAddressLibrary();
		v.UsesNoStructs();
		return v;
	}
();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo* pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{

	SKSE::Init(a_skse, true);
	LoadINI();

	spdlog::set_pattern("[%H:%M:%S:%e] [%l] %v"s);

	if (debugLog)
	{
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_on(spdlog::level::trace);
	}
	else
	{
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_on(spdlog::level::info);
	}

	SKSE::log::info("Game version: {}", a_skse->RuntimeVersion());

	// do stuff
	AttackDataTransitionFix::AnimEventHook::InstallHook();


	if (REL::Module::IsAE())
	{
		AttackDataTransitionFix::AttackDataUpdateHookAE::InstallHook();
	}
	else
	{
		AttackDataTransitionFix::AttackDataUpdateHookSE::InstallHook();
	}

	return true;
}
