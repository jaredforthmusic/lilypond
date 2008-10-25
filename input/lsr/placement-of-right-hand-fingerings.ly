%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.11.62"

\header {
  lsrtags = "fretted-strings"

  texidoces = "
Es posible ejercer un mayor control sobre la colocación de las
digitaciones de la mano derecha estableciendo el valor de una
propiedad específica, como se muestra en el ejemplo siguiente.

"
  doctitlees = "Posicionamiento de digitaciones de mano derecha"

  texidoc = "
It is possible to exercise greater control over the placement of
right-hand fingerings by setting a specific property, as demonstrated
in the following example.

"
  doctitle = "Placement of right-hand fingerings"
} % begin verbatim
#(define RH rightHandFinger)

\relative c {
  \clef "treble_8"
  
  \set strokeFingerOrientations = #'(up down)
  <c-\RH #1 e-\RH #2 g-\RH #3 c-\RH #4 >4
  
  \set strokeFingerOrientations = #'(up right down)
  <c-\RH #1 e-\RH #2 g-\RH #3 c-\RH #4 >4
  
  \set strokeFingerOrientations = #'(left)
  <c-\RH #1 e-\RH #2 g-\RH #3 c-\RH #4 >2
}
