---
layout: default
title: Introduction
parent: The V5 Programming Bible
nav_order: 1
---

# Introduction

Welcome to the V5 Programming Bible, my shot at a comprehensive guide for VEX Robotics programming using the V5 Brain and its associated components. 

My aim with this document is to create a one-stop shop that provides teams new to advanced V5 programming not just a way to get off the ground, but a clear and practical roadmap from absolute basics to advanced autonomous programming. Topics covered will include:

- The best software tools for V5 programming, and how to use them.
- Implementing basic functionality like driver controls.
- Case studies in how *NOT* to do autonomous.
- Basic feedback-based autonomous control with techniques like PID.
- How to leverage the powerful features offered by C++ while avoiding its quirks and pitfalls.
- Creating basic GUIs for autonomous routine selection and debugging.
- The theory and implementation of absolute position tracking (odometry) systems.
- The theory and implementation of advanced movement algorithms, such as odometry-augmented PID and pure pursuit.

In short, everything you as the programmer need to go from zero to (hopefully) hero in VEX. 

## Caveat - Programming Fundamentals

However, this is ***NOT*** a guide to the general fundamentals of programming, which this document assumes you are already familiar with. 

"Programming fundamentals" includes concepts like:

- Variables - `x = 5`, `y = "Hello World!"`.
  - Knowledge of static typing (explicitly specifying the type of a variable, e.g. `int x = 5`) will be helpful, but is not required.
- Control structures - `if`, `else if`, `else`, `switch`.
- Loop structures - `while` and `for`.
- Functions/subroutines - e.g. `def add(x, y):` or `int add(int x, int y)`.
- Basic data structures, such as lists or arrays.
- Syntax - the textual structure and rules you must follow when using a programming language. (This can vary between languages, which is fine - you just need to grasp the basic idea.)

If you are a complete newbie to programming, I would recommend looking for an introductory course online that uses a language like Python or Java. 

- Python is probably the easiest way to learn the above concepts thanks to its simplicity and the wide availability of relevant learning resources, but its syntax and semantics don't translate well to C++.
- A course in a language like Java (or C#, or even C++) will be harder at first, but has the advantage of teaching you C-style syntax and semantics (like static typing) at the same time.

## Other Concepts to Know

Besides programming fundamentals, there are a few mathematical concepts that will be good to know. 

Understanding these things isn't *strictly* needed like with programming fundamentals, but they *will* make your life a lot easier once you get into the more advanced content:

- 2D coordinate and shape geometry, such as using the distance formula or finding the circumference of a circle.
- Basic trigonometry, including radians and the classic function trifecta of sine/cosine/tangent.
- Vectors in the 2D plane.

For learning any of the above, Khan Academy is probably a good place to start.

## A Disclaimer

One last thing before we start - a small disclaimer. 

**I am not a true expert in V5 programming, nor do I claim to be.**

I only seriously did VEX my senior year of high school, during the 2021-2022 Tipping Point season. And the only reason I got as far as I did? My Google-fu skills and relatively large amount of pre-existing programming experience.

What I'm trying to say is that this guide shouldn't be treated like an ultimate authority, since it's not. This guide was originally just a way to pass down as much practical knowledge as possible to the rest of my team; publicizing it was more of an afterthought than anything else.

So, if you find that something works better than what I suggest, go for it. If someone who's been programming VEX robots since the Cortex days offers to lend a hand, you should probably listen to what they have to say first.

Oh, and to any real experts that might be reading this: if you come across something that's just plain wrong - or perhaps was once right, but now isn't - please don't let it stand! Feel free to open an issue on the GitHub repository associated with this site, or even clone the `gh-pages` branch and make a PR with your suggested changes.

With that out of the way, let's begin!