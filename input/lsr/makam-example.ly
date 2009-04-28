%% Do not edit this file; it is auto-generated from input/new
%% This file is in the public domain.
%% Note: this file works from version 2.12.0
\version "2.13.1"
\header {
  texidoces = "
El «Makam» es un tipo de melodía de Turquía que
utiliza intervalos microtonales de 1/9 de tono.  Consulte el
archivo de inicio @code{makam.ly} (véase el 'Manual de
aprendizaje @version{}, 4.6.3 Otras fuentes de información' para
averiguar la situación de este archivo) para ver detalles de los
nombres de las notas y las alteraciones.

"
  doctitlees = "El «Makam»"

  
%% Translation of GIT committish :<0364058d18eb91836302a567c18289209d6e9706>
  texidocde = "
Makam ist eine türkische Melodie, in der 1/9-Tonabstände
eingesetzt werden.  Sehen Sie sich die Initialisierungsdatei
@code{makam.ly} für weiter Information zu Tonhöhenbezeichnungen
und Alterationen an (siehe
Handbuch zum Lernen @version{}, 4.6.3 Weitere Information zu
Hinweisen, wo diese Datei gespeichert ist)."
  
  doctitlede = "Makam-Beispiel"

  lsrtags = "pitches,world-music"
  texidoc = "Makam is a type of melody from Turkey using 1/9th-tone
microtonal alterations.  Consult the initialization file
@code{makam.ly} (see the `Learning Manual @version{},
4.6.3 Other sources of information' for the location of this file)
for details of pitch names and alterations."
  doctitle = "Makam example"
} % begin verbatim


% Initialize makam settings
\include "makam.ly"

\relative c' {
  \set Staff.keySignature = #`((6 . ,(- KOMA)) (3 . ,BAKIYE))
  c4 cc db fk
  gbm4 gfc gfb efk
  fk4 db cc c
}
