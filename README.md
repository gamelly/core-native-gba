# Core Native GBA

[<img align="right" height="168px" src="https://raw.githubusercontent.com/RodrigoDornelles/RodrigoDornelles/refs/heads/master/media/pong-gba-gly-engine.gif">](#core-native-gba)

This project is a basic micro-gameengine to run lua in the **Nintendo Game Boy Advanced**,
which can be expanded to a custom engine, such as [Gly Engine](https://github.com/gamelly/gly-engine) or [Love2d](https://github.com/gamelly/love-engine).

 * [online ide](https://playground.gamely.com.br)
 * [limitations](https://docs.gamely.com.br/limits#limits-in-gameboy-advanced)
 * [documentation](https://docs.gamely.com.br/group__homebrew#nintendo_gba)

<br/>

| :warning: Attention |
| :------------------ |
| there is no sanitization, error handling or observation of resource consumption, all of this takes up precious CPU time, bad code will make the game crash with a black screen.<br><br>**Developing a multiplatform game with desktop support is recommended, especially for testing Lua code!** |

## Building: [Pong](https://github.com/gamelly/gly-engine/blob/main/samples/pong/game.lua) Example with `Gly Engine`

```sql
cmake -Bbuild -H. -DGAME=@pong
```

```sql
make -C build
```

## Building: [Pong](https://github.com/gamelly/love-engine/blob/main/samples/pong/main.lua) Example with `Love2D`

```sql
cmake -Bbuild -H. -DGAME=@love:pong -DENGINE=@love
```

```sql
make -C build
```

## Building: Your Own Game with a Self-Made Engine

```sql
cmake -Bbuild -H. -DGAME=path/game.lua -DENGINE=path/engine.lua
```

```sql
make -C build
```

---

:whale: use [devkitpro/devkitarm](https://hub.docker.com/r/devkitpro/devkitarm) docker image if you do not have [devkitpro](https://devkitpro.org/wiki/devkitPro_pacman) installed.
