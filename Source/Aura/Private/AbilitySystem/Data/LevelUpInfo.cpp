// Copyright Druid Mechanics


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 Experience) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information, LevelUpInformation[2] = Level 2 Information, etc
		if (LevelUpInformation.Num() - 1 <= Level) return Level; 

		if (Experience >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else bSearching = false; 
	}
	return Level;
}
