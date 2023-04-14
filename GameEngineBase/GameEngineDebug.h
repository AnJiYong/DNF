#pragma once
#include <string>

class GameEngineDebug
{ 
public:
	GameEngineDebug(); 
	~GameEngineDebug();

public:
	static void LeakCheckOn();
	static void MsgBoxError(const std::string& _Text);
	static void AssertFalse();
	static void OutPutDebugString(const std::string& _Text);
	static void MsgBox(const std::string& _Ptr);
};

