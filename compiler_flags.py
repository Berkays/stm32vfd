Import("env")

board = env.BoardConfig()

env.Replace(AS="$CC", ASCOM="$ASPPCOM")

env.Append(
    ASFLAGS=["-x", "assembler-with-cpp"],

    CCFLAGS=[
        "-O3",
        "-ffunction-sections",
        "-fdata-sections",
        "--gc-sections",
        "-Wall",
        "-fno-common",
        "-mthumb",
        "-nostdlib",
        "-fno-exceptions",
        "-fno-rtti",
        "USE_HAL_DRIVER"
    ],

    LINKFLAGS=[
        "-mthumb",
        "--specs=nano.specs",
        "--specs=nosys.specs"
    ],

    LIBS=["c", "gcc", "m", "stdc++", "nosys"],
)

# copy CCFLAGS to ASFLAGS (-x assembler-with-cpp mode)
env.Append(ASFLAGS=env.get("CCFLAGS", [])[:])