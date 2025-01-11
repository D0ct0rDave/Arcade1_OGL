# Arcade1_OGL (2001)
Spacial Arcade Shot'em up prototype. The game atmosphere is based organic structures, and trying to mimic flesh and organisms. It tries to create an environment where a space ship flies inside an organic body fighting against organic entities. Most textures were created using ZBrush.

This started as a prototype project which eventually should became a Game Boy video game. 

As you can see level data is stored as plain "in memory" structs, which are used in realtime. Levels were subdivided in sections, and each section in planes (foreground, background...), and these were subdivided in tiles. There was a tile editor created to generate the level data for the game. If I can find the code I'll add to this repo. At a later stage I was planning to create the image data as c files too, but this was never done.

This project was initially made using DirectDraw, but later was ported to OpenGL and introduced mesh morphing for the backgkound, even if this is not possible to reproduce on the original device the game was planned to run.

For some reason I ended calling the game "Cerulean project" not sure why.
