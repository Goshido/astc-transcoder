#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H


namespace astc_transcoder {

typedef void ( *ExitAppHandler ) ();
typedef void ( *TranscodeAppHandler ) ();

class CommandLineParser final
{
    private:
        [[maybe_unused]] ExitAppHandler const           _exitCallback;
        [[maybe_unused]] TranscodeAppHandler const      _transcodeCallback;

    public:
        CommandLineParser () = delete;

        CommandLineParser ( CommandLineParser const & ) = delete;
        CommandLineParser& operator = ( CommandLineParser const & ) = delete;

        CommandLineParser ( CommandLineParser && ) = delete;
        CommandLineParser& operator = ( CommandLineParser && ) = delete;

        explicit CommandLineParser ( int argc,
            char const* const* argv,
            ExitAppHandler exitCallack,
            TranscodeAppHandler transcodeCallback
        );

        ~CommandLineParser () = default;
};

} // namespace astc_transcoder


#endif // COMMAND_LINE_PARSER_H
