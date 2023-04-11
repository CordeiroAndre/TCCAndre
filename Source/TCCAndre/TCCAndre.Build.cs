// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TCCAndre : ModuleRules
{
	public TCCAndre(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {"RuntimeAudioImporter"});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "CommonUI", "RuntimeSpeechRecognizer", "EnhancedInput", "HTTP", "JsonUtilities", "Json"});
	}
}
