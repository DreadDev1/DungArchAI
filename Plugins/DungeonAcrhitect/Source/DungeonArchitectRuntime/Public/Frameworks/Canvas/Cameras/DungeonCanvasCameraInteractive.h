//$ Copyright 2015-25, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//

#pragma once
#include "CoreMinimal.h"
#include "Frameworks/Canvas/Cameras/DungeonCanvasCamera.h"
#include "DungeonCanvasCameraInteractive.generated.h"

UCLASS(Blueprintable)
class UDungeonCanvasCameraInteractive : public UDungeonCanvasCamera {
	GENERATED_BODY()
public:
	virtual void Update_Implementation(UDungeonCanvasComponent* DungeonCanvas, float DeltaSeconds) override;
};

