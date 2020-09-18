// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

const static FName SESSION_NAME = TEXT("My Game Session");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitializer) 
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSysytem/UI/WBP_MainMenu"));

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSysytem/UI/WBP_InGameMenu"));

    if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
    InGameMenuClass = InGameMenuBPClass.Class;

    if (!ensure(MenuBPClass.Class != nullptr)) return;
    MenuClass = MenuBPClass.Class;
    
}

void UPuzzlePlatformGameInstance::Init() 
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    UE_LOG(LogTemp, Display, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
	if (Subsystem != nullptr) 
    {
        SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid())
        {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);       
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);
        }
	}
	else 
    {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem NULL"));
	}
}


void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) 
{
    if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

    if (Menu != nullptr)
    {
        Menu->TearDown();
    }
    
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    GetWorld()->ServerTravel(TEXT("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen"));    
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) 
{
    if (Success)
    {
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::RefreshServerList() 
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    
    if (SessionSearch.IsValid())
    {
        SessionSearch->bIsLanQuery = true;
        UE_LOG(LogTemp, Error, TEXT("Starting to find sessions"));    
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }  
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success) 
{ 
    if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Finished Find Session"));

        TArray<FString> ServerNames;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Error, TEXT("Found sessions : %s"), *SearchResult.GetSessionIdStr());
            ServerNames.Add(SearchResult.GetSessionIdStr());
		}

        Menu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) 
    {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformGameInstance::Host() 
{
    if (SessionInterface.IsValid())
    {
        auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
        if (ExistingSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession();
        }
    }
    
}

void UPuzzlePlatformGameInstance::CreateSession() 
{
    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = true;
        SessionSettings.NumPublicConnections = 2;
        SessionSettings.bShouldAdvertise = true;
        SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
    }
}

void UPuzzlePlatformGameInstance::Join(uint32 Index)
{

    if (!SessionInterface.IsValid()) return;
    if (!SessionSearch.IsValid()) return;

    if (Menu != nullptr)
    {
        Menu->TearDown();
    }

    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformGameInstance::LoadMenu() 
{    

    if (!ensure(MenuClass != nullptr)) return;

    Menu = CreateWidget<UMainMenu>(this, MenuClass);
    if (!ensure(Menu != nullptr)) return;

    Menu->Setup();

    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::LoadInGameMenu() 
{
    if (!ensure(InGameMenuClass != nullptr)) return;
    
    InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
    if (!ensure(InGameMenu != nullptr)) return;

    InGameMenu->Setup();

    InGameMenu->SetMenuInterface(this); 
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSysytem/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::QuitGame() 
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand(TEXT("Quit"), true);
}
