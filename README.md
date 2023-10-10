# How to use spritegen

## Demo
[Demo Video](spritegen/Demo.mkv)

## Usage

_Accepted format: png, jpeg, jpg, gif_

```
Usage: spritegen <filename> [options]
Options:
  -o <filename>   Output filename
  -l              Watch for changes
  -w <width>      Width of the sprite
  -h <height>     Height of the sprite
  -a <algorithm>  Algorithm to use for resizing
                  NearestNeighbor, Bilinear, Bicubic(default), MitchellNetravali, Lanczos2, Lanczos3
```

## How to use live preview

- Copy the executable to TestingGround folder.
- Save your image there
- Run this command `.\spritegen.exe <name> -o out.sprite -h 50 -l`, replace the `<name>` with your file name
- Start TestingGround project
- The preview will reload everytime you save.
