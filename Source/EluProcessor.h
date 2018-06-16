// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetData.h"
#include "UObject/NoExportTypes.h"
#include "EluProcessor.generated.h"


struct RAIDERZASSETS_API FEluMatInfo
{
	FString Texture_DiffuseMap;
	FString Texture_SpecularMap;
	FString Texture_NormalMap;
	FString Texture_SelfIlluminationMap;
	FString Texture_OpacityMap;
	FString Texture_ReflectMap;
	FString Texture_GlossMap;
	FString Texture_SSSMask;

	FVector Vector_Diffuse;
	FVector Vector_Ambient;
	FVector Vector_Specular;

	float Scalar_SpecularLevel;
	float Scalar_Glossiness;
	float Scalar_SelfIllusionScale;

	bool bMaterialType_TwoSided;
	bool bMaterialType_HumanBody;
	bool bMaterialType_HumanFace;
	bool bOpacityMaskChannel_Alpha;
	bool bOpacityBlendModel_Translucent;

	bool bAnimatedTexture_AllTextures;
	bool bAnimatedTexture_Diffuse;
	bool bAnimatedTexture_Specular;
	bool bAnimatedTexture_Normal;
	bool bAnimatedTexture_Glow;
	bool bAnimatedTexture_Opacity;

	FEluMatInfo();

};


struct RAIDERZASSETS_API FEluImportFilesInfo
{
	FString DirPath_EluObject;
	FString DirName_EluObject;

	TArray<FString> FilePaths_EluModels;
	TSet<FString> FilePaths_EluAnimations;

	FString FilePath_AnimationXml;
	FString FilePath_AnimationEventXml;
	FString FilePath_AnimationInfoXml;
	FString FilePath_AnimationSoundEventXml;
	FString FilePath_EluXml;
	FString FilePath_SceneXml;

	TMap<FString, FEluMatInfo> Map_EluMatsInfo;

	FEluImportFilesInfo(const FString& FilePath_EluXml);
};


UENUM(BlueprintType)
enum class EEluModelType : uint8
{
	Unknown,
	Monster,
	MapObject,
	Female,
	Male,
	Ride,
	Sky,
	Weapon,
	NPC,
	SFX,
};


UENUM(BlueprintType)
enum class EImportResult : uint8
{
	Failure,
	Success,
	Cancelled
};

/**
 * 
 */
UCLASS()
class RAIDERZASSETS_API UEluProcessor : public UObject
{
	GENERATED_BODY()
	
public:

	static const FString DirName_EluAnimations;

	static const FString DirName_EluModels;

	static const FString EditorDir_Textures;

	static const FString EditorDir_SimpleMaterials;

	static const FString EditorDir_HumanSkinMaterials;

	static const FString EditorDir_MaterialInstances;

	static const FString DirPath_AllModels;

	static const FString EditorDir_AllModels;

	static const FString FilePath_ErrorFile;

	TMap<FName, FAssetData> AssetDataMap_SimpleMaterials;

	TMap<FName, FAssetData> AssetDataMap_HumanSkinMaterials;

	TMap<FName, FAssetData> AssetDataMap_Textures;

	UPROPERTY()
	class UFbxFactory* SkeletalFbxFactory;

	UPROPERTY()
	class UFbxFactory* StaticFbxFactory;

	UPROPERTY()
	class UMaterialInstanceConstantFactoryNew* MIConstantFactoryNew;

	UEluProcessor(const FObjectInitializer& ObjectInitializer);

	void Initialize();

	void Uninitialize();

	class UStaticMesh* ImportStaticMesh(const FString& FilePath_EluModel, const FString& EditorDir_ModelPackage, EImportResult& Result);

	class USkeletalMesh* ImportSkeletalMesh(const FString& FilePath_EluModel, const FString& EditorDir_ModelPackage, EImportResult& Result);
	
	// class USkeletalMesh* ImportSkeletalMesh();

	static FString GetTextureNameFromXmlNode(class FXmlNode* XmlNode);

	static FString GetEditorMatName(FEluMatInfo MatInfo);

	static EEluModelType GetEluModelType(const FString& FilePath_EluXml);

	static void AddError(const FString& ErrorMessage);

	static bool AreMaterialInstanceParametersSame(class UMaterialInstanceConstant* MIConstant, FEluMatInfo MatInfo);

	static void ParseEluXmlForMaterials(const FString& FilePath_EluXml, TMap<FString, FEluMatInfo>& OutMap_EluMatsInfo);

	bool FillMaterialInstanceTextureParameter(class UMaterialInstanceConstant* MIConstant, FName ParamName, FName TextureName);

	bool FillMaterialInstanceParameters(class UMaterialInstanceConstant* MIConstant, const FString& EditorMatName, const FEluMatInfo& MatInfo);

	EImportResult CreateAndApplyStaticMeshMaterials(class UStaticMesh* StaticMesh, TMap<FString, FEluMatInfo> Map_EluMatsInfo);

	EImportResult CreateAndApplySkeletalMeshMaterials(class USkeletalMesh* SkeletalMesh, TMap<FString, FEluMatInfo> Map_EluMatsInfo);

	EImportResult ImportEluModels(const TArray<FString>& InPaths_EluXmlFilesToLoad, TArray<FString>& OutPaths_EluXmlFilesLoaded);
	
};
