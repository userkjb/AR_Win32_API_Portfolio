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
	/// 외부에서 사용할 파일들을 찾는 함수.
	/// </summary>
	/// <param name="_Ext">확장자들. 대/소문자 가리지 않음.</param>
	/// <param name="_Recursive">하위 폴더 까지 더 볼 것인지?</param>
	/// <returns></returns>
	std::list<UEngineFile> AllFile(
		std::vector<std::string> _Ext = std::vector<std::string>(),
		bool _Recursive = false);

private :
	/// <summary>
	/// 클래스 내부에서 진짜로 파일을 찾는 함수.
	/// 찾아서 List(UEngineFile)에 담음.
	/// </summary>
	/// <param name="_Path">경로</param>
	/// <param name="_Result">경로 담겨 있는 list</param>
	/// <param name="_Ext">확장자</param>
	/// <param name="_Recursive">하위 폴더 까지 더 볼 것인지?</param>
	void AllFileRecursive(
		const std::string_view _Path, 
		std::list<UEngineFile>& _Result, 
		std::vector<std::string> _Ext = std::vector<std::string>(), 
		bool _Recursive = false);
};