#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Core/Log.h"

#include <memory>
#include <string_view>
#include <unordered_map>

namespace Flibbert {

class ClassRegistry
{
public:
	struct ClassInfo
	{
		uint32_t ClassID;
		std::string_view Name;
		ClassInfo* ParentInfo = nullptr;
		void* (*FactoryFunc)() = nullptr;

		bool IsA(const ClassInfo* classInfo) const;
	};

private:
	static uint32_t s_NextClassInfoID;

	// @todo revisit my StringName implementation to use it here instead
	static std::unordered_map<std::string_view, ClassInfo> s_Classes;

	static void GetChildClasses(const ClassInfo* classInfo, std::vector<const ClassInfo*>& classes);

public:
	template <typename T>
	static void* ClassFactory()
	{
		return new T();
	}

	static ClassInfo* AddClass(std::string_view className);
	static ClassInfo* AddClass(std::string_view className, std::string_view parentClassName);

	template <typename T>
	static void GetChildClasses(std::vector<const ClassInfo*>& classes)
	{
		static_assert(std::is_same_v<typename T::ThisClass, T>,
		              "Class not declared properly, please use FBTCLASS.");

		return GetChildClasses(T::StaticClass(), classes);
	}

	template <typename T>
	static void InitializeClass()
	{
		static_assert(std::is_same_v<typename T::ThisClass, T>,
		              "Class not declared properly, please use FBTCLASS.");

		T::InitializeClass();
	}

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
		const auto found = s_Classes.find(T::ClassNamePrivate);
		found->second.FactoryFunc = &ClassFactory<T>;

		FBT_CORE_TRACE("Registered class {0}", T::ClassNamePrivate);
	}

	template <typename T>
	static std::shared_ptr<T> Create()
	{
		static_assert(std::is_same_v<typename T::ThisClass, T>,
		              "Class not declared properly, please use FBTCLASS.");

		return std::make_shared<T>();
	}

	template <typename T>
	static std::shared_ptr<T> Create(const ClassInfo* info)
	{
		static_assert(std::is_same_v<typename T::ThisClass, T>,
		              "Class not declared properly, please use FBTCLASS.");

		return std::shared_ptr<T>(static_cast<T*>(info->FactoryFunc()));
	}
};

// @todo add custom Cast function

} // namespace Flibbert

#define FBT_CLASS_BODY_IMPL(this_class)                                                                                \
                                                                                                                       \
private:                                                                                                               \
	static constexpr std::string_view ClassNamePrivate = #this_class;                                              \
	inline static const ::Flibbert::ClassRegistry::ClassInfo* ClassInfoPrivate = nullptr;                          \
                                                                                                                       \
	friend class ::Flibbert::ClassRegistry;                                                                        \
	using ThisClass = this_class;                                                                                  \
                                                                                                                       \
public:                                                                                                                \
	static const std::string_view& StaticClassName()                                                               \
	{                                                                                                              \
		return ClassNamePrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
	static const ::Flibbert::ClassRegistry::ClassInfo* StaticClass()                                               \
	{                                                                                                              \
		return ClassInfoPrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
private:

#define FBTBASECLASS(this_class)                                                                                       \
	FBT_CLASS_BODY_IMPL(this_class)                                                                                \
                                                                                                                       \
	static void InitializeClass()                                                                                  \
	{                                                                                                              \
		static bool initialized = false;                                                                       \
		if (initialized) {                                                                                     \
			return;                                                                                        \
		}                                                                                                      \
		ClassInfoPrivate = ::Flibbert::ClassRegistry::AddClass(ClassNamePrivate);                              \
		initialized = true;                                                                                    \
	}                                                                                                              \
                                                                                                                       \
public:                                                                                                                \
	virtual const ::Flibbert::ClassRegistry::ClassInfo* GetClass() const                                           \
	{                                                                                                              \
		return ClassInfoPrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
	virtual bool IsA(const ::Flibbert::ClassRegistry::ClassInfo* otherClass)                                       \
	{                                                                                                              \
		return GetClass()->IsA(otherClass);                                                                    \
	}                                                                                                              \
                                                                                                                       \
	template <typename T>                                                                                          \
	bool IsA()                                                                                                     \
	{                                                                                                              \
		return IsA(T::StaticClass());                                                                          \
	}                                                                                                              \
                                                                                                                       \
private:

#define FBTCLASS(this_class, parent_class)                                                                             \
	FBT_CLASS_BODY_IMPL(this_class)                                                                                \
                                                                                                                       \
	using Super = parent_class;                                                                                    \
                                                                                                                       \
	static void InitializeClass()                                                                                  \
	{                                                                                                              \
		static bool initialized = false;                                                                       \
		if (initialized) {                                                                                     \
			return;                                                                                        \
		}                                                                                                      \
		::Flibbert::ClassRegistry::InitializeClass<Super>();                                                   \
		ClassInfoPrivate = ::Flibbert::ClassRegistry::AddClass(ClassNamePrivate, Super::StaticClassName());    \
		initialized = true;                                                                                    \
	}                                                                                                              \
                                                                                                                       \
public:                                                                                                                \
	virtual const ::Flibbert::ClassRegistry::ClassInfo* GetClass() const override                                  \
	{                                                                                                              \
		return ClassInfoPrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
private:
