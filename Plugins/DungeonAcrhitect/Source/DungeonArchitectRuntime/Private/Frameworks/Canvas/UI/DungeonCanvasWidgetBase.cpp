//$ Copyright 2015-25, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#include "Frameworks/Canvas/UI/DungeonCanvasWidgetBase.h"

#include "Core/Dungeon.h"
#include "Frameworks/Canvas/DungeonCanvas.h"
#include "Frameworks/Canvas/Themes/DungeonCanvasMaterialThemeBaker.h"

#include "Engine/CanvasRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"

DEFINE_LOG_CATEGORY_STATIC(LogDungeonCanvasWidget, Log, All);

void UDungeonCanvasWidgetBase::NativeConstruct() {
	Super::NativeConstruct();

	if (ThemeBaker) {
		ThemeBaker->ReleaseResources();
		ThemeBaker = nullptr;
	}

	if (ADungeon* Dungeon = Cast<ADungeon>(UGameplayStatics::GetActorOfClass(this, ADungeon::StaticClass()))) {
		DungeonCanvasComponent = Dungeon->GetComponentByClass<UDungeonCanvasComponent>();
	}
	
	// Setup the camera
	if (Camera) {
		Camera->PlayerController = GetOwningPlayer();
	}
	
	ThemeBaker = NewObject<UDungeonCanvasMaterialThemeBaker>(this);
	ThemeBaker->ThemeOverride.Set(CanvasThemeOverride);
	ThemeBaker->DrawSettings.Set(DrawSettings);
	ThemeBaker->DungeonCanvas.Set(DungeonCanvasComponent.Get());
	ThemeBaker->DungeonCamera.Set(Camera);

	if (MinimapMaterialTemplate) {
		MinimapMaterialInstance = UMaterialInstanceDynamic::Create(MinimapMaterialTemplate, this);
		if (CanvasImageWidget) {
			FSlateBrush SlateBrush = CanvasImageWidget->GetBrush();
			SlateBrush.DrawAs = ESlateBrushDrawType::Image;
			SlateBrush.Tiling = ESlateBrushTileType::NoTile;
			SlateBrush.SetResourceObject(MinimapMaterialInstance);
			CanvasImageWidget->SetBrush(SlateBrush);
		}
	}
	
}

void UDungeonCanvasWidgetBase::NativeDestruct() {
	Super::NativeDestruct();

	if (ThemeBaker) {
		ThemeBaker->ReleaseResources();
		ThemeBaker = nullptr;
	}
}

void UDungeonCanvasWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	FIntPoint CurrentWidgetSize = (MyGeometry.GetDrawSize() * CanvasTextureScaleMultiplier).IntPoint();
	CurrentWidgetSize.X = FMath::Max(CurrentWidgetSize.X, 1);
	CurrentWidgetSize.Y = FMath::Max(CurrentWidgetSize.Y, 1);

	if (ThemeBaker) {
		ThemeBaker->Tick(CurrentWidgetSize);
		if (MinimapMaterialInstance) {
			MinimapMaterialInstance->SetTextureParameterValue(TEXT("RenderTexture"), ThemeBaker->GetRenderedTexture());
			if (DungeonCanvasComponent.IsValid() && Camera) {
				DungeonCanvasComponent->SetupMaterialParameters(MinimapMaterialInstance, ThemeBaker->GetCachedViewTransform(), Camera->FloorIndex);
			}
		}
	}

}

#if WITH_EDITOR
void UDungeonCanvasWidgetBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (FProperty* Property = PropertyChangedEvent.Property) {
		const FName PropertyName = Property->GetFName();
		
	}
}
#endif // WITH_EDITOR

