# The Tesseract

<img src="images/Purple.jpg" alt="Finished Project" width="300"/>

## Introduction
This project is my DIY take on the popular LED cube. I have always been fascinated by the mesmerizing patterns and animations that can be created with these cubes, so I used my school seminary work as an oporunity to build one myself and decided to turn in into a fully open-source project and guide for anyone who wants to build their own.

## My Design
I decided to build a 10^3 cube, which means it has 1000 8mm WS2812 LEDs. I was thinking about making it bigger (12^3), but I wanted to keep the cost reasonable. In hindsight, if I had known how much time and effort it would take to build this cube, I would have gone for a much smaller one. But I am happy with the result, and I learned a lot in the process.<br>
Most of the frame is make of custom PETG 3D printed parts, with some aluminum profiles and HDF board for structural support.<br>
For the brains of the cube, I used a teensy 4.1 microcontroller, which is a very powerful board that can handle the all of the LEDs and peripherals with ease.<br>
The user interface consists of a 4" TFT touch screen and a reset button (just in case).<br>
In the end the total cost was around 750 USD (720€), which is a lot, but I spent a lot of money on prototypes that ended up in the garbage, so I think it could be done for around 450 USD (430€) if you know what you are doing beforehand.

## Features
- 10 animations
- VU meter (audio reactive effect)
- TETRIS game via SNES controller
- Text rendering
- aprox. 300fps (effect dependent)


## How to build it

> [!TIP]
> - Refer to the CAD files and schematics, I tried to make them as detailed as possible.
> - Test the LEDs before soldering them, I know it is a pain (it took me 7 hours), but it is worth it.
> - Use the printed jigs (more on that later) to make the assembly easier.
> - **Turn off the power supply when working on the cube**, I know it is obvious, but I fried more than 30 LEDs after the cube was put together and resoldering them was a nightmare.
> - A good soldering iron helps a lot.


### Step 1: The LED structure
The cube is split into walls and layers. Each wall has 100 LEDs, in 10 columns with alternating directions. Every wall is flipped 180º from the previous one. This results in a zig-zag pattern when viewed from the top. Every layer has its own data connection, to the MCU (more on that later). The walls are inserted into base plates and soldered together.

*Photo from the top here*
*Photo from the side here*

### Step 2: Preparing the wire
The LED connections are made with 0.8mm diameter silver copper wire. I got mine in 13m spools, and I ended up using 9 of them. The wire needs to be straightened and cut.<br>
Use a drill and a wise (or better a helper and pliers) to hold, pull and twist the wire (changing the direction every 8sec) until it is straight. Then use the cutting jig to cut the wire into 21mm pieces with wire cutters. For the collum power wires, cut 100 pieces of around 450mm (I used 500mm and it was too much, but I wanted to be safe).<br>

### Step 3: Soldering the LEDs
**Print the jigs**, soldering is practically impossible without them. 
There are 2 jigs (for the odd and even walls), each consisting of 4 parts that interlock like jig-saw puzzle. The 3d prints are not very heat resistant so I ended up melting them after 2-3 layers so I reprinted all of them 3 times to make the assembly easier.

<img src="images/Jig.jpg" alt="Jig" width="240"/>

Insert the LEDs into the jig in the right orientation, bend the outer legs to their respective sides and the inner two legs up.<br>
Cut the legs 2mm from the LED body and solder a connection wire to the outer legs between all of the LEDs. You should end up with 90 connections in total.<br>
Then solder the longer power wire to the inner legs, connecting all of the LEDs in the column. You should end up with 10 connections in total.<br>

Repeat this process 5 times for each jig, to end up with 10 walls.<br>

<img src="images/Wall.jpg" alt="Wall" height="300"/>
<img src="images/Walls.jpg" alt="Walls" height="300"/>

*This looks quick and easy, but it took me around 3 months (working after school and on weekends) to finish all of the walls.*

### Step 4: Making the base plates
The base plates are made of two layers: a 3mm HDF board and a 3D printed grid. And they are split into 4 parts to make the manufacturing easier.

<img src="images/HDF_base.jpg" alt="HDF base" height="240"/>
<img src="images/Printed_base.jpg" alt="Printed base" height="240"/>

Cut 4 HDF boards to 175x175mm *drill holes for the LEDs*. The 4 plates are identical, but each is mirrored to the other. <br>
The drilling has to be precise, so I used a cheap 3018 CNC with a 1mm drill to make the HDF boards. You can find the excelon files in the CAD folder.<br>

<img src="images/HDF_drilling.jpg" alt="Drilling" height="240"/>

The 3D printed boards are just for looks and you can pain the HDF boards instead, but I think the 3D printed boards make the cube look more professional. I wasnt sure about the accuracy of the 3D printer for such small holes, so I made the holes Only on the bottom really small and drilled them on a drill press to the right size (0.8mm).<br>
I also picked a nice looking top infill pattern in the slicer to make the cube look better.

The front left plates need 10 holes in the front left corner for the data connections, I decided to make those by hand with a drill press, but you can also use the CNC for that.<br>

<img src="images/Bottom_base_data.jpg" alt="Printed base data" height="240"/>

### Step 5: Wiring the base plates

You might have noticed that the polarity of the columns is a mess. This is to make the data connections easier and allow for more even current distribution.

<img src="images/Bottom_base_glueing.jpg" alt="Bottom base glueing" height="240"/>

I used 1.5mm2 wire and a lot of solder to make what I call the power planes. Each corner consists of two "power forks", one for +5V and one for GND. The silvered copper wire of each collum is directly soldered to the power planes.

> [!CAUTION]
> Pay attention to the polarity of the LEDs
