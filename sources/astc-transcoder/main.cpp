#include <stdafx.h>
#include <astc-transcoder/command_line_parser.h>
#include <astc-transcoder/version.h>


namespace astc_transcoder {

[[nodiscard]] static int OnHelpApp ()
{
    constexpr char const man[] =
#include <astc-transcoder/help.h>
    ;

    std::cout << man;
    return EXIT_SUCCESS;
}

[[nodiscard]] static int OnTranscodeApp ()
{
    std::cerr << "Implement me, please." << std::endl;
    return EXIT_FAILURE;
}

[[nodiscard]] static int OnVersionApp ()
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
    astc_transcoder::CommandLineParser const parser ( argc,
        argv,
        &astc_transcoder::OnHelpApp,
        &astc_transcoder::OnTranscodeApp,
        &astc_transcoder::OnVersionApp
    );

    return parser.Run ();
}
