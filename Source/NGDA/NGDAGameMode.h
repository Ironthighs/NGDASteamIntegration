// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "NGDAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NGDA_API ANGDAGameMode : public AGameMode
{
	GENERATED_BODY()
	

public:

	ANGDAGameMode(const FObjectInitializer& ObjectInitializer);


	virtual TSubclassOf<AGameSession> GetGameSessionClass() const override;
	
	
};
