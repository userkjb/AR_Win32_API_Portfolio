#pragma once
#include "EnginePath.h"
#include <list>

class UEngineFile;

class UEngineDirectory : public UEnginePath
{
public :
	UEngineDirectory();
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

private :
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