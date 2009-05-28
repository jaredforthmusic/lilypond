%% Do not edit this file; it is auto-generated from LSR http://lsr.dsi.unimi.it
%% This file is in the public domain.
\version "2.13.1"

\header {
  lsrtags = "rhythms"

%% Translation of GIT committish: 151f1bb8c0aae8294e62d0470246ee1019aedfbc
  texidoces = "
Las opciones para agrupar los pulsos de un compás están a nuestra
disposición a través de la función de Scheme
@code{set-time-signature}, que acepta tres argumentos: el número
de pulsos, la longitud del pulso, y la agrupación interna de los
pulsos dentro del compás.  Si se incluye el grabador
@code{Measure_grouping_engraver}, la función crea también símbolos
de agrupación @code{MeasureGrouping}.  Dichos símbolos facilitan
la lectura de la música moderna de cierta complejidad rítmica.  En
el ejemplo, el compás de 9/8 se subdivide en 2, 2, 2 y 3.  Esto se
pasa a la función @code{set-time-signature} como tercer argumento:
@code{'(2 2 2 3)}:

"
doctitlees = "Símbolos de dirección, símbolos de agrupación de compás"

%% Translation of GIT committish: 0364058d18eb91836302a567c18289209d6e9706
  texidocde = "
Optionen, mit denen die Balken in einem Takt gruppiert werden, sind
durch die Scheme-Funktion @code{set-time-signature} erhältlich, die
drei Argumente braucht:  Die Zahl der Taktschläge, die Länge des
Schlages und die interne gruppieren von Balken in dem Takt.  Wenn der
@code{Measure_grouping_engraver} hinzugefügt worden ist, erstellt
diese Funktion auch @code{MeasureGrouping}-(Taktgruppen)-Zeichen.  Derartige
Zeichen erleichtern das Lesen von rhythmisch komplexer Musik.  In dem
Beispiel ist der 9/8-Takt in 2, 2, 2 und 3 aufgeteilt.  Das wird
der @code{set-time-signature}-Funktion als das dritte Argument mitgegeben:
@code{'(2 2 2 3)}:

"
  doctitlede = "Dirigirzeichen, Taktgruppenzeichen"


%% Translation of GIT committish: b3196fadd8f42d05ba35e8ac42f7da3caf8a3079
  texidocfr = "
Il existe des options qui permettent de grouper les ligatures au sein
d'une mesure, grâce à la fonction Scheme @code{set-time-signature}.
Celle-ci prend trois arguments : le nombre de pulsations, la durée de la
pulsation et le regroupement des pulsations dans la mesure.  Si l'on
fait appel au @code{Measure_grouping_engraver}, la fonction 
@code{set-time-signature} créera aussi des symboles
@code{MeasureGrouping}.  Ces symboles aident à la lecture des œuvres
modernes à la rythmique complexe.  Dans l'exemple qui suit, la mesure à
9/8 este divisée en 2, 2, 2 et 3.  Ceci est fourni en tant que troisième
argument à @code{set-time-signature} : @code{'(2 2 2 3)}.

"
  doctitlefr = "Signes de direction, signes de sous-groupe"

  texidoc = "
Options to group beats within a bar are available through the Scheme
function @code{set-time-signature}, which takes three arguments: the
number of beats, the beat length, and the internal grouping of beats in
the measure.  If the @code{Measure_grouping_engraver} is included, the
function will also create @code{MeasureGrouping} signs.  Such signs
ease reading rhythmically complex modern music.  In the example, the
9/8 measure is subdivided in 2, 2, 2 and 3.  This is passed to
@code{set-time-signature} as the third argument: @code{'(2 2 2 3)}:

"
  doctitle = "Conducting signs, measure grouping signs"
} % begin verbatim

\score {
  \relative c'' {
    #(set-time-signature 9 8 '(2 2 2 3))
    #(revert-auto-beam-setting '(end * * 9 8) 3 8)
    #(override-auto-beam-setting '(end 1 8 9 8) 1 4)
    #(override-auto-beam-setting '(end 1 8 9 8) 2 4)
    #(override-auto-beam-setting '(end 1 8 9 8) 3 4)
    g8 g d d g g a( bes g) |
    #(set-time-signature 5 8 '(3 2))
    a4. g4
  }
  \layout {
    \context {
      \Staff
      \consists "Measure_grouping_engraver"
    }
  }
}
