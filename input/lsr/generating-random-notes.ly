%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "pitches"

%% Translation of GIT committish: 48f804da6794a7bc8e7fdd4b1649f485b0b09d26
  texidoces = "
Este fragmento de código basado en Scheme genera
24 notas aleatorias (o tantas como se necesiten), basándose en la
hora actual (o en cualquier número pseudo-aleatorio que se
especifique en su lugar, para obtener las mismas notas aleatorias
cada vez): es decir, para obtener distintos patrones de notas,
sólo tiene que modificar este número.

"
  doctitlees = "Generación de notas aleatorias"

  texidoc = "
This Scheme-based snippet generates 24 random notes (or as many as
required), based on the current time (or any randomish number specified
instead, in order to obtain the same random notes each time): i.e., to
get different random note patterns, just change this number.

"
  doctitle = "Generating random notes"
} % begin verbatim

\score {
  { #(let ((random-state (seed->random-state (current-time))))
    (ly:export
     (make-music 'SequentialMusic 'elements
      (map (lambda x
           (let ((idx (random 12 random-state)))
            (make-music 'EventChord
             'elements (list (make-music 'NoteEvent
                              'duration (ly:make-duration 2 0 1 1)
                              'pitch (ly:make-pitch (quotient idx 7)
                                      (remainder idx 7)
                                      0))))))
       (make-list 24)))))
  }
}
