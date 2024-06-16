#include "DataHandler.h"

namespace AttackDataTransitionFix
{

	bool DelayedAttackDataHandler::ShouldDelayAttack(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		if (!FindAttackData(playerRef, a_animationEvent))
			return false;

		const std::string BoolVarName = "ADTF_ShouldDelay";
		bool shouldDelayed = false;

		return playerRef->GetGraphVariableBool(BoolVarName, shouldDelayed) && shouldDelayed;
	}

	bool DelayedAttackDataHandler::ReapplyAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent, const RE::BSFixedString& a_payload)
	{
		constexpr std::string_view prefix = "ADTF_Reapply_";

		if (!playerRef)
			return false;

		auto playerHighData = playerRef && playerRef->GetActorRuntimeData().currentProcess ? playerRef->GetActorRuntimeData().currentProcess->high : nullptr;
		if (!playerHighData || !playerHighData->attackData)
			return false;

		std::string_view text{ a_animationEvent };
		if (text.starts_with(prefix)) {
			const auto attackEvent = text.substr(prefix.size());
			auto attackData = FindAttackData(playerRef, attackEvent);
			if (attackData && playerHighData->attackData.get() != attackData) {
				playerHighData->attackData.reset(attackData);
				SKSE::log::debug("Reapply Attack Data \"{}\" in event \"{}\"", attackEvent, text);
				return true;
			}
		}

		return false;
	}

	RE::BGSAttackData* DelayedAttackDataHandler::FindAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		if (!playerRef) {
			return nullptr;
		}

		auto race = playerRef->GetRace();
		auto attackDataMap = race ? race->attackDataMap.get() : nullptr;
		if (attackDataMap) {
			auto pair = attackDataMap->attackDataMap.find(a_animationEvent);
			if (pair != attackDataMap->attackDataMap.end() && pair->second) {
				return pair->second.get();
			}
		}

		return nullptr;
	}

}