
#include "LSWebpDisplaySubSystem.h"
#include "LSWebpCore.h"
#include "Engine/World.h"
#include "Async/Async.h"

ULSWebpDisplaySubSystem::ULSWebpDisplaySubSystem()
{
}

bool ULSWebpDisplaySubSystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;

}

void ULSWebpDisplaySubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);




}

void ULSWebpDisplaySubSystem::Deinitialize()
{
	ReleaseLoadedWebp();
	Super::Deinitialize();

}

void ULSWebpDisplaySubSystem::Tick(float DeltaTime)
{
	if (LoadAndShowStatus == ELSWebpLoadAndShowType::Showing && WebpTexture)
	{
		DeltaTime *= 1000;
		//TODO,cost much
		for (int32 WebpIndex = 0; WebpIndex < WebpTimestepMillisecond.Num(); WebpIndex++)
		{
			if (WebpIndex == 0 && CurrentMillisecond < WebpTimestepMillisecond[WebpIndex])
			{
				CurrentMillisecond += DeltaTime;
				WebpShowIndex = WebpIndex;
				break;
			}

			if (WebpIndex == WebpTimestepMillisecond.Num() - 1 && CurrentMillisecond > WebpTimestepMillisecond[WebpIndex])
			{
				CurrentMillisecond = 0;
				WebpShowIndex = WebpIndex;
				break;
			}

			if (CurrentMillisecond > WebpTimestepMillisecond[WebpIndex])
			{
				continue;
			}

			CurrentMillisecond += DeltaTime;
			WebpShowIndex = WebpIndex;

			break;

		}

		/*
				int32 DataSize = sizeof(FColor);
				FUpdateTextureRegion2D  UpdateRegion;
				UpdateRegion.SrcX=0;
				UpdateRegion.SrcY = 0;
				UpdateRegion.DestX = 0;
				UpdateRegion.DestY = 0;
				UpdateRegion.Width = WebpWidth;
				UpdateRegion.Height = WebpHeight;

				WebpTexture->UpdateTextureRegions(0, 1, &UpdateRegion,  WebpWidth* DataSize, DataSize,(uint8*)PicturesColors[WebpShowIndex].GetData());

		*/


		uint8* textureData = static_cast<uint8*>(WebpTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
		FMemory::Memcpy(textureData, PicturesColors[WebpShowIndex].GetData(), WebpWidth * WebpHeight * 4);

		WebpTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		WebpTexture->UpdateResource();

	}

}

TStatId ULSWebpDisplaySubSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(ULSWebpDisplaySubSystem, STATGROUP_Tickables);
}

bool ULSWebpDisplaySubSystem::IsTickable() const
{
	return !IsTemplate();
}

void ULSWebpDisplaySubSystem::LoadWebp
(FLSWebpLoadAndShowWebp InLoadAndShowEbpDegelete,
	FString InWebpFilePath)
{

	bool bRightPath = FLSWebpCore::CheckWebpPicturePath(InWebpFilePath);
	bool bRgihtStatus = LoadAndShowStatus ==ELSWebpLoadAndShowType::None;
	if (!bRightPath|| !bRgihtStatus)
	{
		InLoadAndShowEbpDegelete.ExecuteIfBound(false, nullptr, 0, 0);
	}



	LoadAndShowStatus = ELSWebpLoadAndShowType::Loading;
	LSWebpLoadAndShowWebp = InLoadAndShowEbpDegelete;

	ULSWebpDisplaySubSystem* LSWebpDisplaySubSystem = this;

	AsyncTask(ENamedThreads::AnyThread,[InWebpFilePath,this, LSWebpDisplaySubSystem](){
	
		//此处需要异步
		bool bLoadWebp = FLSWebpCore::LoadDynamicWebpPicture(InWebpFilePath, WebpTimestepMillisecond, PicturesColors, WebpWidth, WebpHeight);

		//此处需要返回主线程
		AsyncTask(ENamedThreads::GameThread,[bLoadWebp, LSWebpDisplaySubSystem](){
		
			LSWebpDisplaySubSystem->LoadedWebpCallShow(bLoadWebp);

		});

		
	
	
	});



}

void ULSWebpDisplaySubSystem::ReleaseLoadedWebp()
{
	switch (LoadAndShowStatus)
	{
	case ELSWebpLoadAndShowType::None:
		break;
	case ELSWebpLoadAndShowType::Loading:

		AsyncTask(ENamedThreads::GameThread,[this](){
		
			ReleaseLoadedWebp();
		
		});

		break;
	case ELSWebpLoadAndShowType::Showing:
		
		LoadAndShowStatus = ELSWebpLoadAndShowType::None;
		WebpTexture = nullptr;
		WebpShowIndex = -1;
		CurrentMillisecond = -1;
		WebpWidth = 0;
		WebpHeight = 0;
		WebpTimestepMillisecond.Empty();
		for (auto& Tmp : PicturesColors)
		{
			Tmp.Empty();
		}
		PicturesColors.Empty();

		break;
	case ELSWebpLoadAndShowType::Max:
		break;
	default:
		break;
	}

}

void ULSWebpDisplaySubSystem::LoadedWebpCallShow(bool bLoaded)
{

	if (bLoaded)
	{
		LoadAndShowStatus=ELSWebpLoadAndShowType::Showing;

		WebpTexture = UTexture2D::CreateTransient(WebpWidth, WebpHeight, EPixelFormat::PF_R8G8B8A8);
		uint8* textureData = static_cast<uint8*>(WebpTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
		FMemory::Memcpy(textureData, PicturesColors[0].GetData(), WebpWidth * WebpHeight * 4);
		WebpShowIndex = 0;
		WebpTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
		WebpTexture->UpdateResource();
		LSWebpLoadAndShowWebp.ExecuteIfBound(true, WebpTexture, WebpWidth, WebpHeight);
	}
	else
	{
		LoadAndShowStatus = ELSWebpLoadAndShowType::None;
		LSWebpLoadAndShowWebp.ExecuteIfBound(false, nullptr, 0, 0);


	}

}


