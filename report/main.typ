#import "@preview/arkheion:0.1.0": arkheion, arkheion-appendices

#show: arkheion.with(
  title: "Computational Geometry (2025) Theory Questions",
  authors: (
    (name: "Diego Barbieri", email: "au802245@uni.au.dk", affiliation: "Aarhus University"),
    (name: "Gioele Scandaletti", email: "au803277@uni.au.dk", affiliation: "Aarhus University"),
    (name: "Samuele Esposito", email: "au803278@uni.au.dk", affiliation: "Aarhus University"),
  ),
  // Insert your abstract after the colon, wrapped in brackets.
  // Example: `abstract: [This is my abstract...]`
  abstract: [
    Answers to the theory questions project for the Computational Geometry course.
  ],
  // keywords: ("Cryptology", "Information Theory", "Entropy"),
)
#set cite(style: "chicago-author-date")
#show link: underline

#line(length: 100%)

#include "chapters/introduction.typ"

#include "chapters/quickhull.typ"

#include "chapters/conclusion.typ"
