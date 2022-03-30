// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SATORI/SATORIGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSATORIGameMode() {}
// Cross Module References
	SATORI_API UClass* Z_Construct_UClass_ASATORIGameMode_NoRegister();
	SATORI_API UClass* Z_Construct_UClass_ASATORIGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_SATORI();
// End Cross Module References
	void ASATORIGameMode::StaticRegisterNativesASATORIGameMode()
	{
	}
	UClass* Z_Construct_UClass_ASATORIGameMode_NoRegister()
	{
		return ASATORIGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ASATORIGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASATORIGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_SATORI,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASATORIGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "SATORIGameMode.h" },
		{ "ModuleRelativePath", "SATORIGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASATORIGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASATORIGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ASATORIGameMode_Statics::ClassParams = {
		&ASATORIGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ASATORIGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ASATORIGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASATORIGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ASATORIGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ASATORIGameMode, 805744708);
	template<> SATORI_API UClass* StaticClass<ASATORIGameMode>()
	{
		return ASATORIGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ASATORIGameMode(Z_Construct_UClass_ASATORIGameMode, &ASATORIGameMode::StaticClass, TEXT("/Script/SATORI"), TEXT("ASATORIGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASATORIGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
