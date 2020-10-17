#include <stdafx.h>
#include <astc-transcoder/command_line_parser.h>
#include <astc-transcoder/crash_report.h>
#include <astc-transcoder/transcoder.h>
#include <astc-transcoder/version.h>


namespace astc_transcoder {

[[nodiscard]] static int OnHelp ()
{
    constexpr char const man[] =
#include <astc-transcoder/help.h>
    ;

    std::cout << man;
    return EXIT_SUCCESS;
}

[[nodiscard]] static int OnTranscode ( uint8_t blockWidth,
    uint8_t blockHeight,
    char const* inputFile,
    char const* outputFile
)
{
    Transcoder const transcoder ( blockWidth, blockHeight, inputFile, outputFile );
    return transcoder.Run ();
}

[[nodiscard]] static int OnVersion ()
{
    std::cout << ASTC_TRANSCODER_VERSION_MAJOR << '.'
        << ASTC_TRANSCODER_VERSION_MINOR << '.'
        << ASTC_TRANSCODER_VERSION_RELEASE << '.'
        << ASTC_TRANSCODER_VERSION_BUILD;

    return EXIT_SUCCESS;
}

} // namespace astc_transcoder

int main ( int argc, char** argv )
{
    if ( !astc_transcoder::InstallCrashReport () )
        return EXIT_FAILURE;

    astc_transcoder::CommandLineParser const parser ( argc,
        argv,
        &astc_transcoder::OnHelp,
        &astc_transcoder::OnTranscode,
        &astc_transcoder::OnVersion
    );

    return parser.Run ();
}
