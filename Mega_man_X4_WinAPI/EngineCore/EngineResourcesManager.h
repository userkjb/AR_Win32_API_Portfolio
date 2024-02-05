#pragma once

/// <summary>
/// 이미지, 사운드 관리 Manager
/// </summary>
class UEngineResourcesManager
{
public :
	// delete Function
	UEngineResourcesManager(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager(UEngineResourcesManager&& _Other) noexcept = delete;
	UEngineResourcesManager& operator=(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager& operator=(UEngineResourcesManager&& _Other) noexcept = delete;

private :
	UEngineResourcesManager();
	~UEngineResourcesManager();
};