import os

Import("env")

board = env.BoardConfig()

BOARD = 'STM32F103C6'
BOARD_FAMILY = 'STM32F1'

PROJECT_DIR = env["PROJECT_DIR"]
SRC_DIR = "src"
LIB_DIR = "lib"

def join(x, *args):
    return x + "".join(["/" + x for x in args])

BOARD_SRC_DIR = join(SRC_DIR, BOARD)
BOARD_DRIVER_DIR = join("CubeMx", BOARD)

RELATIVE_SRC_DIR = os.path.relpath(SRC_DIR)

build_flags = [
    join(BOARD_DRIVER_DIR, "Core", "Inc"),
    join(BOARD_DRIVER_DIR, "Drivers", "CMSIS", "Include"),
    join(BOARD_DRIVER_DIR, "Drivers", "CMSIS", "Device", "ST", f'{BOARD_FAMILY}xx', "Include"),
    join(BOARD_DRIVER_DIR, "Drivers", f'{BOARD_FAMILY}xx_HAL_Driver', "Inc"),
    join(BOARD_DRIVER_DIR, "Drivers", f'{BOARD_FAMILY}xx_HAL_Driver', "Inc", "Legacy"),
    BOARD_SRC_DIR,
    SRC_DIR
]

src_filter = [
    join(BOARD_DRIVER_DIR, "Drivers", f'{BOARD_FAMILY}xx_HAL_Driver', "Src"),
    join(BOARD_DRIVER_DIR, "Core", "Src"),
    BOARD_SRC_DIR,
    SRC_DIR
]

lib_dirs = [x.name for x in os.scandir(join(os.getcwd(), LIB_DIR)) if "DS_Store" not in x.name]

build_flags = build_flags + [join(LIB_DIR, x) for x in lib_dirs]
src_filter = src_filter + [join(LIB_DIR, x) for x in lib_dirs]

src_filter = ["+<{0}>".format(join("..", x)) for x in src_filter]
build_flags = ["-I./" + x for x in build_flags]

env.Append(
    SRC_FILTER=src_filter,
    BUILD_FLAGS=build_flags
)

env.Replace(
    LDSCRIPT_PATH=join(".", BOARD_DRIVER_DIR, f'{BOARD}TX_FLASH.ld')
)

product_line = board.get("build.product_line")
env.Append(BUILD_FLAGS=[f'-D{product_line}'])