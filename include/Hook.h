#pragma once

namespace AttackDataTransitionFix
{
	bool ShouldDelayAttackData(const RE::BSFixedString& a_animationEvent, RE::NiPointer<RE::BGSAttackData>& a_attackDataPtr);

	class AnimEventHook
	{
		using EventResult = RE::BSEventNotifyControl;

	public:
		static void InstallHook()
		{
			REL::Relocation<std::uintptr_t> AnimEventSinkVtbl{ RE::VTABLE_PlayerCharacter[2] };
			_ProcessEvent = AnimEventSinkVtbl.write_vfunc(0x1, ProcessEvent);
			SKSE::log::info("Hook Process Animation Event!");
		}

	private:
		static EventResult ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource);

		static inline REL::Relocation<decltype(ProcessEvent)> _ProcessEvent;
	};

	class AttackDataUpdateHookSE
	{
	public:
		static void InstallHook()
		{
			REL::Relocation<uintptr_t> hook{ REL::VariantID(26729, 0, 0x3F8440) };  //1403E88F0 - SE
			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(14);
			_UpdateAttackData = trampoline.write_call<5>(hook.address() + 0x3A, UpdateAttackData);
		};

	private:
		static bool UpdateAttackData(void* a1, void* a2, int a_flag, const RE::BSFixedString& a_animationEvent, RE::NiPointer<RE::BGSAttackData>& a_attackDataPtr);

		static inline REL::Relocation<decltype(UpdateAttackData)> _UpdateAttackData;
	};

	class AttackDataUpdateHookAE
	{
	public:
		static void InstallHook()
		{
			REL::Relocation<uintptr_t> hook{ REL::ID(27408) };  // 140403CF0 - 640
			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(14);
			_UpdateAttackData = trampoline.write_branch<5>(hook.address() + 0x4, UpdateAttackData);
		};

	private:
		static bool UpdateAttackData(std::uint64_t unk, const RE::BSFixedString& a_animationEvent, RE::NiPointer<RE::BGSAttackData>& a_attackDataPtr);

		static inline REL::Relocation<decltype(UpdateAttackData)> _UpdateAttackData;
	};
}