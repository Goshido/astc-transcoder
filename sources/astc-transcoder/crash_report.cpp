#include <stdafx.h>
#include <astc-transcoder/crash_report.h>
#include <astc-transcoder/version.h>


#define STR(x)         #x
#define STRINGIZE(x)   STR ( x )

#define VERSION_STRING \
    STRINGIZE ( ASTC_TRANSCODER_VERSION_MAJOR ) "."\
    STRINGIZE ( ASTC_TRANSCODER_VERSION_MINOR ) "."\
    STRINGIZE ( ASTC_TRANSCODER_VERSION_RELEASE ) "."\
    STRINGIZE ( ASTC_TRANSCODER_VERSION_BUILD )

typedef BOOL ( WINAPI* MiniDumpWriteDumpProc ) ( HANDLE hProcess,
    DWORD ProcessId,
    HANDLE hFile,
    MINIDUMP_TYPE DumpType,
    PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    PMINIDUMP_CALLBACK_INFORMATION CallbackParam
);

//----------------------------------------------------------------------------------------------------------------------

namespace astc_transcoder {

#define PIVOT "APPDATA"
#define TARGET "Goshido Inc/astc-transcoder/crash dumps"

constexpr static char const PIVOT_ENV[] = PIVOT;
constexpr static char const TARGET_DIRECTORY[] = TARGET;
constexpr static char const CRASH_REPORT_DIRECTORY[] = R"__(%)__" PIVOT R"__(%/)__" TARGET "/" VERSION_STRING;
constexpr static char const INDENT[] = "    ";

// Forward declaration.
static LONG CALLBACK UnhandledExceptionFilter ( EXCEPTION_POINTERS* exceptionPointers );

bool InstallCrashReport ()
{
    char const* pivot = std::getenv ( PIVOT_ENV );

    std::stringstream stream;
    stream << pivot << '/' << TARGET_DIRECTORY << "/" VERSION_STRING;

    std::string const path = stream.str ();

    std::error_code code {};
    bool const result = std::filesystem::create_directories ( path, code );

    if ( !result && code.value () != ERROR_SUCCESS )
    {
        std::cerr << "Can't create crash report directory:" << std::endl << INDENT << path << std::endl;
        return false;
    }

    [[maybe_unused]] auto res = SetUnhandledExceptionFilter ( &UnhandledExceptionFilter );
    return true;
}

static LONG CALLBACK UnhandledExceptionFilter ( EXCEPTION_POINTERS* exceptionPointers )
{
    // Note code here is executed in very unstable environment: stack corruption, heap corruption.
    // Note all valiables are allocated in global memory (not stack) by design to successfully generate crash dump file.
    // see https://stackoverflow.com/questions/5028781/how-to-write-a-sample-code-that-will-crash-and-produce-dump-file

    static HMODULE dbghelp = LoadLibraryA ( "dbghelp.dll" );

    if ( !dbghelp )
        return EXCEPTION_CONTINUE_SEARCH;

    static auto miniDumpWriteDump = static_cast<MiniDumpWriteDumpProc> (
        static_cast<void*> ( GetProcAddress ( dbghelp, "MiniDumpWriteDump" ) )
    );

    if ( !miniDumpWriteDump )
        return EXCEPTION_CONTINUE_SEARCH;

    static FILETIME utcFileTime;
    GetSystemTimeAsFileTime ( &utcFileTime );

    static FILETIME localFileTime;
    FileTimeToLocalFileTime ( &utcFileTime, &localFileTime );

    static SYSTEMTIME currentTime;
    FileTimeToSystemTime ( &localFileTime, &currentTime );

    static char directory[ MAX_PATH ];
    ExpandEnvironmentStringsA ( CRASH_REPORT_DIRECTORY, directory, MAX_PATH );

    std::cout << CRASH_REPORT_DIRECTORY << std::endl;

    static char name[ MAX_PATH ];
    static char const* processFileName = name + GetModuleFileNameA ( GetModuleHandleA ( nullptr ), name, MAX_PATH );

    for ( ; *processFileName != '\\' && *processFileName != '/'; --processFileName );

    ++processFileName;
    static char dumpFilePath[ MAX_PATH ];

    wsprintfA ( dumpFilePath,
        "%s/%s %04hu.%02hu.%02hu %02hu-%02hu-%02hu.dmp",
        directory,
        processFileName,
        currentTime.wYear,
        currentTime.wMonth,
        currentTime.wDay,
        currentTime.wHour,
        currentTime.wMinute,
        currentTime.wSecond
    );

    static HANDLE dumpFile = CreateFileA ( dumpFilePath,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if ( !dumpFile )
        return EXCEPTION_CONTINUE_SEARCH;

    static MINIDUMP_EXCEPTION_INFORMATION exceptionInfo
    {
        .ThreadId = GetCurrentThreadId (),
        .ExceptionPointers = exceptionPointers,
        .ClientPointers = FALSE
    };

    miniDumpWriteDump ( GetCurrentProcess (),
        GetCurrentProcessId (),
        dumpFile,
        static_cast<MINIDUMP_TYPE> ( MiniDumpNormal ),
        exceptionPointers ? &exceptionInfo : nullptr,
        nullptr,
        nullptr
    );

    FlushFileBuffers ( dumpFile );
    CloseHandle ( dumpFile );

    std::cerr << "A fatal error has been detected. The crash dump was saved to:" << std::endl << INDENT
        << dumpFilePath << std::endl;

    return EXCEPTION_CONTINUE_SEARCH;
}

} // namespace astc_transcoder
