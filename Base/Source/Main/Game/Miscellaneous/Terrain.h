#include <string>
#include <map>

class Terrain
{
private:
	
	std::string EntityID;
	std::string TerrainName;
	int TerrainType;

	std::map<int, float> UnitAdvantage;
	std::map<int, float> UnitDisadvantage;
public:
	enum TerrainStatEnum
	{
		T_HEALTH = 0,
		T_MELEE,
		T_MAGIC,
		T_RANGE,
		T_MAX,
	};
	Terrain()
	{

	}
	~Terrain()
	{

	}

	std::string GetEntityID()
	{
		return EntityID;
	}
	std::string GetTerrainName()
	{
		return TerrainName;
	}
	int GetTerrainType()
	{
		return TerrainType;
	}

	float GetUnitAdvantage(int stat)
	{
		return UnitAdvantage.find(stat)->second;
	}

	float GetUnitDisadvantage(int stat)
	{
		return UnitDisadvantage.find(stat)->second;
	}

	void SetEntityID(std::string EntityID)
	{
		this->EntityID = EntityID;
	}
	void SetTerrainName(std::string TerrainName)
	{
		this->TerrainName = TerrainName;
	}
	void SetTerrainType(int TerrainType)
	{
		this->TerrainType = TerrainType;
	}

	void SetUnitAdvantage(int stat, float value)
	{
		UnitAdvantage.insert(std::pair<int, float>(stat,value));
	}

	void SetUnitDisadvantage(int stat, float value)
	{
		UnitDisadvantage.insert(std::pair<int, float>(stat, value));
	}

};
