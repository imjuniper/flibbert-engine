#pragma once

#include <type_traits>

// Adapted from Unreal Engine and https://github.com/ricab/scope_guard/blob/main/scope_guard.hpp

namespace Flibbert::ScopeGuardSupport
{
	// Unlike ricab's ScopeGuard, we don't *really* need to check if it has no args and returns
	// void since we are enforcing it via the macro. In theory, people could call it manually,
	// but it is not supported
	template <typename Callback,
	          typename = std::enable_if_t<std::is_nothrow_destructible_v<Callback>>>
	class ScopeGuard;

	/**
	 * Nice little syntax helper used in UE, removes the need to add parentheses to the scope.
	 */
	struct SyntaxHelper {
		template <typename Callback>
		ScopeGuard<Callback> operator+(Callback&& callback) noexcept(
		    std::is_nothrow_constructible_v<Callback, Callback&&>)
		{
			return ScopeGuard<Callback>(std::forward<Callback>(callback));
		}
	};

	/**
	 * Not meant for direct consumption : use ON_SCOPE_EXIT instead.
	 *
	 * RAII class that calls a lambda when it is destroyed.
	 */
	template <typename Callback, typename>
	class ScopeGuard
	{
	public:
		ScopeGuard() = delete;
		ScopeGuard(ScopeGuard&&) = delete;
		ScopeGuard(const ScopeGuard&) = delete;
		ScopeGuard& operator=(ScopeGuard&&) = delete;
		ScopeGuard& operator=(const ScopeGuard&) = delete;

		~ScopeGuard() noexcept { m_Callback(); }

	private:
		explicit ScopeGuard(Callback&& callback) noexcept(
		    std::is_nothrow_constructible_v<Callback, Callback&&>)
		    : m_Callback(std::forward<Callback>(callback))
		{
		}

		friend ScopeGuard<Callback> SyntaxHelper::operator+ <Callback>(Callback&&) noexcept(
		    std::is_nothrow_constructible_v<Callback, Callback&&>);

	private:
		Callback m_Callback;
	};
} // namespace Flibbert::ScopeGuardSupport

#define FBT_PRIVATE_SCOPE_EXIT_JOIN_INNER(A, B) A##B
#define FBT_PRIVATE_SCOPE_EXIT_JOIN(A, B) FBT_PRIVATE_SCOPE_EXIT_JOIN_INNER(A, B)

/**
 * Enables a lambda to be executed on scope exit.
 *
 * @usage
 *    {
 *      FileHandle* Handle = GetFileHandle();
 *      ON_SCOPE_EXIT
 *      {
 *          CloseFile(Handle);
 *      };
 *
 *      DoSomethingWithFile( Handle );
 *
 *      // File will be closed automatically no matter how the scope is exited, e.g.:
 *      // * Any return statement.
 *      // * break or continue (if the scope is a loop body).
 *      // * An exception is thrown outside the block.
 *      // * Execution reaches the end of the block.
 *    }
 */
#define ON_SCOPE_EXIT                                                                              \
	const auto FBT_PRIVATE_SCOPE_EXIT_JOIN(ScopeGuard_, __LINE__) =                            \
	    ::Flibbert::ScopeGuardSupport::SyntaxHelper() + [&]() -> void
