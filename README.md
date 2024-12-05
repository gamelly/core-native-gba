# Core Native GBA

[<img align="right" height="168px" src="https://raw.githubusercontent.com/RodrigoDornelles/RodrigoDornelles/refs/heads/master/media/pong-gba-gly-engine.gif">](#core-native-gba)

This project is a basic micro-gameengine to run lua in the **Nintendo Game Boy Advanced**,
which can be expanded to a custom engine, such as [Gly Engine](https://github.com/gamelly/gly-engine).

 * [online ide](https://playground.gamely.com.br)
 * [limitations](https://docs.gamely.com.br/limits#limits-in-gameboy-advanced)
 * [documentation](https://docs.gamely.com.br/group__homebrew#nintendo_gba)

<br/>

| :warning: Attention |
| :------------------ |
| there is no sanitization, error handling or observation of resource consumption, all of this takes up precious CPU time, bad code will make the game crash with a black screen.<br><br>**Developing a multiplatform game with desktop support is recommended, especially for testing Lua code!** |

## How to build

use [devkitpro/devkitarm](https://hub.docker.com/r/devkitpro/devkitarm) container if you do not have [devkitpro](https://devkitpro.org/wiki/devkitPro_pacman) installed.

### Clone repo
 
```
git clone https://github.com/gamelly/core-native-gba
```

### Configure project

```
cmake -Bbuild -H. -DGAME=vendor/engine/examples/pong/game.lua
```

### Build cartbridge rom `.gba`

```
make -C build
```

## Run

```
VisualBoyAdvance build/bin/game.gba
```
