# Idea

<img width="2076" alt="gameing" src="https://github.com/thng292/CrossyRoady/assets/61302586/0a0a2b98-bf85-4422-b049-0d11984aff31">

# Note:

- Story (progress):
  - Hidden experience system. Sum of player's score. Passing a threshold -> unlock new map and character
  - At first only one map is available -> grind to unlock more
  - After unlock map, play it through a certain threshold will unlock new character.
  - Should have some thing to indicate that the map's character is unlocked or not
- Gameplay:
  - The player can only unlock a character if choose the character's map (random mode does not count)
  - The new character is spawn at safe place and unlock by interact with them. Should show a quick dialouge.
  - If the player choose a specific map to play, they only play on that map
  - Random mode will choose a random map. After reach a certain score threshold, the map will change (Stage up).
  - A counter should be present on the screen.
  - The character's skill can be used when the "power bar" is filled up, in order to fill it up the player must progresses through the map.

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

- Save your image in spritegen folder
- Run this command `.\spritegen.exe <name> -o out.sprite -h 50 -l`, replace the `<name>` with your file name
- Start TestingGround project
- The preview will reload everytime you save.

# Aserpite

Download link: https://drive.google.com/file/d/1NZjfj_aMfF7UfCMYaBuyLuvjbl3pnlSJ/view?usp=sharing

# File formats

## sprite

| attr   | size                  | note                     |
| ------ | --------------------- | ------------------------ |
| width  | 2 bytes               | Bigendian, unsigned      |
| height | 2 bytes               | Bigendian, unsigned      |
| data   | width \* height bytes | image data as color enum |

## palette

| attr | size           | note                                   |
| ---- | -------------- | -------------------------------------- |
| data | 16\*3=48 bytes | 16 RGB color. Use 8 byte for R,G,B. Or |

## anisprite

| attr       | size                                | note                     |
| ---------- | ----------------------------------- | ------------------------ |
| width      | 2 bytes                             | Bigendian, unsigned      |
| height     | 2 bytes                             | Bigendian, unsigned      |
| totalFrame | 4 bytes                             | Bigendian, unsigned      |
| fps        | 1 byte                              | frame per sec            |
| data       | width \* height \* totalFrame bytes | video data as color enum |

## color enum

```cpp
enum class Color : char {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15,
    // Excluded from color palette. Only use in sprite
    C_TRANSPARENT = 0b11111
};
```
