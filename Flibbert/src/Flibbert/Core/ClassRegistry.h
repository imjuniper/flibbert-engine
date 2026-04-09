#pragma once

#include "Flibbert/Core/Base.h"
#include "Flibbert/Core/Log.h"

#include <concepts>
#include <string_view>
#include <unordered_map>

namespace Flibbert {

template <typename T>
concept HasThisClassMember = requires(T v) { std::same_as<typename T::ThisClass, T>; };

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
	template <HasThisClassMember C>
	static void* ClassFactory()
	{
		return new C();
	}

	static ClassInfo* AddClass(std::string_view className);
	static ClassInfo* AddClass(std::string_view className, std::string_view parentClassName);

	template <HasThisClassMember C>
	static void GetChildClasses(std::vector<const ClassInfo*>& classes)
	{
		return GetChildClasses(C::StaticClass(), classes);
	}

	template <HasThisClassMember C>
	static void InitializeClass()
	{
		C::InitializeClass();
	}

	template <HasThisClassMember C>
	static void RegisterAbstractClass()
	{
		C::InitializeClass();

		FBT_CORE_TRACE("Registered abstract class {0}", C::ClassNamePrivate);
	}

	template <HasThisClassMember C>
	static void RegisterClass()
	{
		C::InitializeClass();
		const auto found = s_Classes.find(C::ClassNamePrivate);
		found->second.FactoryFunc = &ClassFactory<C>;

		FBT_CORE_TRACE("Registered class {0}", C::ClassNamePrivate);
	}

	template <HasThisClassMember C>
	static std::shared_ptr<C> Create()
	{
		return std::make_shared<C>();
	}

	template <HasThisClassMember C>
	static std::shared_ptr<C> Create(const ClassInfo* info)
	{
		return std::shared_ptr<C>(static_cast<C*>(info->FactoryFunc()));
	}
};

// @todo add custom Cast function

template <typename T>
concept HasStaticClassFunction = requires(T) {
	{ T::StaticClass() } -> std::same_as<const ClassRegistry::ClassInfo*>;
};

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
	virtual const std::string_view& GetClassName() const                                                           \
	{                                                                                                              \
		return ClassNamePrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
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
	template <HasStaticClassFunction C>                                                                            \
	bool IsA()                                                                                                     \
	{                                                                                                              \
		return IsA(C::StaticClass());                                                                          \
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
	virtual const std::string_view& GetClassName() const override                                                  \
	{                                                                                                              \
		return ClassNamePrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
	virtual const ::Flibbert::ClassRegistry::ClassInfo* GetClass() const override                                  \
	{                                                                                                              \
		return ClassInfoPrivate;                                                                               \
	}                                                                                                              \
                                                                                                                       \
private:
