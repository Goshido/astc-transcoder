#ifndef TRANCODER_H
#define TRANCODER_H


namespace astc_transcoder {

class Transcoder final
{
    private:
        [[maybe_unused]] uint8_t        _blockWidth;
        [[maybe_unused]] uint8_t        _blockHeight;
        [[maybe_unused]] char const*    _inputFile;
        [[maybe_unused]] char const*    _outputFile;

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
};

} // namespace astc_transcoder


#endif // TRANCODER_H
