#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Core/Log.h"

#include <unordered_map>

namespace Flibbert {

class ClassRegistry
{
public:
	struct ClassInfo
	{
		std::string_view Name;
		std::string_view Parent;
		ClassInfo* ParentInfo = nullptr;
		void* (*FactoryFunc)() = nullptr;
	};

	template <typename T>
	static void* ClassFactory()
	{
		return new T();
	}

	// @todo revisit my StringName implementation to use it here instead
	static std::unordered_map<std::string_view, ClassInfo> Classes;

	static void AddClass(std::string_view className);
	static void AddClass(std::string_view className, std::string_view parentClassName);

	template <typename T>
	static void RegisterAbstractClass()
	{
		static_assert(std::is_same_v<typename T::ThisClass, T>,
		              "Class not declared properly, please use FBTCLASS.");

		T::InitializeClass();

		FBT_CORE_TRACE("Registered abstract class {0}", T::ClassNamePrivate);
	}

	template <typename T>
	static void RegisterClass()
	{
		static_assert(std::is_same_v<typename T::ThisClass, T>,
		              "Class not declared properly, please use FBTCLASS.");

		T::InitializeClass();
		const auto found = Classes.find(T::ClassNamePrivate);
		found->second.FactoryFunc = &ClassFactory<T>;

		FBT_CORE_TRACE("Registered class {0}", T::ClassNamePrivate);
	}

	template <typename T>
	static T* Create()
	{
		static_assert(std::is_same_v<typename T::ThisClass, T>,
		              "Class not declared properly, please use FBTCLASS.");

		const auto found = Classes.find(T::ClassNamePrivate);
		FBT_CORE_ENSURE_MSG(found != Classes.end(), "Class not registered!");

		ClassInfo& info = found->second;

		return static_cast<T*>(info.FactoryFunc());
	}
};

} // namespace Flibbert

#define FBTBASECLASS(this_class)                                                                                       \
                                                                                                                       \
private:                                                                                                               \
	friend class ::Flibbert::ClassRegistry;                                                                        \
	static constexpr std::string_view ClassNamePrivate = #this_class;                                              \
                                                                                                                       \
public:                                                                                                                \
	using ThisClass = this_class;                                                                                  \
                                                                                                                       \
	static void InitializeClass()                                                                                  \
	{                                                                                                              \
		static bool initialized = false;                                                                       \
		if (initialized) {                                                                                     \
			return;                                                                                        \
		}                                                                                                      \
		::Flibbert::ClassRegistry::AddClass(ClassNamePrivate);                                                 \
		initialized = true;                                                                                    \
	}                                                                                                              \
                                                                                                                       \
	static const std::string_view& GetClassName()                                                                  \
	{                                                                                                              \
		return ClassNamePrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
private:

#define FBTCLASS(this_class, parent_class)                                                                             \
                                                                                                                       \
private:                                                                                                               \
	friend class ::Flibbert::ClassRegistry;                                                                        \
	static constexpr std::string_view ClassNamePrivate = #this_class;                                              \
                                                                                                                       \
public:                                                                                                                \
	using ThisClass = this_class;                                                                                  \
	using Super = parent_class;                                                                                    \
                                                                                                                       \
	static void InitializeClass()                                                                                  \
	{                                                                                                              \
		static bool initialized = false;                                                                       \
		if (initialized) {                                                                                     \
			return;                                                                                        \
		}                                                                                                      \
		Super::InitializeClass();                                                                              \
		::Flibbert::ClassRegistry::AddClass(ClassNamePrivate, Super::GetClassName());                          \
		initialized = true;                                                                                    \
	}                                                                                                              \
                                                                                                                       \
	static const std::string_view& GetClassName()                                                                  \
	{                                                                                                              \
		return ClassNamePrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
private:
