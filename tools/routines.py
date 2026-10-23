"""docs/ROUTINES.md from the game's own comments: every function in game/, the original routine it
is (the comment before it, "module94.asm:Label. What it does."), and what it does (that comment's
first sentence). Run from the repository's root after changing the game: python tools/routines.py"""
import os
import re

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
GAME = os.path.join(ROOT, 'game')
FOLDERS = [('play', 'Play: the game on the ice (skating, the AI, checks, the puck, goalies, penalties, periods)'),
           ('screen', 'Screen: the picture (video, sprites, the camera, replays, the crowd, cards, title and attract)'),
           ('menus', 'Menus: setup, the options, records, stats'),
           ('sound', 'Sound: the sound driver and the Z80 that runs it'),
           ('system', 'System: power on, the 68000 jumps through RAM, RAM, the ROM, save RAM, 4 Way Play, data tables, the teams')]
DEF = re.compile(r'^(?:static\s+)?(?:inline\s+)?[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;]*\)\s*\{?\s*$')
LABEL = re.compile(r'^\s*/?\*?\s*(\(native\)\s*)?([a-z0-9]+94\.asm):([\w.]*\w)\.?\s*(.*)', re.S)


def first_sentence(text):
    text = ' '.join(text.split())
    m = re.match(r'(.+?[.!?])(\s|$)', text)
    return (m.group(1) if m else text).replace('|', '\\|')


def routines(path):
    lines = open(path, encoding='utf-8').read().splitlines()
    out = []
    for i, line in enumerate(lines):
        m = DEF.match(line)
        if not m or line.startswith((' ', '\t', '#', '/', '*', '}')) or m.group(1) in ('if', 'while', 'for', 'switch', 'return'):
            continue
        if not (line.rstrip().endswith('{') or (i + 1 < len(lines) and lines[i + 1].startswith('{'))):
            continue
        j, block = i - 1, []
        while j >= 0 and (lines[j].lstrip().startswith(('*', '/*')) or lines[j].rstrip().endswith('*/')):
            block.insert(0, lines[j].strip())
            if lines[j].lstrip().startswith('/*'): break
            j -= 1
        comment = ' '.join(b.lstrip('/*').rstrip('*/').strip() for b in block)
        c = LABEL.match(comment)
        if c: out.append((m.group(1), c.group(2) + ':' + c.group(3), first_sentence(c.group(4))))
        else: out.append((m.group(1), '', first_sentence(comment) if comment else ''))
    return out


total = labelled = 0
md = ['# The routines', '',
      'Every function in `game/`, by folder and file: the C name, the original routine it is (the',
      'original assembly module and label, as the community\'s byte-matching rebuild names them), and what it does.',
      'Generated from the code\'s own comments by `tools/routines.py`. A helper is a piece split out of an',
      'original routine (its comment says which) or a small function of the port\'s own (the 68000\'s',
      'arithmetic, its jumps through RAM).', '']
body = []
for folder, title in FOLDERS:
    body += ['## ' + title, '']
    for f in sorted(os.listdir(os.path.join(GAME, folder))):
        if not f.endswith('.c'): continue
        rs = routines(os.path.join(GAME, folder, f))
        if not rs: continue
        body += ['### `game/%s/%s` (%d)' % (folder, f, len(rs)), '', '| Function | Original | What it does |', '|---|---|---|']
        for name, orig, what in rs:
            body.append('| `%s` | %s | %s |' % (name, '`%s`' % orig if orig else '(a helper)', what))
            total += 1
            labelled += bool(orig)
        body.append('')
md += ['%d functions: %d original routines, %d helpers.' % (total, labelled, total - labelled), ''] + body
open(os.path.join(ROOT, 'docs', 'ROUTINES.md'), 'w', encoding='utf-8', newline='\n').write('\n'.join(md))
print('docs/ROUTINES.md: %d functions, %d original routines, %d helpers' % (total, labelled, total - labelled))
