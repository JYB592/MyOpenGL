#pragma once
#include<set>
#include<string>

using namespace std;

class YYModel;

class YYModelManager
{
public:
	static YYModelManager* GetInstance()
	{
		return modelManager;
	}
	virtual ~YYModelManager();

private:
	static YYModelManager* modelManager;

public:
	void init();
	YYModel* AddModel(string const& path, bool gamma = false);
	void RemoveModel(YYModel* model);
	void Update();
	void Draw();

protected:
	set<YYModel*> Models;
};
