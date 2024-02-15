#pragma once

#include <map>
#include <string>

class UWindowImage;

/// <summary>
/// 이미지, 사운드 관리 Manager
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

	// 싱글톤
	static UEngineResourcesManager& GetInst()
	{
		static UEngineResourcesManager Inst = UEngineResourcesManager();
		return Inst;
	}

	/// <summary>
	/// 호출 되는 LoadImg 함수.
	/// </summary>
	/// <param name="_Path"></param>
	/// <returns></returns>
	UWindowImage* LoadImg(std::string_view _Path);
	UWindowImage* LoadImg(std::string_view _Path, std::string_view _Name);

	/// <summary>
	/// ImageRenderer, CuttingImage 에서 사용.
	/// 이미지 찾기.
	/// </summary>
	/// <param name="_Name">찾을 파일 이름</param>
	/// <returns></returns>
	UWindowImage* FindImg(std::string_view _Name);

	/// <summary>
	/// 이미지를 잘라서 사용할 때 사용하는 함수.
	/// </summary>
	/// <param name="_Name">이미지 이름</param>
	/// <param name="_X">행</param>
	/// <param name="_Y">렬</param>
	void CuttingImage(std::string_view _Name, int _X, int _Y);

private :
	/// <summary>
	/// 읽어온 모든 이미지 리소스들을 담는 map
	/// </summary>
	std::map<std::string, UWindowImage*> Images;
};