#pragma once

#include <map>

namespace Flibbert {

template <typename T>
struct MemberClass;

template <typename C, typename ReturnValue, typename... Args>
struct MemberClass<ReturnValue (C::*)(Args...)>
{
	using Type = C;
};

template <typename C, typename ReturnValue, typename... Args>
struct MemberClass<ReturnValue (C::*)(Args...) const>
{
	using Type = const C;
};

template <auto Function>
using MemberClassOf = typename MemberClass<decltype(Function)>::Type;

// @todo can this be simplified further with other concepts?
// Checks that Function is a member function pointer callable with Args and returns ReturnValue
template <auto Function, typename ReturnValue, typename... Args>
concept MatchingMemberFunction =
    std::is_member_function_pointer_v<decltype(Function)> &&
    std::is_invocable_r_v<ReturnValue, decltype(Function), MemberClassOf<Function>*, Args...>;

struct DelegateHandle
{
	constexpr DelegateHandle() noexcept : m_ID(INVALID_ID) {}
	explicit DelegateHandle(bool /*generateId*/) noexcept : m_ID(GetNewID()) {}
	~DelegateHandle() noexcept = default;

	bool IsValid() const noexcept
	{
		return m_ID != INVALID_ID;
	}

	void Reset() noexcept
	{
		m_ID = INVALID_ID;
	}

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

	operator bool() const noexcept
	{
		return IsValid();
	}
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

// @todo: Can I create a base class so that at least the *Stub functions can be shared?

template <typename ReturnValue, typename... Args>
class Delegate
{
	using DelegateFunction = ReturnValue (*)(void*, Args...);

	void* m_Instance = nullptr;
	DelegateFunction m_Function = nullptr;

	template <ReturnValue (*Function)(Args...)>
	static ReturnValue FreeFunctionStub(void*, Args... args)
	{
		return std::invoke(Function, std::forward<Args>(args)...);
	}

	template <auto Function>
		requires MatchingMemberFunction<Function, ReturnValue, Args...>
	static ReturnValue MemberFunctionStub(void* instance, Args... args)
	{
		return std::invoke(Function, (MemberClassOf<Function>*)instance, std::forward<Args>(args)...);
	}

public:
	Delegate() noexcept : m_Instance(nullptr), m_Function(nullptr) {}

	template <ReturnValue (*Function)(Args...)>
	void Bind()
	{
		m_Function = &FreeFunctionStub<Function>;
	}

	template <auto Function>
		requires MatchingMemberFunction<Function, ReturnValue, Args...>
	void Bind(MemberClassOf<Function>* instance)
	{
		m_Instance = instance;
		m_Function = &MemberFunctionStub<Function>;
	}

	bool IsBound() const
	{
		return m_Function != nullptr;
	}

	template <typename... Args2>
	ReturnValue Execute(Args2&&... args) const
	{
		FBT_CORE_ENSURE_MSG(IsBound(), "Cannot invoke unbound delegate. Call Bind() first.");
		return m_Function(m_Instance, std::forward<Args2>(args)...);
	}

	template <typename... Args2>
	ReturnValue ExecuteIfBound(Args2&&... args) const
	{
		if (IsBound()) {
			return m_Function(m_Instance, std::forward<Args2>(args)...);
		}
		return ReturnValue();
	}

	void Clear()
	{
		m_Instance = nullptr;
		m_Function = nullptr;
	}
};

template <typename... Args>
class MulticastDelegate
{
	using DelegateFunction = void (*)(void*, Args...);
	using Stub = std::pair<void*, DelegateFunction>;

	std::map<DelegateHandle, Stub> m_BoundFunctions;

	template <void (*Function)(Args...)>
	static void FreeFunctionStub(void*, Args... args)
	{
		std::invoke(Function, std::forward<Args>(args)...);
	}

	template <auto Function>
		requires MatchingMemberFunction<Function, void, Args...>
	static void MemberFunctionStub(void* instance, Args... args)
	{
		std::invoke(Function, (MemberClassOf<Function>*)instance, std::forward<Args>(args)...);
	}

public:
	MulticastDelegate() noexcept : m_BoundFunctions() {}

	template <void (*Function)(Args...)>
	[[nodiscard]] DelegateHandle Add()
	{
		DelegateHandle newHandle(true);
		m_BoundFunctions.insert({newHandle, Stub(nullptr, &FreeFunctionStub<Function>)});
		return newHandle;
	}

	template <auto Function>
		requires MatchingMemberFunction<Function, void, Args...>
	[[nodiscard]] DelegateHandle Add(MemberClassOf<Function>* instance)
	{
		DelegateHandle newHandle(true);
		m_BoundFunctions.insert({newHandle, Stub{instance, &MemberFunctionStub<Function>}});
		return newHandle;
	}

	bool Remove(DelegateHandle& handleToRemove)
	{
		if (!handleToRemove.IsValid()) {
			return false;
		}

		const bool erased = m_BoundFunctions.erase(handleToRemove) > 0;
		if (erased) {
			handleToRemove.Reset();
		}
		return erased;
	}

	void RemoveAll(void* boundObject)
	{
		if (boundObject == nullptr) {
			return;
		}

		std::erase_if(m_BoundFunctions, [boundObject](const auto& pair) {
			DelegateHandle& handle = pair.first;
			const Stub& stub = pair.second;

			const bool shouldErase = stub.first == boundObject;
			if (shouldErase) {
				handle.Reset();
			}
			return shouldErase;
		});
	}

	template <typename... Args2>
	void Broadcast(Args2&&... args) const
	{
		for (const auto& [_, stub] : m_BoundFunctions) {
			stub.second(stub.first, std::forward<Args2>(args)...);
		}
	}
};

} // namespace Flibbert
