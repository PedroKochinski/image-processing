# image-processing
PGM Image processing project createt in c to the CI1001 course at Federal University of Paraná

## Filters:
There's a total of 6 filters avaliable: negative, rotation, threshold, noise reduction by mean value and median.

## Compile:
To compile, simply run
```
make
```


## Usage:
The filters accept a pgm P2 or P5 image as input `-i` and returns a pgm P2 or P5 `-o`.

Negative:
```
pgmnega -i input -o output
```
Rotation:
>The `-a` is the angle of rotation, by default is 90º
```
pgmrotacao -a N -i input -o output
```
Threshold:
>The `-l` is the threshold value between 0.0 and 1.0, by default is 0.5
```
pgmlimiar -l N -i input -o output
```
Noise reduction:
```
pgmmedia -i input -o output
```
>The `-m` is the mask dimensions (NxN). The value must be odd > 0, by default is 3
```
pgmmediana -m N -i input -o output
```
