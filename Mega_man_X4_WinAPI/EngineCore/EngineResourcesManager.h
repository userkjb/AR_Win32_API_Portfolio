#pragma once

#include <map>
#include <string>

class UWindowImage;

/// <summary>
/// �̹���, ���� ���� Manager
/// </summary>
class UEngineResourcesManager
{
private:
	UEngineResourcesManager();
	~UEngineResourcesManager();

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

	/// <summary>
	/// ȣ�� �Ǵ� LoadImg �Լ�.
	/// </summary>
	/// <param name="_Path"></param>
	/// <returns></returns>
	UWindowImage* LoadImg(std::string_view _Path);
	UWindowImage* LoadImg(std::string_view _Path, std::string_view _Name);

	/// <summary>
	/// ImageRenderer���� ���.
	/// </summary>
	/// <param name="_Name">ã�� ���� �̸�</param>
	/// <returns></returns>
	UWindowImage* FindImg(std::string_view _Name);

private :
	/// <summary>
	/// �о�� ��� �̹��� ���ҽ����� ��� map
	/// </summary>
	std::map<std::string, UWindowImage*> Images;
};