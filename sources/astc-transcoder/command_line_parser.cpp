#include <stdafx.h>
#include <astc-transcoder/command_line_parser.h>


namespace astc_transcoder {

CommandLineParser::CommandLineParser ( int /*argc*/,
    char const* const* /*argv*/,
    ExitAppHandler exitCallack,
    TranscodeAppHandler transcodeCallback
):
    _exitCallback ( exitCallack ),
    _transcodeCallback ( transcodeCallback )
{
    // TODO
}

} // namespace astc_transcoder
