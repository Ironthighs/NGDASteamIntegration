// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class NGDA_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION(BlueprintCallable, Category = "Online")
	bool OnClickCreateGame();
	
	UFUNCTION(BlueprintCallable, Category = "Online")
	bool OnClickFindSession();

	UFUNCTION(BlueprintCallable, Category = "Online")
	bool OnClickJoinGame();

	class ANGDAGameSession* GetGameSession();

	void FindSessionComplete(bool wasSuccessful);

};
