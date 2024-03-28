workspace "AnimationProject"
    architecture "x86_64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

    flags 
    {
        "MultiProcessorCompile"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
    }

    sourcedir = "./src"
    vendordir = "./vendor"
    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

    group "Dependencies"
        include "Animation-Engine/vendor/glad"
        include "Animation-Engine/vendor/GLFW"
    group ""

    group "Core"
        include "Animation-Engine"
    group ""

    group "Misc"
        include "Sandbox"
    group ""

newaction
{
    trigger = "clean",
    description = "Remove all binaries, intermediate binaries, and VS Files.",
    execute = function()
        print("Removing binaries & intermediate binaries")
        os.rmdir("./Animation-Engine/build")
        os.rmdir("./Sandbox/build")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
    end
}