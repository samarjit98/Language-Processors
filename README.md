# Language-Processors

This repository contains tools and algorithms used to build compilers.

The following parsers have been implemented and tested on standard grammars:
  1. CYK algorithm (Membership problem and derivation sequence)
  2. Predictive parsing (LL(1) - non recursive)
  3. Simple LR(0) parsing
  4. Canonical LR(1) parsing
  5. LALR parsing by merging states
  6. Operator precedence parsing
  
An extremely simple intermediate code generator has been implemented using Flex and Bison for a subset of C language (declarations, assignment statements, arithmetic expressions, boolean expressions and simple control flow blocks).
  
Please follow the book "Compilers: Principles, Tools and Techniques, by Aho, Ullman, Lam and Sethi" for theoretical reference.

Made with lots of blood, sweat, caffeine and </>. :p (Never liked Compilers at all!!)
