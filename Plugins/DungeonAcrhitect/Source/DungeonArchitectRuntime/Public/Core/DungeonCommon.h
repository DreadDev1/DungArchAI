//$ Copyright 2015-25, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#pragma once
#include "CoreMinimal.h"
#include "Core/Utils/DungeonUserParameters.h"
#include "DungeonCommon.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct DUNGEONARCHITECTRUNTIME_API FDungeonBuildStatusMessage {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Dungeon")
	bool bBuildInProgress{};
    
	UPROPERTY(BlueprintReadOnly, Category="Dungeon")
	FText ProgressText;
    
	UPROPERTY(BlueprintReadOnly, Category="Dungeon")
	FText ProgressSubText;
    
	UPROPERTY(BlueprintReadOnly, Category="Dungeon")
	bool bShowProgressBar{};
    
	UPROPERTY(BlueprintReadOnly, Category="Dungeon")
	int32 TotalWorkUnits{};
    
	UPROPERTY(BlueprintReadOnly, Category="Dungeon")
	int32 CompletedWorkUnits{};
};

enum class EDungeonBuildGraphPhase : uint8 {
	Initial,
	GenerateLayout,
	SetupPCG,
	GeneratePCG,
	SpawnItems,
	Finalize
};


struct FDungeonBuilderParameters {
	bool bOverrideDungeonTransform {};
	FTransform DungeonTransformOverride = FTransform::Identity;
	FDungeonUserParameterCollection CustomParameters;

	static const FDungeonBuilderParameters Default;
};

struct FDungeonBuildSettings {
	EDungeonBuildGraphPhase StartPhase = EDungeonBuildGraphPhase::Initial;

	/** Should we override the dungeon config's instance setting with the instanced setting below */ 
	bool bOverrideInstancedSetting = false;

	/** Only valid if bOverrideInstancedSetting is set to true, this setting overrides the dungeon config's instance flag */ 
	bool bInstanced = false;

	/** Should we override the dungeon config's frame build time setting with the setting below? */ 
	float bOverrideFrameBuildTimeMs = false;

	/** Only valid if bOverrideFrameBuildTimeMs is set to true, this setting overrides the dungeon config's frame build time setting */ 
	int32 FrameBuildTimeMs = 0;

	FDungeonBuilderParameters BuilderParameters;
};

