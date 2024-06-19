// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorComponent.h"

// Sets default values for this component's properties
UOpenDoorComponent::UOpenDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// vado a catturare il pawn attuale del giocatore
	Chiave = GetWorld()->GetFirstPlayerController()->GetPawn();

	//memorizzo la rotazione iniziale
	// in caso di successiva chiusura della porta
	// ma anche per sapere quando ho raggiunto
	//l'apertura desiderata
	if (!AntaDaAprire) { UE_LOG(LogTemp, Error, TEXT("Manca Anta")); return; }
	StartRotation = AntaDaAprire->GetComponentRotation();
   //-180/180  360 / - 360

}


void UOpenDoorComponent::OpenDoor(float DT)
{

	FRotator ActualRot = AntaDaAprire->GetComponentRotation();

	//if (ActualRot.Yaw > StartRotation.Yaw - OpenDeg)
	if (OffsetDeg < OpenDeg)
	{
		ActualRot.Yaw -= OpenDeg * DT;
		OffsetDeg     += OpenDeg * DT;
		AntaDaAprire->SetWorldRotation(ActualRot);
	}
	//else bClose = false;
	
}


void UOpenDoorComponent::CloseDoor(float DT)
{

	FRotator ActualRot = AntaDaAprire->GetComponentRotation();

	//if (ActualRot.Yaw < StartRotation.Yaw)
	if (OffsetDeg > 0)
	{
		ActualRot.Yaw += OpenDeg * DT;	
		OffsetDeg     -= OpenDeg * DT;
		AntaDaAprire->SetWorldRotation(ActualRot);
	}
//	else bClose = true;

}


// Called every frame
void UOpenDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
	if (!Chiave || !Attivatore) return;
//	UE_LOG(LogTemp, Error, TEXT("TICK 2 "));

	//GetWorld()->GetTimerManager().SetTimer

	if (Attivatore->IsOverlappingActor(Chiave))
	{
		bOpening = true;
		//if (bClose)  OpenDoor(DeltaTime);	
	}
	else
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(CloseTimer))
		{
			GetWorld()->GetTimerManager().SetTimer
			(CloseTimer, this, &UOpenDoorComponent::ResetClose, DelayClose);
		}
		//if (!bClose) CloseDoor(DeltaTime);
	}



	if (bOpening)
	{ 
	OpenDoor(DeltaTime);
	//UE_LOG(LogTemp, Error, TEXT("APRO"));
	}
	else 
	{ 
	CloseDoor(DeltaTime);       
	//UE_LOG(LogTemp, Error, TEXT("CHIUDO"));
	};



	// ...
}

void UOpenDoorComponent::ResetClose()
{
	bOpening = false;

}