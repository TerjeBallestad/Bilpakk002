// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"

UActorPool::UActorPool()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < PreWarmed; ++i)
	{
		InitializeNewPooledActor();
	}
}

UStaticMeshComponent* UActorPool::GetPooledActor()
{
	if(ActorPool.Num() < 1)
	{
		InitializeNewPooledActor();
	}
	UStaticMeshComponent* NewActor = ActorPool.Pop();
	if(!NewActor) return nullptr;
	NewActor->SetVisibility(true);
	NewActor->Activate();
	ActiveActors.Add(NewActor);
	
	return NewActor;
}

void UActorPool::InitializeNewPooledActor()
{
	UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(GetOwner());
	NewComponent->RegisterComponent();
	NewComponent->SetVisibility(false);
	NewComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActorPool.Add(NewComponent);
}

void UActorPool::ReturnPooledActors()
{
	for (auto Actor : ActiveActors)
	{
		Actor->SetVisibility(false);
		Actor->Deactivate();
		Actor->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ActorPool.Add(Actor);
	}
	ActiveActors.Empty();
}


