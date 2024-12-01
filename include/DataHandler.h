#pragma once

namespace AttackDataTransitionFix
{
	class DelayedAttackDataHandler
	{
	public:
		static bool ShouldDelayAttack(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);
		static bool ReapplyAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);
		static RE::BGSAttackData* FindAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);
	};
}