R"__(>>> Format:
    astc-transcoder <command> [parameters]

>>> Supported commands:
    --help - The command prints man page. The command has no parameters.
    --version - The command prints tool version. The command has no parameters.
    --transcode - The command perform transcode operation. The command has parameters.

>>> "--transcode" parameters:
    --block-width <integer value> - ASTC block width in texels. Required parameter.
    --block-height <integer value> - ASTC block height in texels. Required parameter.
    --in <file name> - input image file. Required parameter.
    --out <file name> - output image file. Required parameter.

>>> Supported ASTC block sizes [--block-width]x[--block-height]:
    4x4
    5x4
    5x5
    6x5
    6x6
    8x5
    8x6
    8x8
    10x5
    10x6
    10x8
    10x10
    12x10
    12x12

>>> CompressonatorCLI dependency:
    The tool is using CompressonatorCLI dependency with version 4.0.4855.0. So user have to define
    the environment variable with name COMPRESSONATOR_PATH which will contain the directory to the
    CompressonatorCLI.exe.

    For example:
        COMPRESSONATOR_PATH = D:\Programs\Compressonator_4.0.4855\bin\CLI

>>> Examples:
    astc-transcoder --help
    astc-transcoder --version
    astc-transcoder --transcode --block-width 6 --block-height 6 --in texture_D.dds --out texture_D.ktx
)__"
