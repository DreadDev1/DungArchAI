//$ Copyright 2015-25, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#include "Frameworks/Canvas/UI/Viewport/DungeonCanvasViewportWidget.h"

#include "Core/Dungeon.h"
#include "Frameworks/Canvas/DungeonCanvas.h"
#include "Frameworks/Canvas/UI/Viewport/SDungeonCanvasViewport.h"

#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "DungeonCanvasViewportWidget"

void UDungeonCanvasViewportWidget::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);

	DungeonCanvasViewport = nullptr;
	DungeonCanvasPtr = nullptr;
}

TSharedRef<SWidget> UDungeonCanvasViewportWidget::RebuildWidget()
{
	DungeonCanvasViewport = SNew(SDungeonCanvasViewport)
		.DrawSettings_Lambda([this]() { return DrawSettings; })
		.Camera_Lambda([this]() { return Camera; });

	if (!DungeonCanvasPtr.IsValid()) {
		if (ADungeon* Dungeon = Cast<ADungeon>(UGameplayStatics::GetActorOfClass(GetWorld(), ADungeon::StaticClass()))) {
			DungeonCanvasPtr = Cast<UDungeonCanvasComponent>(Dungeon->GetComponentByClass<UDungeonCanvasComponent>());
		}
	}

	DungeonCanvasViewport->SetDungeonCanvasInstance(DungeonCanvasPtr.Get());
	DungeonCanvasViewport->SetDungeonCanvasTheme(DungeonCanvasTheme);
	return DungeonCanvasViewport.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE

