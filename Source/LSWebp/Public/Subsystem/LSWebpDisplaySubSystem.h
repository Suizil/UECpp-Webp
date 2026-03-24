
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LSWebpType.h"
#include "Tickable.h"
#include "Engine/Texture2D.h"
#include "LSWebpDisplaySubSystem.generated.h"



/**
 * Internal use,maybe you can use FLSwebpCore API or LSWebpLibrary.
 */
UCLASS()
class  ULSWebpDisplaySubSystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	friend class ULSWebpBPLibrary;

	GENERATED_BODY()

public:
	ULSWebpDisplaySubSystem();

	void LoadedWebpCallShow(bool bLoaded);
public:


	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:


	virtual void    Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool    IsTickable() const override;

protected:


	void LoadWebp(FLSWebpLoadAndShowWebp InLoadAndShowEbpDegelete, FString InWebpFilePath);
	void ReleaseLoadedWebp();

	UPROPERTY()
		UTexture2D* WebpTexture = nullptr;

	ELSWebpLoadAndShowType LoadAndShowStatus = ELSWebpLoadAndShowType::None;
	FLSWebpLoadAndShowWebp LSWebpLoadAndShowWebp;



	int32 WebpShowIndex = -1;
	int32 CurrentMillisecond = -1;

	TArray<int32> WebpTimestepMillisecond;
	TArray<TArray<FColor>> PicturesColors;

	int32 WebpWidth = 0;
	int32 WebpHeight = 0;




};