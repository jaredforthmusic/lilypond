%%  Do not edit this file; it is auto-generated from LSR!
\version "2.11.23"

\header { texidoc = "
Here is an excerpt of Mozart's KV545 piano Sonata (as brought to
Mutopia by A. Sierra), in which you can see the typical classical
trill, i.e. beginning with an acciaccatura, then trilling the whole
bar, and ending with two afterGrace notes. 

This combination, which is rather frequent (if not systematic) in
classical piano music, requires a rather precise typesetting: first the
\acciaccatura, then the \afterGrace (which you have to define using
Scheme-like syntax), then the main note, the TrillSpan and the two
final notes (don't forget the curly braces here). You can also notice
here the \slurDown, which avoids slurs collision.
" }

rh = \relative {
  b''8-3(\f g-1) d'2-5(\sf c16 b a g)
#(define afterGraceFraction (cons 31 32))
 \acciaccatura b8-3 \slurDown \afterGrace a1  \startTrillSpan \( ( {g16[ a ] )
\stopTrillSpan}
  g4 \) r2.
}

lh = \relative {
  \clef treble 
  d16_5 b'_1 g_2 b \repeat unfold 3 { d, b' g b } 
  \repeat unfold 4 { d, c' fis, c' }
  <g-2 b-1>4 r2. 
}


\score {
  \new PianoStaff <<
    \new Staff \rh
    \new Staff \lh
  >>
}
