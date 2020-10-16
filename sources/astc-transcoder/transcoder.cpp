#include <stdafx.h>
#include <astc-transcoder/transcoder.h>


namespace astc_transcoder {

constexpr static char const COMPRESSONATOR_PATH_ENV[] = "COMPRESSONATOR_PATH";
constexpr static char const COMPRESSONATOR_TOOL_ENV[] = "CompressonatorCLI.exe";
constexpr static char const TEMP_DIRECTORY_ENV[] = "TEMP";
constexpr static char const TEMP_SUBDIRECTORY[] = "astc-transcoder";

Transcoder::Transcoder ( uint8_t blockWidth, uint8_t blockHeight, char const* inputFile, char const* outputFile ):
    _blockWidth ( blockWidth ),
    _blockHeight ( blockHeight ),
    _inputFile ( inputFile ),
    _outputFile ( outputFile )
{
    // NOTHING
}

int Transcoder::Run () const
{
    if ( !CheckInputFile () )
        return EXIT_FAILURE;

    [[maybe_unused]] char const* nativeBlockSize = nullptr;

    if ( !ResolveBlockSize ( nativeBlockSize ) )
        return EXIT_FAILURE;

    [[maybe_unused]] std::string compressonator;

    if ( !FindCompressonator ( compressonator ) )
        return EXIT_FAILURE;

    [[maybe_unused]] std::string tempFile;

    if ( !CreateTempFile ( tempFile ) )
        return EXIT_FAILURE;

    // TODO

    return EXIT_SUCCESS;
}

bool Transcoder::CheckInputFile () const
{
    if ( std::filesystem::exists ( _inputFile ) )
        return true;

    std::cerr << "Input file does not exist:" << std::endl << "    " << _inputFile << std::endl;
    return false;
}

bool Transcoder::ResolveBlockSize ( char const* &nativeBlockSize ) const
{

#define ASTC_RECORD(x, y) { { static_cast<uint8_t> ( x ), static_cast<uint8_t> ( y ) }, #x "x" #y }

    static std::map<std::pair<uint8_t, uint8_t>, char const*> const supported =
    {
        ASTC_RECORD ( 4, 4 ),
        ASTC_RECORD ( 5, 4 ),
        ASTC_RECORD ( 5, 5 ),
        ASTC_RECORD ( 6, 5 ),
        ASTC_RECORD ( 6, 6 ),
        ASTC_RECORD ( 8, 5 ),
        ASTC_RECORD ( 8, 6 ),
        ASTC_RECORD ( 8, 8 ),
        ASTC_RECORD ( 10, 5 ),
        ASTC_RECORD ( 10, 6 ),
        ASTC_RECORD ( 10, 8 ),
        ASTC_RECORD ( 10, 10 ),
        ASTC_RECORD ( 12, 10 ),
        ASTC_RECORD ( 12, 12 )
    };

#undef ASTC_RECORD

    auto const findResult = supported.find ( std::make_pair ( _blockWidth, _blockHeight ) );

    if ( findResult != supported.cend () )
    {
        nativeBlockSize = findResult->second;
        return true;
    }

    std::cerr << "Block size " << static_cast<int> ( _blockWidth ) << 'x' << static_cast<int> ( _blockHeight )
        << " is unsupported. The supported block sizes:";

    for ( auto const& [tmp, native] : supported )
        std::cerr << std::endl << "    " << native;

    std::cerr << std::endl;
    return false;
}

bool Transcoder::CheckRpcStatus ( RPC_STATUS status, char const* message )
{
    if ( status == RPC_S_OK )
        return true;

    std::cerr << message << " Error " << static_cast<int> ( status ) << '.' << std::endl;
    return false;
}

bool Transcoder::CreateTempFile ( std::string &path )
{
    UUID uuid {};

    if ( !CheckRpcStatus ( UuidCreate ( &uuid ), "Can't create UUID for temporary file." ) )
        return false;

    RPC_CSTR nameBase = nullptr;

    if ( !CheckRpcStatus ( UuidToStringA ( &uuid, &nameBase ), "Can't makeup the name for temporary file." ) )
        return false;

    path = std::getenv ( TEMP_DIRECTORY_ENV );
    path += '/';
    path += TEMP_SUBDIRECTORY;

    std::error_code code {};
    bool const result = std::filesystem::create_directories ( path, code );

    if ( !result && code.value () != ERROR_SUCCESS )
    {
        std::cerr << "Can't create directory for temporary file:" << std::endl << "    " << path << std::endl
            << "Error: " << code.value () << '.' << std::endl;

        return false;
    }

    path += '/';
    path += reinterpret_cast<char const*> ( nameBase );

    return CheckRpcStatus ( RpcStringFreeA ( &nameBase ), "Can't free the name string for temporary file." );
}

bool Transcoder::FindCompressonator ( std::string &path )
{
    char const* compressonatorDirectory = std::getenv ( COMPRESSONATOR_PATH_ENV );

    if ( !compressonatorDirectory )
    {
        std::cerr << R"__(")__" << COMPRESSONATOR_PATH_ENV <<
            R"__(" environment variable is not set. Did you forget to set it?)__" << std::endl;

        return false;
    }

    path = compressonatorDirectory;
    char const last = *path.crbegin ();

    if ( last != '/' && last != '\\' )
        path += '/';

    path += COMPRESSONATOR_TOOL_ENV;

    if ( std::filesystem::exists ( path ) )
        return true;

    std::cerr << "Can't find CompressonatorCLI in the directory:" << std::endl << "    "
        << compressonatorDirectory << std::endl << std::endl << R"__(Please check the environment variable ")__"
        << COMPRESSONATOR_PATH_ENV << R"__(")__" << std::endl;

    return false;
}

} // namespace astc_transcoder 
