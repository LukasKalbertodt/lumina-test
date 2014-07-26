Lumina Test-Applications
========================

A bunch of applications to test the [Lumina Graphics Library](https://github.com/LukasKalbertodt/lumina) version-0.1. There are different tests included. Currently finished are:

### 1. Mandelbulb Set
It shows a picture of the mandelbulb set, completly calculated in a fragment shader. You can move around with `WASD`, zoom in with `+/-` and change the number of maximal iterations with `arrow-right` and `arrow-left`. You can also toggle coordinate-axis with `space` and toggle "anti-aliasing" with `F`. Anti-Aliasing means: For every pixel I test 4 complex numbers and take the average color.

I tested on a Intel HD4000 and 100 iterations with "anti-aliasing" at 1024*756 were no problem. Of course all calculations are done with float, so you will see ugly artifacts when zooming in long enough.

The implementation of Mandelbulb is just **~200** (C++) + **~100** (Shader) lines of code. 

Mandelbulb with 50 iterations:
![alt text](https://raw.githubusercontent.com/LukasKalbertodt/lumina-test/master/gfx/mandelbulb.png "the whole thing")
Float artifacts:
![alt text](https://raw.githubusercontent.com/LukasKalbertodt/lumina-test/master/gfx/mandel-artifacts.png "float artifacts")

### 2. Game of Life
Conway's Game of Life, calculated with shaders. I'm not really happy with it yet... Still under development.


More applications coming soon...
