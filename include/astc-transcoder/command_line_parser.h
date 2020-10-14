#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H


namespace astc_transcoder {

typedef int ( *HelpHandler ) ();
typedef int ( *TranscodeHandler ) ();
typedef int ( *VersionHandler ) ();

class CommandLineParser final
{
    private:
        using CommandHandler = int ( CommandLineParser::* ) ( int argc, char const* const* argv ) const;

    private:
        int                                                             _argc;
        char const* const*                                              _argv;

        std::unordered_map<std::string_view, CommandHandler> const      _handlers;

        HelpHandler const                                               _helpCallback;
        TranscodeHandler const                                          _transcodeCallback;
        VersionHandler const                                            _versionCallback;

    public:
        CommandLineParser () = delete;

        CommandLineParser ( CommandLineParser const & ) = delete;
        CommandLineParser& operator = ( CommandLineParser const & ) = delete;

        CommandLineParser ( CommandLineParser && ) = delete;
        CommandLineParser& operator = ( CommandLineParser && ) = delete;

        explicit CommandLineParser ( int argc,
            char const* const* argv,
            HelpHandler helpCallback,
            TranscodeHandler transcodeCallback,
            VersionHandler versionCallback
        );

        ~CommandLineParser () = default;

        [[nodiscard]] int Run () const;

    private:
        [[nodiscard]] int OnHelp ( int argc, char const* const* argv ) const;
        [[nodiscard]] int OnTranscode ( int argc, char const* const* argv ) const;
        [[nodiscard]] int OnVersion ( int argc, char const* const* argv ) const;

        [[nodiscard]] int OnInvalidCommand () const;
};

} // namespace astc_transcoder


#endif // COMMAND_LINE_PARSER_H
