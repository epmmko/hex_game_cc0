# hex_game_cc0
Hex game using Mente Carlo method for AI to decide the next step was created as part of an online C++ course.
Then, I add GUI to make it look better. AI fills the board 1000 time to see which next move will give the highest winning chance.
A spanning tree method is used to check the winning condition (connection between top to bottom & left to right).
The board size can be set upon calling the program. The AI takes longer time (to fill the boards 1000 times) as the board size increases.
The game with AI run fine on the board size of 3x3 to 12x12. Two player mode is suggested for a larger board size like 15x15 to have no wait time for AI<br>

# Compile and run
(On linux) run makefile with `make` command to create all executable files.

# License
The source code is CC0, so use freely with no worry.<br>
The library SDL2 has zlib license, so still worry free even for a closed-source project.

# Acknowledgement
* Thanks to Dr Ira Pohl to teach me C++. I did hex game in his course and decide to add GUI afterword <br>
* lazyfoo site helps me to go through SDL library + installation process https://lazyfoo.net/tutorials/SDL/ <br>
* The SDL2 library was used to create the front-end of this game https://www.libsdl.org/<br>
* Video screen play was captured using Kazam 1.4.5 software https://launchpad.net/kazam <br>
* The video was cropped and sped up by using Kdenlive 19.12.3 https://kdenlive.org <br>

# Possible Additional Fun
Use only the backend of hex game to train DRL agent, then compete with Monte Carlo based AI.

# Appendix
screenshot + videos<br>
![11x11_play_with_AI](https://github.com/epmmko/hex_game_cc0/assets/26897526/7f9c88b9-2fac-4511-80a9-e5933e1d77bc)

<br>run with
`./main 5` (video 2x speed)
https://github.com/epmmko/hex_game_cc0/assets/26897526/f788bff6-d25d-460e-b66e-defcdf30ad26

<br>run with
`./main 11` (video 11x speed)
https://github.com/epmmko/hex_game_cc0/assets/26897526/a4735014-3244-4294-9f2f-0137feeca379



