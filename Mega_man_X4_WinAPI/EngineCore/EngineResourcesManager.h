#pragma once

#include <map>
#include <string>

class UWindowImage;

/// <summary>
/// �̹���, ���� ���� Manager
/// </summary>
class UEngineResourcesManager
{
public :
	// delete Function
	UEngineResourcesManager(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager(UEngineResourcesManager&& _Other) noexcept = delete;
	UEngineResourcesManager& operator=(const UEngineResourcesManager& _Other) = delete;
	UEngineResourcesManager& operator=(UEngineResourcesManager&& _Other) noexcept = delete;

	// �̱���
	static UEngineResourcesManager& GetInst()
	{
		static UEngineResourcesManager Inst = UEngineResourcesManager();
		return Inst;
	}

private :
	UEngineResourcesManager();
	~UEngineResourcesManager();

	/// <summary>
	/// �о�� ��� �̹��� ���ҽ����� ��� map
	/// </summary>
	std::map<std::string, UWindowImage*> Images;
};