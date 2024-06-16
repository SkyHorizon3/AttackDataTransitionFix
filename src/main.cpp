#include "Hook.h"

void SetupLog()
{
	auto logsFolder = SKSE::log::log_directory();
	if (!logsFolder)
	{
		SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
	}

	auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
	auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
	auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
	auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));

	loggerPtr->set_level(spdlog::level::info);
	loggerPtr->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(loggerPtr));
}

#define DLLEXPORT __declspec(dllexport)
extern "C" DLLEXPORT const auto SKSEPlugin_Version = []() noexcept {
	SKSE::PluginVersionData v;
	v.PluginName(Plugin::NAME.data());
	v.PluginVersion(Plugin::VERSION);
	v.UsesAddressLibrary(true);
	v.HasNoStructUse();
	return v;
	}
();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo * pluginInfo)
{
	pluginInfo->name = SKSEPlugin_Version.pluginName;
	pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
	pluginInfo->version = SKSEPlugin_Version.pluginVersion;
	return true;
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	/*
#ifndef NDEBUG
	while (!IsDebuggerPresent()) {
		Sleep(100);
	}
#endif
	*/

	SKSE::Init(a_skse);
	SetupLog();

	SKSE::log::info("{} v{} loaded", Plugin::NAME, Plugin::VERSION);

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
