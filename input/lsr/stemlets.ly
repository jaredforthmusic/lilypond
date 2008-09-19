%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.57"

\header {
  lsrtags = "rhythms"

  texidoc = "
In some notational conventions beams are allowed to extend over rests. 
Depending on preference, these beams may drop 'stemlets' to help the
eye appreciate the rhythm better, and in some modern music the rest
itself is omitted and only the stemlet remains.


This snippet shows a progression from traditional notation, to beams
over the rest, to stemlets over the rest, to stemlets alone.  Stemlets
are generated by overriding the @code{'stemlet-length} property of
@code{Stem}, while rests are hidden by setting @code{'transparent =
##t}.


Some @code{\\markup} elements are included in the source to highlight
the different notations. 

"
  doctitle = "Stemlets"
} % begin verbatim
\paper { ragged-right = ##f }

{
  c'16^\markup { traditional } d' r f'
  g'16[^\markup { beams over rests } f' r d']
  
  % N.B. use Score.Stem to set for the whole score.
  \override Staff.Stem #'stemlet-length = #0.75
  
  c'16[^\markup { stemlets over rests } d' r f']
  g'16[^\markup { stemlets and no rests } f' \once \override Rest #'transparent = ##t r d']
}

