#pragma once
class GameInstance
{
public:
	static GameInstance* GetInstance()
	{
		if (gameInstance == nullptr)
		{
			gameInstance = new GameInstance();
		}
		return gameInstance;
	}
	static GameInstance* gameInstance;

	void Init();
	void Update();
	void LateUpdate();
};

