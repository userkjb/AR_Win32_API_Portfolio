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
	/// LoadFolder �Լ��� ����� �� ���.
	/// </summary>
	/// <param name="_Path"></param>
	/// <returns></returns>
	std::string AppendPath(std::string_view _Path);

	/// <summary>
	/// ���� �̸� ��������.
	/// </summary>
	/// <returns></returns>
	std::string GetFileName() const;
	/// <summary>
	/// ���� Ȯ���� ��������.
	/// </summary>
	/// <returns></returns>
	std::string GetExtension() const;
	/// <summary>
	/// ���� ��� ��������
	/// </summary>
	/// <returns></returns>
	std::string GetFullPath() const
	{
		return Path.string();
	}

protected :
	std::filesystem::path Path;
};