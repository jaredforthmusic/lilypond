%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "expressive-marks, tweaks-and-overrides"

%% Translation of GIT committish: dff50e8e1d3134657a6b6203b9c93826dc4cef65
  texidoces = "
El orden vertical que ocupan las inscripciones gráficas está
controlado con la propiedad @code{script-priority}. Cuanto más
bajo es este número, más cerca de la nota se colocará. En este
ejemplo, el @code{TextScript} (el sostenido) tiene primero la
prioridad más baja, por lo que se sitúa en la posición más baja en
el primer ejemplo. En el segundo, el semitrino (el @code{Script})
es el que la tiene más baja, por lo que se sitúa en la parte
interior. Cuando dos objetos tienen la misma prioridad, el orden
en que se introducen determina cuál será el que aparece en primer
lugar.

"
  doctitlees = "Controlar la ordenación vertical de las inscripciones"
  
%% Translation of GIT committish: 6ce7f350682dfa99af97929be1dec6b9f1cbc01a
texidocde = "
Die vertikale Anordnung von Beschriftungen wird mit der
@code{'script-priority}-Eigenschaft kontrolliert.  Um so kleiner die
Zahl, umso näher wird die Beschriftung in Bezug auf die Note gesetzt.  In
diesem Beispiel hat das @code{TextScript}-Objekt (das Kreuz) zuerst
die niedrigste Priorität, wird also auch am niedrigsten in dem ersten
Beispiel gesetzt.  Im zweiten Fall hat der Praller (das @code{Script}) 
die niedrigste Priorität, darum wird er am nächsten zum System gesetzt.
Wenn zwei Objekte die gleiche Priorität haben, wird ihre Reihenfolge
anhand ihres Auftretens in der Quelldatei entschieden.

"
  doctitlede = "Die vertikale Anordnung von Beschriftungen kontrollieren"

  texidoc = "
The vertical ordering of scripts is controlled with the
@code{'script-priority} property. The lower this number, the closer it
will be put to the note. In this example, the @code{TextScript} (the
sharp symbol) first has the lowest priority, so it is put lowest in the
first example. In the second, the prall trill (the @code{Script}) has
the lowest, so it is on the inside. When two objects have the same
priority, the order in which they are entered determines which one
comes first. 

"
  doctitle = "Controlling the vertical ordering of scripts"
} % begin verbatim

\relative c''' {
  \once \override TextScript #'script-priority = #-100
  a2^\prall^\markup { \sharp }
  
  \once \override Script #'script-priority = #-100
  a2^\prall^\markup { \sharp }
}
