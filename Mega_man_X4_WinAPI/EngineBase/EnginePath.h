#pragma once
#include <string>
#include <filesystem>

class UEnginePath
{
public :
	UEnginePath();
	UEnginePath(std::filesystem::path _Path);
	~UEnginePath();

	bool IsFile();
	bool IsDirectory();
	bool IsExists();

	void MoveParent();
	void Move(std::string_view _Path);

	/// <summary>
	/// 파일 이름 가져오기.
	/// </summary>
	/// <returns></returns>
	std::string GetFileName() const;
	/// <summary>
	/// 파일 확장자 가져오기.
	/// </summary>
	/// <returns></returns>
	std::string GetExtension() const;
	/// <summary>
	/// 파일 경로 가져오기
	/// </summary>
	/// <returns></returns>
	std::string GetFullPath() const;

protected :
	std::filesystem::path Path;
};