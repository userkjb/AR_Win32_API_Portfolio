#pragma once
#include "EnginePath.h"
#include <list>

class UEngineFile;

class UEngineDirectory : public UEnginePath
{
public :
	UEngineDirectory();
	/// <summary>
	/// ��θ� UEnginePath�� �ѱ�� ������.
	/// </summary>
	/// <param name="_Path">���� ���</param>
	UEngineDirectory(const UEnginePath& _Path);
	~UEngineDirectory();

	/// <summary>
	/// �ܺο��� ����� ���ϵ��� ã�� �Լ�.
	/// </summary>
	/// <param name="_Ext">Ȯ���ڵ�. ��/�ҹ��� ������ ����.</param>
	/// <param name="_Recursive">���� ���� ���� �� �� ������?</param>
	/// <returns></returns>
	std::list<UEngineFile> AllFile(
		std::vector<std::string> _Ext = std::vector<std::string>(),
		bool _Recursive = false);

	/// <summary>
	/// ��� ã�Ƽ� �ø���.
	/// </summary>
	/// <param name="_Path"></param>
	void MoveToSearchChild(std::string_view _Path);

	// �밡�� ���
	std::list<UEngineDirectory> AllDirectory(bool _Recursive = false);

	UEngineFile NewFile(std::string_view FileName);

private :

	void AllDirectoryRecursive(const std::string_view _Path, std::list<UEngineDirectory>& _Result, bool _Recursive = false);

	/// <summary>
	/// Ŭ���� ���ο��� ��¥�� ������ ã�� �Լ�.
	/// ã�Ƽ� List(UEngineFile)�� ����.
	/// </summary>
	/// <param name="_Path">���</param>
	/// <param name="_Result">��� ��� �ִ� list</param>
	/// <param name="_Ext">Ȯ����</param>
	/// <param name="_Recursive">���� ���� ���� �� �� ������?</param>
	void AllFileRecursive(
		const std::string_view _Path, 
		std::list<UEngineFile>& _Result, 
		std::vector<std::string> _Ext = std::vector<std::string>(), 
		bool _Recursive = false);
};