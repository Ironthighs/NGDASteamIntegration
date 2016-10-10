// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class NGDA : ModuleRules
{
	public NGDA(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateDependencyModuleNames.Add("OnlineSubsystem");
    }
}
