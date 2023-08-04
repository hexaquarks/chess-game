import re
import argparse

def expand_blanks(fen):
    def expand(match):
        return ' ' * int(match.group(0))
    return re.compile(r'\d').sub(expand, fen)

def outer_join(sep, ss):
    return '%s%s%s' % (sep, sep.join(ss), sep)

def ascii_draw_chess_position(fen):
    pieces = expand_blanks(fen).replace('/', '')
    divider = '+-+-+-+-+-+-+-+-+\n'
    rows = ((outer_join('|', pieces[r: r + 8]) + '\n')
            for r in range(0, 8 * 8, 8))
    return divider + ''.join(rows) + divider

def fen_to_commented_ascii(fen):
    output = ascii_draw_chess_position(fen)
    cpp_comment_output = '\n'.join('// ' + line for line in output.split('\n'))
    return cpp_comment_output

parser = argparse.ArgumentParser(description='Convert FEN to ASCII Chess board')
parser.add_argument('fen', type=str, help='FEN string to be converted')
args = parser.parse_args()

print(fen_to_commented_ascii(args.fen))
