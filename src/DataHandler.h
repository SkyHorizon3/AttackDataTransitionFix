#pragma once
#include "DKUtil/Config.hpp"

namespace AttackDataTransitionFix
{
	using namespace DKUtil::Alias;

	class DelayedAttackDataHandler
	{
	public:
		static bool ShouldDelayAttack(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);
		static bool ReapplyAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent, const RE::BSFixedString& a_payload);
		static RE::BGSAttackData* FindAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);
	};
}