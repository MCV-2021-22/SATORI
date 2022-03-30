// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SATORI/Character/SATORI_PlayerState.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSATORI_PlayerState() {}
// Cross Module References
	SATORI_API UClass* Z_Construct_UClass_ASATORI_PlayerState_NoRegister();
	SATORI_API UClass* Z_Construct_UClass_ASATORI_PlayerState();
	ENGINE_API UClass* Z_Construct_UClass_APlayerState();
	UPackage* Z_Construct_UPackage__Script_SATORI();
// End Cross Module References
	void ASATORI_PlayerState::StaticRegisterNativesASATORI_PlayerState()
	{
	}
	UClass* Z_Construct_UClass_ASATORI_PlayerState_NoRegister()
	{
		return ASATORI_PlayerState::StaticClass();
	}
	struct Z_Construct_UClass_ASATORI_PlayerState_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ASATORI_PlayerState_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerState,
		(UObject* (*)())Z_Construct_UPackage__Script_SATORI,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ASATORI_PlayerState_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "Character/SATORI_PlayerState.h" },
		{ "ModuleRelativePath", "Character/SATORI_PlayerState.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ASATORI_PlayerState_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ASATORI_PlayerState>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ASATORI_PlayerState_Statics::ClassParams = {
		&ASATORI_PlayerState::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002A4u,
		METADATA_PARAMS(Z_Construct_UClass_ASATORI_PlayerState_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ASATORI_PlayerState_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ASATORI_PlayerState()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ASATORI_PlayerState_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ASATORI_PlayerState, 1945919817);
	template<> SATORI_API UClass* StaticClass<ASATORI_PlayerState>()
	{
		return ASATORI_PlayerState::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ASATORI_PlayerState(Z_Construct_UClass_ASATORI_PlayerState, &ASATORI_PlayerState::StaticClass, TEXT("/Script/SATORI"), TEXT("ASATORI_PlayerState"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ASATORI_PlayerState);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
