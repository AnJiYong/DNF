#pragma once
#include <vector>
#include <map>

class GameEngineInput
{
private:
    class GameEngineKey 
    {
        friend GameEngineInput;

    private:
        GameEngineKey();
        ~GameEngineKey();

    private:
        bool KeyCheck();
        void Update();
        void Reset();

    public:
        void PushKey(int _CurKey)
        {
            CheckKey_.push_back(_CurKey);
        }

    private:
        bool Down_;
        bool Press_;
        bool Up_;
        bool Free_;
        std::vector<int> CheckKey_;
    };

private:
    static GameEngineInput* Inst;

public:
    static GameEngineInput& GetInst()
    {
        return *Inst;
    }

    static void Destroy()
    {
        if (nullptr != Inst)
        {
            delete Inst;
            Inst = nullptr;
        }
    }

public:
    GameEngineInput();
    ~GameEngineInput();
    GameEngineInput(const GameEngineInput& _Other) = delete;
    GameEngineInput(const GameEngineInput&& _Other) = delete;

public:
    GameEngineInput operator=(const GameEngineInput& _Other) = delete;
    GameEngineInput operator=(const GameEngineInput&& _Other) = delete;


private:
    GameEngineKey* FindKey(const std::string& _KeyName);

public:
    static bool IsKey(const std::string& _Name);
    static bool Down(const std::string& _Name);
    static bool Up(const std::string& _Name);
    static bool Press(const std::string& _Name);
    static bool Free(const std::string& _Name);

    static void HideCursor()
    {
        ShowCursor(false);
    }

    static float4 GetMousePos();
    static float4 GetMouse3DPos();
    static float4 GetMouse3DDir();

    void CreateKey(const std::string& _KeyName, int _Key)
    {
        if (nullptr != FindKey(_KeyName))
        {
            assert("if (nullptr != FindKey(_KeyName))");
            return;
        }

        if ('a' <= _Key && 'z' >= _Key)
        {
            _Key = std::toupper(_Key);
        }

        GameEngineKey* NewKey = new GameEngineKey();
        NewKey->Reset();
        NewKey->PushKey(_Key);
        AllKey_.insert(std::map<std::string, GameEngineKey*>::value_type(_KeyName, NewKey));
    }

    void Update();

private:
    std::map<std::string, GameEngineKey*> AllKey_;

    float4 MousePos_;
    float4 MousePos3D_;
    float4 PrevMousePos3D_;
    float4 MouseDir3D_;
};

