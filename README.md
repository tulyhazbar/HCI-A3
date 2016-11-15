# HCI-A3
Components:
1) Arduino 
2) Photon 
3) Flora Motion Sensors
4) Proximity Sensor 
5) Buzzer
^) LED Strip 


3D Printed parts:
1) Enclosure for the game circuit (Electronic Enclosure. stl )
2) two handle to make it easier to hold the board of the game (handle.Stl) 

Laser cut Parts 
1) the board of the game (game board.ai)
2) the wall holder of the game (Hanger.ai)

Circuits: 
1) the game circuit (My_Game.Ino)
2)The hanger circuit (Case.Ino)



The Game: 
The idea is to make an interactive game using a sensor that can measure the change in the orientation of a board that has a LED strip with one flashing LED fixed on it. This change in the board orientation will be reflected on the direction of motion of the flashing LED. The game is made for two players each one carries the board from one side (Left and Right). Each player will try to change the orientation if the board when the flashing LED moves towards his/her end. If the Flashing LED reaches to one of the ends it means the player who is on the opposite direction won the game.

The Physical structure of the game consists of two parts: 
1) The game board that has the LED Strip fixed on.
2) The hanger that holds the board when nobody is playing the game. 
    
   - The game board 

I have laser cut the board ( see file board.ai ) and I have designed and 3D printed two handle that will be attached to each end of the board to make it easier for the user to hold ( see file handle.stl). I have also designed an enclosure for the photon and the circuit that has the Flora 9DoF sensor. The board will slide in this enclosure ( see picture 1). 
The code which runs on the photon is the My_Game.ino. it controls the LED Strip based on the input of the motion sensor. 

- the hanger

The game will be hanged on a board in the the lab when not in use. I have laser cut a circle that has some holes on it so the the game board can be instead in it. I have added a Proximity sensor and a buzzer to make this part interactive more interactive with the environment. Whenever someone gets close to the hanged game the buzzer will play Super Mario theme Song to grab the attention of the person who passed by the game. 
I have used Arduino to control the circuit of the game holder. The reason why I used a separate controller is because I want these two parts to be complete separated specially that while playing the game the board will be take a way from the holder. 
The code that controls the holder circuit is the Case.Ino. 



In general for this project i have used the housing and sliding techniques to join two objects.
