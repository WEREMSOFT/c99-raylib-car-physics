# c99-raylib-car-physics
Car physiscs made with raylib, physiscs are hand made. Insipred on the game "The art of rally".

Split screen and local "multiplayer"

Check the [online demo](https://weremsoft.github.io/c99-raylib-car-physics/)

![alt text](https://github.com/WEREMSOFT/c99-raylib-car-physics/blob/master/doc_assets/demo.gif "Logo Title Text")

# How Physics Work

The physics on this game is based on somethign called "Verlet Integration". The original article where I learned it it was in the site gamasutra.com. The article was called [Advanced Character Physics](https://www.gamasutra.com/view/feature/131313/advanced_character_physics.php).

This article was writen waaay back in the days. Where flash where a thing. The author created the dead body physics simulation for the game "Hitman: Codename 47"(the original hitman).

The article was very clear, today the text still remain, but the images where lost. So big part of the explanation is missing. There are a lot of articles in internet about it, but none is as clear as that one. Digging a little one, I found the paper in [pdf format](https://www.cs.cmu.edu/afs/cs/academic/class/15462-s13/www/lec_slides/Jakobsen.pdf) (grab it before its too late!!).

Anyway, I was requested to explain a little more how this method works, so I'll try to highlight the basics leaving to the reader wander in the details of the original article.

## Why Verlet Integration is a Thing.

Because it allows you to calculate the trajectory of a particle without the velocity, wich is cool, because you don't have to recalculate the velocity when the particle collides with something.

## The Formula

The secret relies in saving the previous position of the particle, and calculate the velocity vector substracting the last position agains the current position. This difference gives us another vector, that when we add to the current position give us the next position of the particle. Confusing? Let simplify it like this:

> The future position is the current position, plus the difference of the current position and the previous position.

So, let's calculate this only for the X coordinate of the particle:

```
x += x - xp;
```

Where:
x: particle x position.
xp: particle previous x position.

As simple as that. The algorithm is just a little more complicated. Because we need to save the current position before update it and then set the previous position with it.

```
xtemp = x;
x += x - xp;
xp = xtemp;
```

Let's try it with numbers: previous position to 0, actual position to 10.

```
xp = 10;
x = 20;

function particle_update()
    xtemp = x;
    x += 20 - 10; // x is now 30
    xp = xtemp; // xp now 20

```

To calculate the particle in 2D, we just do the same for the y coordinate:

```
xtemp = x;
x += x - xp;
xp = xtemp;

ytemp = y;
y += y - yp;
yp = ytemp;
```

This can be simplified using functions to handle vectors:

```
Vector position = new Vector();
Vectpr previous_position = new Vector();

function update()
    Vector position_temp = position;
    position = VectorAdd(position, VectorSubstract(position, previous_position));
```

Here, vector can be on 2D or 3D. The code remains the same.


## Calculating Collision Inside a Box

In order to constrain the particel inside a box, we use the min/max functions.

Let's do it only with the X coordinate. The position of the box is (0, 10) and the size of the box is (100, 200)

```
function restrict_particle(x, y)
    x = max(0, min(x, 100));
    y = max(10, min(y, 200));

```

Again, we can do it for the vector in one step
```
function restrict_particle(position)
    position = VectorMax([0, 10], VectorMin(position, [100, 200]);
```

Done. We don't update the previous position, this will done automatically.

This way we can have very realistics physiscs with a minimal code footprint.