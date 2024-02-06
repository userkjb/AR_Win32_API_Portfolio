#pragma once

#include <map>
#include <string>

class UWindowImage;

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

	// 싱글톤
	static UEngineResourcesManager& GetInst()
	{
		static UEngineResourcesManager Inst = UEngineResourcesManager();
		return Inst;
	}

private :
	UEngineResourcesManager();
	~UEngineResourcesManager();

	/// <summary>
	/// 읽어온 모든 이미지 리소스들을 담는 map
	/// </summary>
	std::map<std::string, UWindowImage*> Images;
};