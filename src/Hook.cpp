#include "Hook.h"
#include "DataHandler.h"

namespace AttackDataTransitionFix
{
	using EventResult = RE::BSEventNotifyControl;

	bool ShouldDelayAttackData(const RE::BSFixedString& a_animationEvent, RE::NiPointer<RE::BGSAttackData>& a_attackDataPtr) {
		if (a_attackDataPtr) {
			auto playerRef = RE::PlayerCharacter::GetSingleton();
			auto playerHighData = playerRef && playerRef->GetActorRuntimeData().currentProcess ? playerRef->GetActorRuntimeData().currentProcess->high : nullptr;
			if (playerHighData && playerHighData->attackData && std::addressof(playerHighData->attackData) == std::addressof(a_attackDataPtr) && a_animationEvent != a_attackDataPtr->event) {
				if (DelayedAttackDataHandler::ShouldDelayAttack(playerRef, a_animationEvent)) {
					SKSE::log::debug("Delay Attack Data \"{}\"", a_animationEvent.c_str());
					return true;
				}
			}
		}
		return false;
	}

	bool AttackDataUpdateHookSE::UpdateAttackData(void* a1, void* a2, int a_flag, const RE::BSFixedString& a_animationEvent, RE::NiPointer<RE::BGSAttackData>& a_attackDataPtr)
	{
		if (ShouldDelayAttackData(a_animationEvent, a_attackDataPtr)) {
			return false;
		}

		return _UpdateAttackData(a1, a2, a_flag, a_animationEvent, a_attackDataPtr);
	}

	bool AttackDataUpdateHookAE::UpdateAttackData(std::uint64_t unk, const RE::BSFixedString& a_animationEvent, RE::NiPointer<RE::BGSAttackData>& a_attackDataPtr)
	{
		if (ShouldDelayAttackData(a_animationEvent, a_attackDataPtr)) {
			return false;
		}

		return _UpdateAttackData(unk, a_animationEvent, a_attackDataPtr);
	}

	EventResult AnimEventHook::ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
	{
		if (a_event && a_event->holder && a_event->holder->IsPlayerRef()) {
			auto playerRef = RE::PlayerCharacter::GetSingleton()->As<RE::Actor>();
			if (playerRef) {
				DelayedAttackDataHandler::ReapplyAttackData(playerRef, a_event->tag);
			}
		}

		return _ProcessEvent(a_sink, a_event, a_eventSource);
	}
}