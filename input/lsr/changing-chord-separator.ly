%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "chords"

%% Translation of GIT committish: 7e2910d2bc6167b10cfe328eb17a5f710f2a515a
  texidoces = "
Se puede establecer el separador entre las distintas partes del
nombre de un acorde para que sea cualquier elemento de marcado.

"
  doctitlees = "Modificación del separador de acordes"

%% Translation of GIT committish: 33f623301a41fcc53efadf96bca1c72834763415
  texidocde = "
Der Trenner zwischen unterschiedlichen Teilen eines Akkordsymbols kann
beliebeiger Text sein.

"
  doctitlede = "Akkordsymboltrenner verändern"

  texidoc = "
The separator between different parts of a chord name can be set to any
markup.

"
  doctitle = "Changing chord separator"
} % begin verbatim

\chords {
  c:7sus4
  \set chordNameSeparator
    = \markup { \typewriter | }
  c:7sus4
}
