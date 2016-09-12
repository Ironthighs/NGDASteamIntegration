// Fill out your copyright notice in the Description page of Project Settings.

#include "NGDA.h"
#include "NGDAGameSession.h"
#include "NGDAGameMode.h"


ANGDAGameMode::ANGDAGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


TSubclassOf<class AGameSession> ANGDAGameMode::GetGameSessionClass() const
{
	return ANGDAGameSession::StaticClass();
}
