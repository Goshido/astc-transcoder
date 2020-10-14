#include <stdafx.h>
#include <astc-transcoder/command_line_parser.h>


namespace astc_transcoder {

CommandLineParser::CommandLineParser ( int argc,
    char const* const* argv,
    HelpHandler helpCallback,
    TranscodeHandler transcodeCallback,
    VersionHandler versionCallback
):
    _argc ( argc ),
    _argv ( argv ),

    _handlers
    {
        { std::string_view ( "--help" ), &CommandLineParser::OnHelp },
        { std::string_view ( "--transcode" ), &CommandLineParser::OnTranscode },
        { std::string_view ( "--version" ), &CommandLineParser::OnVersion }
    },

    _helpCallback ( helpCallback ),
    _transcodeCallback ( transcodeCallback ),
    _versionCallback ( versionCallback )
{
    // NOTHING
}

int CommandLineParser::Run () const
{
    if ( _argc < 2 )
        return OnInvalidCommand ();

    auto const findResult = _handlers.find ( _argv[ 1U ] );

    if ( findResult == _handlers.cend () )
        return OnInvalidCommand ();

    CommandHandler const handler = findResult->second;
    return ( this->*handler ) ( _argc - 2, _argv + 2U );
}

int CommandLineParser::OnHelp ( int /*argc*/, char const* const* /*argv*/ ) const
{
    return _helpCallback ();
}

int CommandLineParser::OnTranscode ( int /*argc*/, char const* const* /*argv*/ ) const
{
    // TODO
    return _transcodeCallback ();
}

int CommandLineParser::OnVersion ( int /*argc*/, char const* const* /*argv*/ ) const
{
    return _versionCallback ();
}

int CommandLineParser::OnInvalidCommand () const
{
    std::cerr << "Invalid command. Please refer to help section for more details:" << std::endl << std::endl;
    _helpCallback ();
    return EXIT_FAILURE;
}

} // namespace astc_transcoder
