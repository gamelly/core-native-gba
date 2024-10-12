# Core Native GBA

> create your own game-engine with just lua for game boy advance.

[<img align="right" height="100px" src="https://raw.githubusercontent.com/RodrigoDornelles/RodrigoDornelles/refs/heads/master/media/pong-gba-gly-engine.gif">](#run)

This project is a basic micro-gameengine to run lua in the **Nintendo GBA**,
which can be expanded to a custom engine,
such as [Gly Engine](https://github.com/gamelly/gly-engine).

<br/><br/>

| :warning: Attention |
| :------------------ |
| there is no sanitization, error handling or observation of resource consumption, all of this takes up precious CPU time, bad code will make the game crash with a black screen. I recommend multiplatform games that support desktop especially for testing! |

## How to build

 * i will recommend using the [devkitpro/devkitarm](https://hub.docker.com/r/devkitpro/devkitarm) container if you do not have [devkitpro](https://devkitpro.org/wiki/devkitPro_pacman) installed.

### Clone repo
 
```
git clone https://github.com/gamelly/core-native-gba
```

### Configure project

 * **GAME:** path to your `game.lua`<br/><sup><sub>(can also be an example from the **vendor/engine/examples** folder)</sub></sup>
 * **FPS_MODE 0:** loop 60 fps / draw 60 fps
 * **FPS_MODE 1:** loop 60 fps / draw 30 fps
 * **FPS_MODE 2:** loop 30 fps / draw 30 fps
 * **FPS_MODE 3:** loop 20 fps / draw 20 fps

```
cmake -Bbuild -H. -DGAME=vendor/engine/examples/pong/game.lua -DFPS_MODE=1
```

### Build cartbridge rom `.gba`

```
make -C build
```

## Run

```
VisualBoyAdvance build/GBA_Game.gba
```
