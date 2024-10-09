# Core Native GBA

> create your own game-engine with just lua for game boy advance.

## How to build

 * i will recommend using the [devkitpro/devkitarm](https://hub.docker.com/r/devkitpro/devkitarm) container if you do not have [devkitpro](https://devkitpro.org/wiki/devkitPro_pacman) installed.
 
```
git clone https://github.com/gamelly/core-native-gba
```

```
cmake -Bbuild -H. -DGAME=/vendor/engine/examples/pong/game.lua
```

```bash
make -C build
```

## Run

```
VisualBoyAdvance build/GBA_Game.gba
```
