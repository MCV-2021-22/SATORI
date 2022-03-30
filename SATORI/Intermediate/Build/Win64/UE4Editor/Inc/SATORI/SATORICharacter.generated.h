// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SATORI_SATORICharacter_generated_h
#error "SATORICharacter.generated.h already included, missing '#pragma once' in SATORICharacter.h"
#endif
#define SATORI_SATORICharacter_generated_h

#define SATORI_Source_SATORI_SATORICharacter_h_14_SPARSE_DATA
#define SATORI_Source_SATORI_SATORICharacter_h_14_RPC_WRAPPERS
#define SATORI_Source_SATORI_SATORICharacter_h_14_RPC_WRAPPERS_NO_PURE_DECLS
#define SATORI_Source_SATORI_SATORICharacter_h_14_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesASATORICharacter(); \
	friend struct Z_Construct_UClass_ASATORICharacter_Statics; \
public: \
	DECLARE_CLASS(ASATORICharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/SATORI"), NO_API) \
	DECLARE_SERIALIZER(ASATORICharacter) \
	virtual UObject* _getUObject() const override { return const_cast<ASATORICharacter*>(this); }


#define SATORI_Source_SATORI_SATORICharacter_h_14_INCLASS \
private: \
	static void StaticRegisterNativesASATORICharacter(); \
	friend struct Z_Construct_UClass_ASATORICharacter_Statics; \
public: \
	DECLARE_CLASS(ASATORICharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/SATORI"), NO_API) \
	DECLARE_SERIALIZER(ASATORICharacter) \
	virtual UObject* _getUObject() const override { return const_cast<ASATORICharacter*>(this); }


#define SATORI_Source_SATORI_SATORICharacter_h_14_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ASATORICharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ASATORICharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASATORICharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASATORICharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASATORICharacter(ASATORICharacter&&); \
	NO_API ASATORICharacter(const ASATORICharacter&); \
public:


#define SATORI_Source_SATORI_SATORICharacter_h_14_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ASATORICharacter(ASATORICharacter&&); \
	NO_API ASATORICharacter(const ASATORICharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ASATORICharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ASATORICharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ASATORICharacter)


#define SATORI_Source_SATORI_SATORICharacter_h_14_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CameraBoom() { return STRUCT_OFFSET(ASATORICharacter, CameraBoom); } \
	FORCEINLINE static uint32 __PPO__FollowCamera() { return STRUCT_OFFSET(ASATORICharacter, FollowCamera); } \
	FORCEINLINE static uint32 __PPO__AbilitySystemComponent() { return STRUCT_OFFSET(ASATORICharacter, AbilitySystemComponent); }


#define SATORI_Source_SATORI_SATORICharacter_h_11_PROLOG
#define SATORI_Source_SATORI_SATORICharacter_h_14_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SATORI_Source_SATORI_SATORICharacter_h_14_PRIVATE_PROPERTY_OFFSET \
	SATORI_Source_SATORI_SATORICharacter_h_14_SPARSE_DATA \
	SATORI_Source_SATORI_SATORICharacter_h_14_RPC_WRAPPERS \
	SATORI_Source_SATORI_SATORICharacter_h_14_INCLASS \
	SATORI_Source_SATORI_SATORICharacter_h_14_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define SATORI_Source_SATORI_SATORICharacter_h_14_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SATORI_Source_SATORI_SATORICharacter_h_14_PRIVATE_PROPERTY_OFFSET \
	SATORI_Source_SATORI_SATORICharacter_h_14_SPARSE_DATA \
	SATORI_Source_SATORI_SATORICharacter_h_14_RPC_WRAPPERS_NO_PURE_DECLS \
	SATORI_Source_SATORI_SATORICharacter_h_14_INCLASS_NO_PURE_DECLS \
	SATORI_Source_SATORI_SATORICharacter_h_14_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SATORI_API UClass* StaticClass<class ASATORICharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID SATORI_Source_SATORI_SATORICharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
