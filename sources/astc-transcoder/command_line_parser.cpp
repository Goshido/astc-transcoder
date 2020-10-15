#include <stdafx.h>
#include <astc-transcoder/command_line_parser.h>


namespace astc_transcoder {

using TranscoderParamHandler = bool ( * ) ( void* result, int &count, char const* const* &params );
using TranscoderParamContext = std::pair<TranscoderParamHandler, void*>;

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

int CommandLineParser::OnTranscode ( int argc, char const* const* argv ) const
{
    uint8_t blockWidth = 0U;
    uint8_t blockHeight = 0U;
    char const* inputFile = nullptr;
    char const* outputFile = nullptr;

    std::map<std::string_view, TranscoderParamContext> mapper =
    {
        { std::string_view ( "--block-width" ), { &CommandLineParser::ParseUByte, &blockWidth } },
        { std::string_view ( "--block-height" ), { &CommandLineParser::ParseUByte, &blockHeight } },
        { std::string_view ( "--in" ), { &CommandLineParser::ParseFilePath, &inputFile } },
        { std::string_view ( "--out" ), { &CommandLineParser::ParseFilePath, &outputFile } }
    };

    std::set<std::string_view> processed;

    while ( !mapper.empty () && argc > 0 )
    {
        char const* param = *argv;
        auto findResult = mapper.find ( param );

        if ( findResult == mapper.cend () )
        {
            if ( processed.count ( param ) > 0U )
                std::cerr << R"__(Parameter ")__" << param << R"__(" duplication.)__" << std::endl;
            else
                std::cerr << R"__(Unknown parameter ")__" << param << R"__(".)__" << std::endl;

            return EXIT_FAILURE;
        }

        TranscoderParamContext const& context = findResult->second;

        if ( !context.first ( context.second, argc, argv ) )
            return EXIT_FAILURE;

        processed.insert ( findResult->first );
        mapper.erase ( findResult );
    }

    if ( !mapper.empty () )
    {
        std::cerr << "Too few parameters. Missing required parameters:";

        for ( auto const& [param, context] : mapper )
            std::cerr << std::endl << "    " << param;

        return EXIT_FAILURE;
    }

    if ( argc == 0 )
        return _transcodeCallback ( blockWidth, blockHeight, inputFile, outputFile );

    std::cerr << "Too much parameters.";
    return EXIT_FAILURE;
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

bool CommandLineParser::ParseFilePath ( void* result, int &count, char const* const* &params )
{
    if ( count < 2 )
    {
        std::cerr << R"__(Missing file path for ")__" << *params << R"__(".)__" << std::endl;
        return false;
    }

    auto const** path = reinterpret_cast<char const**> ( result );
    *path = params[ 1U ];

    count -= 2;
    params += 2U;

    return true;
}

bool CommandLineParser::ParseUByte ( void* result, int &count, char const* const* &params )
{
    if ( count < 2 )
    {
        std::cerr << R"__(Missing value for ")__" << *params << R"__(".)__" << std::endl;
        return false;
    }

    char const* param = params[ 1U ];
    static std::regex const isInteger ( R"__(^\d+$)__" );

    if ( !std::regex_match ( param, isInteger ) )
    {
        std::cerr << R"__(The parameter ")__" << params[ 0U ]
            << R"__(" is not an unsigned integer value.)__" << std::endl;

        return false;
    }

    int raw = std::atoi ( param );
    constexpr int const limit = static_cast<int> ( UINT8_MAX );

    if ( raw < 0 || raw > limit )
    {
        std::cerr << R"__(The parameter ")__" << params[ 0U ] << R"__(" is not in range [0, )__" << limit << ']'
            << std::endl;

        return false;
    }

    auto* value = static_cast<uint8_t*> ( result );
    *value = static_cast<uint8_t> ( raw );

    count -= 2;
    params += 2U;

    return true;
}

} // namespace astc_transcoder
