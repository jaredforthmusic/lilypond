;;;; This file is part of LilyPond, the GNU music typesetter.
;;;;
;;;; Copyright (C) 2004--2015 Han-Wen Nienhuys <hanwen@xs4all.nl>
;;;;
;;;; LilyPond is free software: you can redistribute it and/or modify
;;;; it under the terms of the GNU General Public License as published by
;;;; the Free Software Foundation, either version 3 of the License, or
;;;; (at your option) any later version.
;;;;
;;;; LilyPond is distributed in the hope that it will be useful,
;;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;;; GNU General Public License for more details.
;;;;
;;;; You should have received a copy of the GNU General Public License
;;;; along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.

;; TODO:
;;
;; lookup-font should be written in  C.
;;

;; We have a tree, where each level of the tree is a qualifier
;; (eg. encoding, family, shape, series etc.)  this defines the levels
;; in the tree.  The first one is encoding, so we can directly select
;; between text or music in the first step of the selection.
(define default-qualifier-order
  '(font-encoding font-family font-shape font-series))

(define-class <Font-tree-element>
  ())

(define-class <Font-tree-leaf> (<Font-tree-element>)
  (default-size #:init-keyword #:default-size)
  (size-vector  #:init-keyword #:size-vector))

(define-class <Font-tree-node> (<Font-tree-element>)
  (qualifier #:init-keyword #:qualifier  #:accessor font-qualifier)
  (default #:init-keyword #:default #:accessor font-default)
  (children #:init-keyword #:children #:accessor font-children))

(define (make-font-tree-leaf size size-font-vector)
  (make <Font-tree-leaf> #:default-size size #:size-vector size-font-vector))

(define (make-font-tree-node
         qualifier default)
  (make <Font-tree-node>
    #:qualifier qualifier
    #:default default
    #:children (make-hash-table 11)))

(define-method (display (leaf <Font-tree-leaf>) port)
  (for-each (lambda (x) (display x port))
            (list
             "#<Font-size-family:\n"
             (slot-ref leaf 'default-size)
             (slot-ref leaf 'size-vector)
             "#>"
             )))

(define-method (display (node <Font-tree-node>) port)
  (for-each
   (lambda (x)
     (display x port))
   (list
    "Font_node {\nqual: "
    (font-qualifier node)
    "(def: "
    (font-default node)
    ") {\n"))
  (for-each
   (lambda (x)
     (display "\n")
     (display (car x) port)
     (display "=" port)
     (display (cdr x) port))
   (hash-table->alist (font-children node)))
  (display "} }\n"))


(define-method (add-font (node <Font-tree-node>) fprops size-family)
  (define (assoc-delete key alist)
    (assoc-remove! (list-copy alist) key))

  (define (make-node fprops size-family)
    (if (null? fprops)
        (make-font-tree-leaf (car size-family) (cdr size-family))
        (let* ((qual (next-qualifier default-qualifier-order fprops)))
          (make-font-tree-node qual
                               (assoc-get qual fprops)))))

  (define (next-qualifier order props)
    (cond
     ((and (null? props) (null? order))
      #f)
     ((null? props) (car order))
     ((null? order) (caar props))
     (else
      (if (assoc-get (car order) props)
          (car order)
          (next-qualifier (cdr order) props)))))

  (let* ((q (font-qualifier node))
         (d (font-default node))
         (v (assoc-get q fprops d))
         (new-fprops (assoc-delete q fprops))
         (child (hashq-ref (slot-ref node 'children)
                           v #f)))
    (if (not child)
        (begin
          (set! child (make-node new-fprops size-family))
          (hashq-set! (slot-ref node 'children) v child)))
    (if (pair? new-fprops)
        (add-font child new-fprops size-family))))

(define-method (add-font (node <Font-tree-leaf>) fprops size-family)
  (throw "must add to node, not leaf"))

(define-method (g-lookup-font (node <Font-tree-node>) alist-chain)
  (let* ((qual (font-qualifier node))
         (def (font-default node))
         (val (chain-assoc-get qual alist-chain def))
         (desired-child (hashq-ref (font-children node) val)))

    (if desired-child
        (g-lookup-font desired-child alist-chain)
        (g-lookup-font (hashq-ref (font-children node) def) alist-chain))))

(define-method (g-lookup-font (node <Font-tree-leaf>) alist-chain)
  node)

;; two step call is handy for debugging.
(define (lookup-font node alist-chain)
  (g-lookup-font node alist-chain))

;; TODO - we could actually construct this by loading all OTFs and
;; inspecting their design size fields.
(define-public feta-design-size-mapping
  '((11 . 11.22)
    (13 . 12.60)
    (14 . 14.14)
    (16 . 15.87)
    (18 . 17.82)
    (20 . 20)
    (23 . 22.45)
    (26 . 25.20)))

;; Each size family is a vector of fonts, loaded with a delay.  The
;; vector should be sorted according to ascending design size.
(define-public (add-music-fonts node family name brace design-size-alist factor)
  "Set up music a set of optical music fonts.

Arguments:
@itemize
@item
@var{node} is the font tree to modify.

@item
@var{family} is the family name of the music font.

@item
@var{name} is the basename for the music font.
@file{@var{name}-<designsize>.otf} should be the music font,

@item
@var{brace} is the basename for the brace font.
@file{@var{brace}-brace.otf} should have piano braces.

@item
@var{design-size-alist} is a list of @code{(rounded . designsize)}.
@code{rounded} is a suffix for font filenames, while @code{designsize}
should be the actual design size.  The latter is used for text fonts
loaded through pango/@/fontconfig.

@item
@var{factor} is a size factor relative to the default size that is being
used.  This is used to select the proper design size for the text fonts.
@end itemize"
  (for-each
   (lambda (x)
     (add-font node
               (list (cons 'font-encoding (car x))
                     (cons 'font-family family))
               (cons (* factor (cadr x))
                     (caddr x))))

   `((fetaText ,(ly:pt 20.0)
               ,(list->vector
                 (map (lambda (tup)
                        (cons (ly:pt (cdr tup))
                              (format #f "~a-~a ~a"
                                      name
                                      (car tup)
                                      (ly:pt (cdr tup)))))
                      design-size-alist)))
     (fetaMusic ,(ly:pt 20.0)
                ,(list->vector
                  (map (lambda (size-tup)
                         (delay (ly:system-font-load
                                 (format #f "~a-~a" name (car size-tup)))))
                       design-size-alist
                       )))
     (fetaBraces ,(ly:pt 20.0)
                 #(,(delay (ly:system-font-load
                            (format #f "~a-brace" brace)))))
     )))

;; *****************************************************************************
;; custom function for loading a single-size music font
;; *****************************************************************************
(define-public (add-music-font node family name brace factor)
  "Set up a music font without optical sizes.

Arguments:
@itemize
@item
@var{node} is the font tree to modify.

@item
@var{family} is the family name of the music font.

@item
@var{name} is the name for the music font.
@file{@var{name}.otf} should be the music font,

@item
@var{brace} is the name for the brace font.
@file{@var{brace}.otf} should have piano braces.

@item
@var{factor} is a size factor relative to the default size that is being
used.  This is used to select the proper design size for the text fonts.
@end itemize"
  ;; These are dummy numbers that don't actually get used. I just didn't know
  ;; how to get rid of at least one pair without breaking the code
  (define design-size-alist '((20 . 20)))
  (for-each
   (lambda (x)
     (add-font node
               (list (cons 'font-encoding (car x))
                     (cons 'font-family family))
               (cons (* factor (cadr x))
                     (caddr x))))

   `((fetaText ,(ly:pt 20.0)
               ,(list->vector
                 (map (lambda (tup)
                        (cons (ly:pt (cdr tup))
                              (format #f "~a" name)))
                      design-size-alist)))
     (fetaMusic ,(ly:pt 20.0)
                ,(list->vector
                  (map (lambda (size-tup)
                         (delay (ly:system-font-load
                                 (format #f "~a" name))))
                       design-size-alist
                       )))
     (fetaBraces ,(ly:pt 20.0)
                 #(,(delay (ly:system-font-load
                            (format #f "~a" brace)))))
     )))
;; *****************************************************************************

(define (fonts-in-dir dir name brace ext)
  "Iterates over the files in a given directory and collects
    notation and brace fonts. Returns an alist that _can_ have these entries:
    - 'opticals-font (if a font with optical sizes is found)
    - 'font (if a font without optical sizes is found)
    - 'brace (if a matching brace font is found)"
  (let* ((result-tup '())
         (font-list '())
         (font-dir (opendir dir)))
    ;; read all files in the directory
    (do ((entry (readdir font-dir)(readdir font-dir)))
      ((eof-object? entry))
      (set! font-list
            (append font-list (list entry))))
    ;; test for presence of an opticals font
    ;; (we already consider it present if one file is found)
    (if (member (string-append name "-11." ext) font-list)
        (set! result-tup
              (assoc-set! result-tup
                'opticals-font
                (string-append
                 dir "/" name))))
    ;; test for presence of a non-opticals font
    (if (member (string-append name "." ext) font-list)
        (set! result-tup
              (assoc-set! result-tup
                'font
                (string-append
                 dir "/" name))))
    ;; test for presence of a brace font
    (if (member (string-append brace "-brace." ext) font-list)
        (set! result-tup
              (assoc-set! result-tup
                'brace
                (string-append
                 dir "/" brace))))
    (closedir font-dir)
    result-tup))


(define-public setNotationFont
  (define-scheme-function (parser location names)
    (symbol-list?)
    "Set up a music font with or without optical sizes.

Arguments:
@itemize
@item
@var{names} is a font name or a list of font and brace names.
If just one name is provided a corresponding brace font is assumed.
If two names are provided the second name is used for finding the
brace font.  If the @var{fontdir} command line option has been set
this directory is first searched for fonts, otherwise only the
installation's own font directory is used.
The function can load fonts that have optical variants or that
have not.  In any case only the name of the font has to be
provided, which is read case insensitive.

@end itemize"
    (let* (
           ;; define include/search paths
           ;; respect the active backend
           (font-extension
            (cond
              ((eq? 'svg (ly:get-option 'backend))
               "svg")
              ((eq? 'svg-woff (ly:get-option 'backend))
               "woff")
              (else "otf")))
           ;; external font dir, given by the 'fontdir option
           (ext-fontdir
            (let ((fd (ly:get-option 'fontdir)))
              (if fd (symbol->string fd) #f)))
           ;; font root in the LilyPond installation
           (loc-fontdir
            (string-append
             (ly:get-option 'datadir)
             ;
             ; TODO
             ; Check path handling on Windows
             ;
              "/fonts/"
              (if (string=? "otf" font-extension)
                  "otf"
                  "svg")))

           ;; font and brace font names
           ;; font name is first or only element of the names list
           (name (string-downcase (symbol->string (car names))))
           (brace
            (if
             ;; One name: brace font = music font
             (= 1 (length names)) name
             ;; more than one name: 2nd is brace font or 'none'
             ;; (none falls back to "emmentaler"
             (if (eq? (cadr names) 'none)
                 "emmentaler"
                  (string-downcase (symbol->string (cadr names))))))

           ;; actual fonts
           (ext-fonts
            (if ext-fontdir (fonts-in-dir ext-fontdir name brace font-extension) #f))
           (loc-fonts (fonts-in-dir loc-fontdir name brace font-extension))
           (use-opticals-font (or (assoc-ref ext-fonts 'opticals-font)
                                  (assoc-ref loc-fonts 'opticals-font)))
           (use-font (or (assoc-ref ext-fonts 'font)
                         (assoc-ref loc-fonts 'font)))
           (use-brace (or (assoc-ref ext-fonts 'brace)
                          (assoc-ref loc-fonts 'brace)))

           (fonts (create-empty-font-tree))
           (paper (ly:parser-lookup parser '$defaultpaper))
           (staff-height (ly:output-def-lookup paper 'staff-height))
           (pt (ly:output-def-lookup paper 'pt)))

      ;; test and fallback for nonexistent fonts
      (if (not (or use-font use-opticals-font))
          (begin
           (ly:input-warning location
             (format "Notation font \"~a\" not found. Falling back to Emmentaler" name))
           (set! use-opticals-font "emmentaler")))
      (if (not use-brace)
          (begin
           (ly:input-warning location
             (format "Brace font \"~a\" not found. Falling back to Emmentaler" brace))
           (set! use-brace "emmentaler")))

      ;; Load font with opticals or without,
      ;; depending on what has been found
      (if use-opticals-font
          (add-music-fonts fonts 'feta use-opticals-font use-brace '((20 . 20)) (/ staff-height pt 20))
          (add-music-font fonts 'feta use-font use-brace (/ staff-height pt 20)))

      ;; Add default text font (otherwise font tree node would be empty)
      (add-pango-fonts fonts 'roman
        "Century Schoolbook L"
        (/ staff-height pt 20))
      ;
      ; TODO:
      ; Check back when the other default fonts have been added to LilyPond
      ;
      fonts)))


(define-public (add-pango-fonts node lily-family family factor)
  ;; Synchronized with the `text-font-size' variable in
  ;; layout-set-absolute-staff-size-in-module (see paper.scm).
  (define text-font-size (ly:pt (* factor 11.0)))

  (define (add-node shape series)
    (add-font node
              `((font-family . ,lily-family)
                (font-shape . ,shape)
                (font-series . ,series)
                (font-encoding . latin1) ;; ugh.
                )
              `(,text-font-size
                . #(,(cons
                      (ly:pt 12)
                      (ly:make-pango-description-string
                       `(((font-family . ,family)
                          (font-series . ,series)
                          (font-shape . ,shape)))
                       (ly:pt 12)))))))

  (add-node 'upright 'normal)
  (add-node 'caps 'normal)
  (add-node 'upright 'bold)
  (add-node 'italic 'normal)
  (add-node 'italic 'bold))

; This function allows the user to change the specific fonts, leaving others
; to the default values. This way, "make-pango-font-tree"'s syntax doesn't
; have to change from the user's perspective.
;
; Usage:
;   \paper {
;     #(define fonts
;       (set-global-fonts
;        #:music "gonville"  ; (the main notation font)
;        #:roman "FreeSerif" ; (the main/serif text font)
;       ))
;   }
;
; Leaving out "#:brace", "#:sans", and "#:typewriter" leave them at
; "emmentaler", "sans-serif", and "monospace", respectively. All fonts are
; still accesible through the usual scheme symbols: 'feta, 'roman, 'sans, and
; 'typewriter.
(define*-public (set-global-fonts #:key
  (music "emmentaler")
  (brace "emmentaler")
  (roman "Century Schoolbook L")
  (sans "sans-serif")
  (typewriter "monospace")
  (factor 1))
  (let ((n (make-font-tree-node 'font-encoding 'fetaMusic)))
    (add-music-fonts n 'feta music brace feta-design-size-mapping factor)
    (add-pango-fonts n 'roman roman factor)
    (add-pango-fonts n 'sans sans factor)
    (add-pango-fonts n 'typewriter typewriter factor)
    n))

;; *****************************************************************************
(define*-public (create-empty-font-tree)
  (let ((n (make-font-tree-node 'font-encoding 'fetaMusic)))
    n))
;; *****************************************************************************

(define-public (make-pango-font-tree roman-str sans-str typewrite-str factor)
  (let ((n (make-font-tree-node 'font-encoding 'fetaMusic)))
    (add-music-fonts n 'feta "emmentaler" "emmentaler" feta-design-size-mapping factor)
    (add-pango-fonts n 'roman roman-str factor)
    (add-pango-fonts n 'sans sans-str factor)
    (add-pango-fonts n 'typewriter typewrite-str factor)
    n))

(define-public (make-century-schoolbook-tree factor)
  (make-pango-font-tree
   "Century Schoolbook L"
   "sans-serif"
   "monospace"
   factor))

(define-public all-text-font-encodings
  '(latin1))

(define-public all-music-font-encodings
  '(fetaBraces
    fetaMusic
    fetaText))

(define-public (magstep s)
  (exp (* (/ s 6) (log 2))))

(define-public (magnification->font-size m)
  (* 6 (/ (log m) (log 2))))