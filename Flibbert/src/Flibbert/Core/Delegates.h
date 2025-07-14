#pragma once

#include <map>

// Would love to implement something more similar to actual delegates, like
// https://github.com/simco50/CppDelegates/blob/master/Delegates.h
// but I want to understand it first

// Thanks Hazel <3
#define FBT_BIND_EVENT(function)                                                                   \
	[this](auto&&... args) -> decltype(auto) {                                                 \
		return this->function(std::forward<decltype(args)>(args)...);                      \
	}

#define FBT_BIND_EVENT_OBJ(objectPtr, function)                                                    \
	[objectPtr](auto&&... args) -> decltype(auto) {                                            \
		return (objectPtr)->function(std::forward<decltype(args)>(args)...);                 \
	}

namespace Flibbert
{
	struct DelegateHandle {
		constexpr DelegateHandle() noexcept : m_ID(INVALID_ID) {}
		explicit DelegateHandle(bool /*generateId*/) noexcept : m_ID(GetNewID()) {}
		~DelegateHandle() noexcept = default;

		bool IsValid() const noexcept { return m_ID != INVALID_ID; }

		void Reset() noexcept { m_ID = INVALID_ID; }

		// Copy constructor and assignment
		DelegateHandle(const DelegateHandle& other) = default;
		DelegateHandle& operator=(const DelegateHandle& other) = default;

		// Move constructor
		DelegateHandle(DelegateHandle&& other) noexcept : m_ID(other.m_ID)
		{
			other.Reset();
		}

		// Move assignment
		DelegateHandle& operator=(DelegateHandle&& other) noexcept
		{
			m_ID = other.m_ID;
			other.Reset();
			return *this;
		}

		operator bool() const noexcept { return IsValid(); }
		bool operator==(const DelegateHandle& other) const noexcept
		{
			return m_ID == other.m_ID;
		}

		bool operator<(const DelegateHandle& other) const noexcept
		{
			return m_ID < other.m_ID;
		}

	private:
		uint32_t m_ID;
		constexpr static uint32_t INVALID_ID = std::numeric_limits<uint32_t>::max();
		static uint32_t NEXT_ID;

		static uint32_t GetNewID()
		{
			uint32_t output = DelegateHandle::NEXT_ID++;
			if (DelegateHandle::NEXT_ID == INVALID_ID) {
				DelegateHandle::NEXT_ID = 0;
			}
			return output;
		}
	};

	template <typename ReturnValue, typename... Args>
	class Delegate
	{
	public:
		using DelegateFunction = std::function<ReturnValue(Args...)>;

		// Default constructor
		Delegate() noexcept : m_BoundFunction() {}

		void Bind(DelegateFunction function) { m_BoundFunction = function; }

		bool IsBound() const { return !!m_BoundFunction; }

		ReturnValue Execute(Args&... args) const
		{
			FBT_CORE_ENSURE_MSG(m_BoundFunction, "Delegate is not bound!");
			return m_BoundFunction(std::forward<Args>(args)...);
		}

		ReturnValue ExecuteIfBound(Args&... args) const
		{
			if (IsBound()) {
				return m_BoundFunction(std::forward<Args>(args)...);
			}
			return ReturnValue();
		}

		void Clear() { m_BoundFunction = nullptr; }

	private:
		DelegateFunction m_BoundFunction;
	};

	template <typename... Args>
	class MulticastDelegate
	{
	public:
		using DelegateFunction = std::function<void(Args...)>;

		// Default constructor
		MulticastDelegate() noexcept : m_Events() {}

		DelegateHandle Add(DelegateFunction function)
		{
			DelegateHandle newHandle(true);
			m_Events.insert({newHandle, function});
			return newHandle;
		}

		bool Remove(DelegateHandle& handleToRemove)
		{
			if (!handleToRemove.IsValid()) {
				return false;
			}

			for (const auto& event : m_Events) {
				if (event.first == handleToRemove) {
					m_Events.erase(handleToRemove);
					handleToRemove.Reset();
					return true;
				}
			}
		}

		void Broadcast(Args&... args) const
		{
			for (const auto& event : m_Events) {
				event.second(std::forward<Args>(args)...);
			}
		}

	private:
		std::map<DelegateHandle, DelegateFunction> m_Events;
	};
} // namespace Flibbert
