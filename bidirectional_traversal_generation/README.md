## A pseudo-random, bi-directional approach to a carving algorithm
___

Here we can see a different approach that produces a more random, non-characteristic result, instead of the more 'procedural' approach that was implemented before. The main principle lies beneath the pseudo-usage of a *directional vector*. 

Let's say we started from `(x_0, y_0)` co-ordinates. By generating a random directional bias in the range of `[-1, 1]`. If the resulting position, by moving our pixels with the given bias, is within bounds `[0,0] <-> (width, height)`, we can declare our current coordinates as visited and set our current position as a sum of our *previous co-ordinates* and the *directional bias*. 

The main drawback of this approach is the average-case complexity induced by the randomness factor we implemented: not many times, will the iteration result in *the visit of a previously-visited pixel*, so our `iterations` parameter will not strictly define the amount of uniquely-visited pixels. The solutional approach to this is not quite simple: we can declare a `vector` which will contain our directional biases `[-1, 1]` and iterate until we reach a new pixel using those given biases. Like in our previous approach, we can move in compass-like directions. However, the problem that occurs again are the corner-cases: if we are located at a corner and all of our neighbouring pixels are occupied, we must use some sort of a pathfinding algorithm (*such as A-star via Manhattan distance*) to find the closest, non-visited pixel and initialize our currente position as such.

___

## Examples
```
dimension: 1024x1024
box blur filter strength: 2
lower threshold limit: 71
upper threshold limit: 255 
```

<table>
  <tr>
    <td><img src="./more_examples/1024x1024_cave1.bmp" width="100%" height="100%"></td>
    <td><img src="./more_examples/blurred_1024x1024_cave1.bmp" width="100%" height="100%"></td>
    <td><img src="./more_examples/binary_blurred_1024x1024_cave1.bmp" width="100%" height="100%"></td>
    <td><img src="./more_examples/adaptive_blurred_1024x1024_cave1.bmp" width="100%" height="100%"></td>
  </tr>
  <tr>
    <td><img src="./more_examples/1024x1024_cave2.bmp" width="100%" height="100%"></td>
    <td><img src="./more_examples/blurred_1024x1024_cave2.bmp" width="100%" height="100%"></td>
    <td><img src="./more_examples/binary_blurred_1024x1024_cave2.bmp" width="100%" height="100%"></td>
    <td><img src="./more_examples/adaptive_blurred_1024x1024_cave2.bmp" width="100%" height="100%"></td>
  </tr>
</table>