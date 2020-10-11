#include <stdafx.h>
#include <astc-transcoder/command_line_parser.h>


namespace astc_transcoder {

void OnExitApp ()
{
    // TODO
}

void OnTranscodeApp ()
{
    // TODO
}

} // namespace astc_transcoder 

int main ( int argc, char** argv )
{
    astc_transcoder::CommandLineParser ( argc,
        argv,
        &astc_transcoder::OnExitApp,
        &astc_transcoder::OnTranscodeApp
    );

    return EXIT_SUCCESS;
}
