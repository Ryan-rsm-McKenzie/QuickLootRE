#pragma once

namespace Animation
{
	class IEventSink
	{
	public:
		virtual ~IEventSink() = default;

		void operator()() { OnAnimationEvent(); }

	protected:
		virtual void OnAnimationEvent() = 0;
	};

	class AnimationManager
	{
	public:
		static AnimationManager& GetSingleton()
		{
			static AnimationManager singleton;
			return singleton;
		}

		static void Install()
		{
			auto& singleton = GetSingleton();
			singleton.DoInstall();
		}

		constexpr void SetEventSink(observer<IEventSink*> a_sink) noexcept { _sink = a_sink; }

	protected:
		class AnimHandler :
			public RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>
		{
		private:
			using super = RE::IHandlerFunctor<RE::Actor, RE::BSFixedString>;

		public:
			AnimHandler() = delete;

			AnimHandler(RE::BSTSmartPointer<super> a_original) :
				super(),
				_original(std::move(a_original))
			{}

			bool ExecuteHandler(RE::Actor& a_handler, const RE::BSFixedString& a_parameter) override
			{
				const auto& manager = AnimationManager::GetSingleton();
				manager.OnAnimationEvent();

				return _original ? (*_original)(a_handler, a_parameter) : true;
			}

		private:
			RE::BSTSmartPointer<super> _original;
		};

		void OnAnimationEvent() const
		{
			if (_sink) {
				(*_sink)();
			}
		}

	private:
		AnimationManager() = default;
		AnimationManager(const AnimationManager&) = delete;
		AnimationManager(AnimationManager&&) = delete;

		~AnimationManager() = default;

		AnimationManager& operator=(const AnimationManager&) = delete;
		AnimationManager& operator=(AnimationManager&&) = delete;

		void DoInstall()
		{
			auto handlers = RE::ResponseDictionary::GetSingleton();
			RE::BSSpinLockGuard locker(handlers->definitionLock);
			auto& definitions = handlers->objectDefinitions;

			auto it = definitions.find("PlayerCharacterResponse"sv);
			if (it != definitions.end() && it->second) {
				auto animResponse = it->second;
				for (const auto& event : EVENTS) {
					InjectHandler(*animResponse, event);
				}
			} else {
				assert(false);
			}

			logger::info("Installed {}"sv, typeid(decltype(*this)).name());
		}

		void InjectHandler(RE::AnimResponse& a_response, std::string_view a_animation)
		{
			const RE::BSFixedString anim(a_animation);
			auto original = a_response.GetHandler(anim);
			a_response.handlerMap.insert(
				{ std::move(anim),
					RE::make_smart<AnimHandler>(std::move(original)) });
		}

		static constexpr std::array EVENTS{
			"KillMoveStart"sv,
			"KillMoveEnd"sv,
			"StartAnimatedCamera"sv,
			"EndAnimatedCamera"sv
		};

		observer<IEventSink*> _sink{ nullptr };
	};
}
