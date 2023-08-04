#define BOOST_TEST_DYN_LINK
#include <iostream>

namespace testUtil 
{
    // +-+-+-+-+-+-+-+-+
    // |r|n|b|q|k|b|n|r|
    // |p|p|p|p|p|p|p|p|
    // | | | | | | | | |
    // | | | | | | | | |
    // | | | | | | | | |
    // | | | | | | | | |
    // |P|P|P|P|P|P|P|P|
    // |R|N|B|Q|K|B|N|R|
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // +-+-+-+-+-+-+-+-+
    // |r| |b| |k| |n|r|
    // |p|p|p|p| |p|p|p|
    // | | |n| | |q| | |
    // | | |b| | | | | |
    // | | | |N|P| | | |
    // | | |P| |B| | | |
    // |P|P| | | |P|P|P|
    // |R|N| |Q|K|B| |R|
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_SCOTCH_MAINLINE = "r1b1k1nr/pppp1ppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R b KQkq - 0 6";

    // +-+-+-+-+-+-+-+-+
    // |r| |b|q|k|b| |r|
    // |p|p|p| | |p|p|p|
    // | | | | | |n| | |
    // | | | |P|p| |N| |
    // | | |B|n| | | | |
    // | | | | | | | | |
    // |P|P|P|P| |P|P|P|
    // |R|N|B|Q|K| | |R|
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_FRIED_LIVER_ATTACK_FRITZ = "r1bqkb1r/ppp2ppp/5n2/3Pp1N1/2Bn4/8/PPPP1PPP/RNBQK2R w KQkq - 1 6";

    // +-+-+-+-+-+-+-+-+
    // | | | | | | | | |
    // | | | | | | | | |
    // | | | | | | | | |
    // | | | | | |n| | |
    // | | | | |p| | | |
    // | | | | | | |N| |
    // | | | | |P| | | |
    // | | | | | | | | |
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_KNIGHT_MOVES = "8/8/8/5n2/4p3/6N1/4P3/8 w - - 0 1";

    // +-+-+-+-+-+-+-+-+
    // | | | | | | | | |
    // | | | | | | | | |
    // | | | | | | | | |
    // | |p|P|p| | | | |
    // | | | | | | | | |
    // | | | | |p| |p|p|
    // | | | | | |P| |P|
    // | | | | | | | | |
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_PAWN_MOVES = "8/8/8/1pPp4/8/4p1pp/5P1P/8 w - - 0 1";

    // +-+-+-+-+-+-+-+-+
    // |r|n|b|q|k| |n|r|
    // |p|p|p|p| |p|p|p|
    // | | | | | | | | |
    // | | | | |p| | |Q|
    // | | | | |P| | | |
    // |B| | | | | | | |
    // |P|P|P|P| |P|P|P|
    // |R|N| | |K|B|N|R|
    // +-+-+-+-+-+-+-+-+    
    inline const std::string FEN_POSITION_WITH_ILLEGAL_MOVES_1 = "rnbqk1nr/pppp1ppp/8/4p2Q/4P3/B7/PPPP1PPP/RN2KBNR w KQkq - 0 1";

    // +-+-+-+-+-+-+-+-+
    // |r| | |q|k| | |r|
    // | |p|p| |b|p|p|p|
    // | | |n|p| |n| | |
    // |p| | | | |b| | |
    // | |P| |P| |B| | |
    // |P| | | | |N|P| |
    // | | |P| | |P|B|P|
    // |R|N| |Q|R| |K| |
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_POSITION_WITH_ILLEGAL_MOVES_2 = "r2qk2r/1pp1bppp/2np1n2/p4b2/1P1P1B2/P4NP1/2P2PBP/RN1QR1K1 w Qkq - 0 1";

    // +-+-+-+-+-+-+-+-+
    // |r| |b|q|k|b|n|r|
    // |p|p|p| |p| | |p|
    // | | |n|p| |P| | |
    // | | | | | | | |Q|
    // | | | | |P| | | |
    // | | | | | | | | |
    // |P|P|P|P| |P|P| |
    // |R|N|B| |K|B|N|R|
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_KING_IN_CHECK_POSITION = "r1bqkbnr/ppp1p2p/2np1P2/7Q/4P3/8/PPPP1PP1/RNB1KBNR w KQkq - 0 1";

    // +-+-+-+-+-+-+-+-+
    // | | | | | | | | |
    // | | | | | | | | |
    // | | | | | | | | |
    // | | | | |b| | |p|
    // | | | | | | | |P|
    // | | | | | | |p|K|
    // | | | | | | |P| |
    // | | | | | | | | |
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_STALEMATE_POSITION = "8/8/8/4b2p/7P/6pK/6P1/8 w - - 0 1";

    // +-+-+-+-+-+-+-+-+
    // |r|n|b|q|k|b|n|r|
    // |p|p|p|p|p| | |p|
    // | | | | | |p| | |
    // | | | | | | |p|Q|
    // | | | |P|P| | | |
    // | | | | | | | | |
    // |P|P|P| | |P|P|P|
    // |R|N|B| |K|B|N|R|
    // +-+-+-+-+-+-+-+-+
    inline const std::string FEN_FOOLS_CHECKMATE_REVERSED = "rnbqkbnr/ppppp2p/5p2/6pQ/3PP3/8/PPP2PPP/RNB1KBNR w KQkq - 0 1";
}
