//$ Copyright 2015-25, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#pragma once
#include "CoreMinimal.h"
#include "DungeonCanvasMaterialLayer.generated.h"

class UMaterialFunctionMaterialLayer;
class UMaterialFunctionMaterialLayerBlend;

UCLASS(BlueprintType, Blueprintable)
class DUNGEONARCHITECTRUNTIME_API UDungeonCanvasMaterialLayer : public UObject {
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, Category="Material Layer")
	FText LayerName;

	UPROPERTY(EditAnywhere, Category="Material Layer")
	FText LayerDescription;

	UPROPERTY(EditDefaultsOnly, Category="Material Functions")
	TSoftObjectPtr<UMaterialFunctionMaterialLayer> MaterialLayer;
	
	UPROPERTY(EditDefaultsOnly, Category="Material Functions")
	TSoftObjectPtr<UMaterialFunctionMaterialLayerBlend> MaterialBlend;

	UPROPERTY() 
	bool bEnabled = true;
	
	UPROPERTY()
	int32 LayerIndex = INDEX_NONE;
};

