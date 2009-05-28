%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "rhythms, ancient-notation, tweaks-and-overrides"

%% Translation of GIT committish: a634a20e0ab2a874ff8bf423e5651139df8733fc
  doctitlees = "Estilos de silencios"
  texidoces = "
Los silencios se pueden imprimir en distintos estilos.
"
%% Translation of GIT committish: 6743e7d829e2efe37d5d00133094df664f1960df

  doctitlede = "Pausenstile"
  texidocde = "
Pausen können in verschiedenen Stilen dargestellt werden.
"


%% Translation of GIT committish: e71f19ad847d3e94ac89750f34de8b6bb28611df
  texidocfr = "
Les silences peuvent être gravés selon différents styles.

"
  doctitlefr = "Styles de silences"

  texidoc = "
Rests may be used in various styles.




"
  doctitle = "Rest styles"
} % begin verbatim

\layout {
  indent = 0.0
  \context {
    \Staff
    \remove "Time_signature_engraver"
  }
}

\new Staff \relative c {
  \cadenzaOn
  \override Staff.Rest #'style = #'mensural
  r\maxima^\markup \typewriter { mensural }
  r\longa r\breve r1 r2 r4 r8 r16 s32 s64 s128 s128
  \bar ""
  
  \override Staff.Rest #'style = #'neomensural
  r\maxima^\markup \typewriter { neomensural }
  r\longa r\breve r1 r2 r4 r8 r16 s32 s64 s128 s128
  \bar ""
  
  \override Staff.Rest #'style = #'classical
  r\maxima^\markup \typewriter { classical }
  r\longa r\breve r1 r2 r4 r8 r16 r32 r64 r128 s128
  \bar ""
  
  \override Staff.Rest  #'style = #'default
  r\maxima^\markup \typewriter { default }
  r\longa r\breve r1 r2 r4 r8 r16 r32 r64 r128 s128
}
