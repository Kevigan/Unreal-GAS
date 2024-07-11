// Artic Mango Gaming.


#include "AbilitySystem/Abilities/DashAbility.h"

FString UDashAbility::GetDescription(int32 Level)
{
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>DASH</>\n\n"

			//Level
			"<Small>Level: </><Level>%d</>\n"
			
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Dashing, "
			"Dash to a distanced location! </>"

		),

			//Values
			Level,
			Cooldown
		);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>DASH</>\n\n"

			//Level
			"<Small>Level: </><Level>%d</>\n"
			
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Dashing, "
			"Dash to a distanced location and someting else! </>"
			
		),

			//Values
			Level,
			Cooldown
		);
	}
}

FString UDashAbility::GetNextLevelDescription(int32 Level)
{
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		//Title
		"<Title>Next Level: </>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		//Description
		"<Default>Dashing, "
		"Dash big! </>"
	),

		//Values
		Level,
		Cooldown
	);
}
