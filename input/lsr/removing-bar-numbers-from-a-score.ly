%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "rhythms, contexts-and-engravers"

%% Translation of GIT committish: 5a33019d8b4a837f564eadd4979501ff07e9dfd5
  texidoces = "

Se pueden eliminar completamente los números de compás quitando el
grabador @code{Bar_number_engraver} del contexto de @code{Score}.

"

  doctitlees = "Suprimir los números de compás de toda la partitura"


%% Translation of GIT committish: 13ac9d1dfbcc297166948ee396e9ade078d2446e  
  texidocde = "
Taktnummern können vollkommen aus den Noten entfernt werden, indem
man den @code{Bar_number_engraver} aus dem @code{Score}-Kontext
entfernt.

"
  doctitlede = "Entfernung von Taktnummern in einer Partitur"




  texidoc = "
Bar numbers can be removed entirely by removing the
@code{Bar_number_engraver} from the @code{Score} context.

"
  doctitle = "Removing bar numbers from a score"
} % begin verbatim

\layout {
  \context {
    \Score
    \remove "Bar_number_engraver"
  }
}

\relative c'' {
  c4 c c c \break
  c4 c c c
}
