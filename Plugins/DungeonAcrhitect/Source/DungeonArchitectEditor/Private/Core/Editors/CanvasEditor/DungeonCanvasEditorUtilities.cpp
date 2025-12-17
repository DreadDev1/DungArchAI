//$ Copyright 2015-25, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#include "Core/Editors/CanvasEditor/DungeonCanvasEditorUtilities.h"

#include "Core/Editors/CanvasEditor/DungeonCanvasEditorSettings.h"
#include "Frameworks/Canvas/DungeonCanvasMaterialLayer.h"

#include "MaterialEditingLibrary.h"
#include "Materials/MaterialFunctionMaterialLayer.h"
#include "Materials/MaterialFunctionMaterialLayerBlend.h"
#include "Materials/MaterialInstanceConstant.h"

DEFINE_LOG_CATEGORY_STATIC(LogDungeonCanvasEditorUtils, Log, All);

void FDungeonCanvasEditorUtilities::InitializeThemeAsset(UDungeonCanvasMaterialTheme* NewAsset) {
	if (NewAsset) {
		const FSoftObjectPath PathMasterMaterial(TEXT("/DungeonArchitect/Core/Runtime/Features/DungeonCanvas/Materials/M_DungeonCanvasMaster.M_DungeonCanvasMaster")); 
		const FSoftObjectPath PathFogOfWarMaterial(TEXT("/DungeonArchitect/Core/Runtime/Features/DungeonCanvas/Materials/M_CanvasFogOfWar.M_CanvasFogOfWar")); 
		NewAsset->MaterialTemplateCanvas = Cast<UMaterialInterface>(PathMasterMaterial.TryLoad());
		NewAsset->FogOfWarMaterialTemplate = Cast<UMaterialInterface>(PathFogOfWarMaterial.TryLoad());
		NewAsset->CompiledThemeMaterial = NewObject<UMaterialInstanceConstant>(NewAsset, UMaterialInstanceConstant::StaticClass(), "CompiledMaterial", RF_Public);
		
		const FSoftObjectPath Path(TEXT("/DungeonArchitect/Core/Editors/CanvasEditor/Data/DungeonCanvasMaterialThemePresets.DungeonCanvasMaterialThemePresets"));
		if (UDungeonCanvasMaterialThemePresets* Presets = Cast<UDungeonCanvasMaterialThemePresets>(Path.TryLoad())) {
			NewAsset->OverlayIcons = Presets->OverlayIcons;
			NewAsset->StairIcon = Presets->StairIcon;
			NewAsset->MaterialTemplateCanvas = Presets->MaterialTemplateCanvas;
			NewAsset->FogOfWarMaterialTemplate = Presets->FogOfWarMaterialTemplate;
		}
	}
}

void FDungeonCanvasEditorUtilities::CompileDungeonCanvasMaterialTemplate(UDungeonCanvasMaterialTheme* CanvasThemeAsset) {
	if (!CanvasThemeAsset) {
		return;
	}
	UMaterialInterface* MaterialParent = CanvasThemeAsset->MaterialTemplateCanvas.LoadSynchronous();

	CanvasThemeAsset->CompiledThemeMaterial->Parent = MaterialParent;
	CompileDungeonCanvasMaterialTemplate(CanvasThemeAsset->CompiledThemeMaterial, CanvasThemeAsset->MaterialLayers);
}

void FDungeonCanvasEditorUtilities::CompileDungeonCanvasMaterialTemplate(UMaterialInstanceConstant* MaterialInstance, const TArray<UDungeonCanvasMaterialLayer*>& MaterialLayers) {
	if (!MaterialInstance) {
		return;
	}

	FMaterialLayersFunctions LayerFunctions;
	TArray<UDungeonCanvasMaterialLayer*> ReversedMatLayers = MaterialLayers.FilterByPredicate([](const UDungeonCanvasMaterialLayer* InLayer) {
		return InLayer && InLayer->MaterialLayer && InLayer->MaterialBlend;
	});
	Algo::Reverse(ReversedMatLayers);

	int32 LayerIndex = 0;
	for (UDungeonCanvasMaterialLayer* MaterialLayerInfo : ReversedMatLayers) {
		UMaterialFunctionMaterialLayerBlend* MaterialBlend = MaterialLayerInfo->MaterialBlend.LoadSynchronous();
		UMaterialFunctionMaterialLayer* MaterialLayer = MaterialLayerInfo->MaterialLayer.LoadSynchronous();
		
		if (!MaterialLayerInfo || !MaterialLayer || !MaterialBlend) {
			if (MaterialLayerInfo) {
				MaterialLayerInfo->LayerIndex = INDEX_NONE;
			}
			continue;
		}

		if (LayerIndex == 0) {
			LayerFunctions.AddDefaultBackgroundLayer();
			LayerFunctions.Layers[0] = MaterialLayer;
			LayerFunctions.EditorOnly.LayerNames[0] = MaterialLayerInfo->LayerName;
		}
		else {
			const int32 LayerIdx = LayerFunctions.AppendBlendedLayer();
			LayerFunctions.Layers[LayerIdx] = MaterialLayer;
			LayerFunctions.EditorOnly.LayerNames[LayerIdx] = MaterialLayerInfo->LayerName;
			LayerFunctions.EditorOnly.LayerStates[LayerIdx] = MaterialLayerInfo->bEnabled;
			
			check(LayerFunctions.Blends.IsValidIndex(LayerIdx - 1));
			LayerFunctions.Blends[LayerIdx - 1] = MaterialBlend;
		}

		MaterialLayerInfo->LayerIndex = LayerIndex;
		MaterialLayerInfo->Modify();
		
		LayerIndex++;
	}

	if (LayerFunctions.Layers.Num() > 0) {
		FMaterialInstanceParameterUpdateContext UpdateContext(MaterialInstance, EMaterialInstanceClearParameterFlag::None);
		UpdateContext.SetForceStaticPermutationUpdate(true);
		UpdateContext.SetMaterialLayers(LayerFunctions);
	}
	
	UMaterialEditingLibrary::UpdateMaterialInstance(MaterialInstance);
	MaterialInstance->MarkPackageDirty();
}

void FDungeonCanvasEditorUtilities::CopyMaterialLayers(const UMaterialInstanceConstant* Source, UMaterialInstanceConstant* Destination) {
	if (Source && Destination) {
		return;
	}
	
	FMaterialLayersFunctions LayerFunctions;
	Source->GetMaterialLayers(LayerFunctions);
	if (LayerFunctions.Layers.Num() > 0) {
		Destination->SetMaterialLayers(LayerFunctions);
		Destination->Modify();
	}
	
	// Copy over the material layer properties
	// Scalar Values:
	{
		TArray<FMaterialParameterInfo> ParamInfoList;
		TArray<FGuid> ParamIds;
		Source->GetAllScalarParameterInfo(ParamInfoList, ParamIds);
		for (const FMaterialParameterInfo& ParamInfo : ParamInfoList) {
			float Value{};
			Source->GetScalarParameterValue(ParamInfo, Value);
			Destination->SetScalarParameterValueEditorOnly(ParamInfo, Value);
		} 
	}
	
	// Vector Values:
	{
		TArray<FMaterialParameterInfo> ParamInfoList;
		TArray<FGuid> ParamIds;
		Source->GetAllVectorParameterInfo(ParamInfoList, ParamIds);
		for (const FMaterialParameterInfo& ParamInfo : ParamInfoList) {
			FLinearColor Value{};
			Source->GetVectorParameterValue(ParamInfo, Value);
			Destination->SetVectorParameterValueEditorOnly(ParamInfo, Value);
		} 
	}
	
	// Texture Values:
	{
		TArray<FMaterialParameterInfo> ParamInfoList;
		TArray<FGuid> ParamIds;
		Source->GetAllTextureParameterInfo(ParamInfoList, ParamIds);
		for (const FMaterialParameterInfo& ParamInfo : ParamInfoList) {
			UTexture* Value{};
			Source->GetTextureParameterValue(ParamInfo, Value);
			Destination->SetTextureParameterValueEditorOnly(ParamInfo, Value);
		} 
	}
}

