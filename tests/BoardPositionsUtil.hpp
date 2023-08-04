#define BOOST_TEST_DYN_LINK
#include <iostream>

namespace testUtil 
{
    inline const std::string FEN_DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    inline const std::string FEN_SCOTCH_MAINLINE = "r1b1k1nr/pppp1ppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R b KQkq - 0 6";
    inline const std::string FEN_FRIED_LIVER_ATTACK_FRITZ = "r1bqkb1r/ppp2ppp/5n2/3Pp1N1/2Bn4/8/PPPP1PPP/RNBQK2R w KQkq - 1 6";

    inline const std::string FEN_KNIGHT_MOVES = "8/8/8/5n2/4p3/6N1/4P3/8 w - - 0 1";
    inline const std::string FEN_PAWN_MOVES = "8/8/8/1pPp4/8/4p1pp/5P1P/8 w - - 0 1";
    inline const std::string FEN_POSITION_WITH_ILLEGAL_MOVES_1 = "rnbqk1nr/pppp1ppp/8/4p2Q/4P3/B7/PPPP1PPP/RN2KBNR w KQkq - 0 1";
    inline const std::string FEN_POSITION_WITH_ILLEGAL_MOVES_2 = "r2qk2r/1pp1bppp/2np1n2/p4b2/1P1P1B2/P4NP1/2P2PBP/RN1QR1K1 w Qkq - 0 1";
}
