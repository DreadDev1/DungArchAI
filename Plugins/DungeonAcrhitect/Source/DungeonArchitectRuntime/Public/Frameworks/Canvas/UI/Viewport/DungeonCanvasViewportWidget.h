//$ Copyright 2015-25, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#pragma once
#include "CoreMinimal.h"
#include "Frameworks/Canvas/DungeonCanvas.h"

#include "Blueprint/UserWidget.h"
#include "DungeonCanvasViewportWidget.generated.h"

class SDungeonCanvasViewport;
class SWidget;

UCLASS(HideDropdown)
class DUNGEONARCHITECTRUNTIME_API UDungeonCanvasViewportWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Expose the DrawSettings attribute to UMG */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Canvas")
	FDungeonCanvasDrawSettings DrawSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category="Dungeon Canvas")
	TObjectPtr<UDungeonCanvasCamera> Camera = nullptr;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category="Dungeon Canvas")
	TObjectPtr<UDungeonCanvasMaterialTheme> DungeonCanvasTheme;
	
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TSharedPtr<SDungeonCanvasViewport> DungeonCanvasViewport;
	TWeakObjectPtr<UDungeonCanvasComponent> DungeonCanvasPtr;
};

