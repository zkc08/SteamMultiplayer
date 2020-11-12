// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    bUseSeamlessTravel = true;
    Super::PostLogin(NewPlayer);
    ++NumberOfPlayers;

    UWorld* World = GetWorld();
    if(!ensure(World != nullptr))return;
    if (NumberOfPlayers >= 2)
    {
        World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
    } 
}

void ALobbyGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);
    --NumberOfPlayers;    
}
