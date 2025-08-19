// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestForMinigame : ModuleRules
{
	public TestForMinigame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TestForMinigame",
			"TestForMinigame/Variant_Platforming",
			"TestForMinigame/Variant_Platforming/Animation",
			"TestForMinigame/Variant_Combat",
			"TestForMinigame/Variant_Combat/AI",
			"TestForMinigame/Variant_Combat/Animation",
			"TestForMinigame/Variant_Combat/Gameplay",
			"TestForMinigame/Variant_Combat/Interfaces",
			"TestForMinigame/Variant_Combat/UI",
			"TestForMinigame/Variant_SideScrolling",
			"TestForMinigame/Variant_SideScrolling/AI",
			"TestForMinigame/Variant_SideScrolling/Gameplay",
			"TestForMinigame/Variant_SideScrolling/Interfaces",
			"TestForMinigame/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
