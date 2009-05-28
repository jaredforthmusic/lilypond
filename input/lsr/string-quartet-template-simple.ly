%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "unfretted-strings, template"

%% Translation of GIT committish: d35687993883eb31442009fc50d5ae063108bfa7
  texidoces = "
Esta plantilla muestra un cuarteto de cuerda normal. También utiliza
una sección @code{\\global} para el compás y la armadura

"
  doctitlees = "Plantilla de cuarteto de cuerda (sencilla)"
%% Translation of GIT committish: 17633f6b8681af86230aa84597fe7561e98c91d6
  
  texidocde = "
Dieses Beispiel demonstriert die Partitur für ein Streichquartett. Hier 
wird auch eine @qq{@code{\global}}-Variable für Taktart und 
Vorzeichen benutzt.
"

%% Translation of GIT committish: 06d99c3c9ad1c3472277b4eafd7761c4aadb84ae
  texidocja = "
これは簡単な弦楽四重奏のためのテンプレートです。これは拍子記号と調号のために 
@code{@bs{}global} セクションを使っています。
"

  texidoc = "
This template demonstrates a simple string quartet. It also uses a
@code{\\global} section for time and key signatures

"
  doctitle = "String quartet template (simple)"
} % begin verbatim

global= {
  \time 4/4
  \key c \major
}

violinOne = \new Voice \relative c'' {
  \set Staff.instrumentName = #"Violin 1 "
  
  c2 d
  e1
  
  \bar "|."
}
 
violinTwo = \new Voice \relative c'' {
  \set Staff.instrumentName = #"Violin 2 "
  
  g2 f
  e1
  
  \bar "|."
}

viola = \new Voice \relative c' {
  \set Staff.instrumentName = #"Viola "  
  \clef alto
  
  e2 d
  c1
  
  \bar "|."
}

cello = \new Voice \relative c' {
  \set Staff.instrumentName = #"Cello "
  \clef bass
  
  c2 b
  a1
  
  \bar "|."
}

\score {
  \new StaffGroup <<
    \new Staff << \global \violinOne >>
    \new Staff << \global \violinTwo >>
    \new Staff << \global \viola >>
    \new Staff << \global \cello >>
  >>
  \layout { }
  \midi { }
}
