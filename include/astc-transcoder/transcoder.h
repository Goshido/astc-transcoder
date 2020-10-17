#ifndef TRANSCODER_H
#define TRANSCODER_H


namespace astc_transcoder {

class Transcoder final
{
    private:
        uint8_t         _blockWidth;
        uint8_t         _blockHeight;
        char const*     _inputFile;
        char const*     _outputFile;

    public:
        Transcoder () = delete;

        Transcoder ( Transcoder const & ) = delete;
        Transcoder& operator = ( Transcoder const & ) = delete;

        Transcoder ( Transcoder && ) = delete;
        Transcoder& operator = ( Transcoder && ) = delete;

        explicit Transcoder ( uint8_t blockWidth, uint8_t blockHeight, char const* inputFile, char const* outputFile );

        ~Transcoder () = default;

        // The method returns "main" function return code.
        [[nodiscard]] int Run () const;

    private:
        [[nodiscard]] bool CheckInputFile () const;
        [[nodiscard]] bool ResolveBlockSize ( char const* &nativeBlockSize ) const;

        [[nodiscard]] bool RunConvertStep ( std::string const &tempFile,
            std::string &&compressonator,
            char const* nativeBlockSize
        ) const;

        [[nodiscard]] bool RunDecompressStep ( std::string const &tempFile, std::string const &compressonator ) const;

        [[nodiscard]] static bool CheckRpcStatus ( RPC_STATUS status, char const* message );
        [[nodiscard]] static bool CreateTempFile ( std::string &path );
        [[nodiscard]] static bool FindCompressonator ( std::string &path );
        [[nodiscard]] static bool RemoveTempFile ( std::string &&path );
        [[nodiscard]] static bool RunProcess ( std::string &&process );
};

} // namespace astc_transcoder


#endif // TRANSCODER_H
