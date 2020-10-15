#include <stdafx.h>
#include <astc-transcoder/transcoder.h>


namespace astc_transcoder {

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
    std::cerr << "Transcoder::Run - Almost there:" << std::endl
        << "    _blockWidth: " << static_cast<int> ( _blockWidth ) << std::endl
        << "    _blockHeight: " << static_cast<int> ( _blockHeight ) << std::endl
        << "    _inputFile: " << _inputFile << std::endl
        << "    _outputFile: " << _outputFile;

    // TODO
    return EXIT_FAILURE;
}

} // namespace astc_transcoder 
