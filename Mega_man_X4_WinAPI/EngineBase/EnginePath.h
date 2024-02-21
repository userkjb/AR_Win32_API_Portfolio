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
	/// LoadFolder 함수를 사용할 때 사용.
	/// </summary>
	/// <param name="_Path"></param>
	/// <returns></returns>
	std::string AppendPath(std::string_view _Path);

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
	std::string GetFullPath() const
	{
		return Path.string();
	}

protected :
	std::filesystem::path Path;
};