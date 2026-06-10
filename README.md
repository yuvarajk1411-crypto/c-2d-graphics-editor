# 2D Graphics Editor in C

A lightweight, menu-driven 2D graphics editor written in C. It uses a 2D character array as a canvas to draw, modify, and delete geometric shapes via the command line.

## Features
* Draw Lines, Rectangles, Circles, and Triangles.
* Uses Bresenham's Line Algorithm and the Midpoint Circle Algorithm.
* Delete and Modify active shapes using a simulated Scene Graph.

## How to Run
1. Compile the code using GCC:
   `gcc editor.c -o editor`
2. Run the executable:
   * **Linux/macOS:** `./editor`
   * **Windows:** `editor.exe`
