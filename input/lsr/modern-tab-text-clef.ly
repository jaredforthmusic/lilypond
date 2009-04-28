%% Do not edit this file; it is auto-generated from input/new
%% This file is in the public domain.
%% Note: this file works from version 2.12.0
\version "2.13.1"

\header {
  lsrtags = "staff-notation, fretted-strings"

  texidoc = "
Use a markup text to replace the (TAB) clef glyph with a modern font.

"
  doctitle = "Modern TAB text clef"
} % begin verbatim


TAB = \markup {
  \raise #1.5
  \sans
  \bold
  \huge
  \override #'(baseline-skip . 2.5)
  \center-column {
    T
    A
    B
  }
}

\new TabStaff {
  \override Staff.Clef #'stencil = #(lambda (grob)
    (grob-interpret-markup grob TAB))
  a
}
