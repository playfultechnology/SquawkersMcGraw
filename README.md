# SquawkersMcGraw
Reverse engineering and remixing the Hasbro FurReal Friend "Squawkers McGraw" animatronic parrot
<img src="Squawkers_McGraw.jpg" width="300" alt="Squawkers McGraw Animatronic Parrot" />


## Internals
<table>
<tr>
<td><img src="images/head.jpg" width="300" alt="Squawkers McGraw Head" /></td>
<td><img src="images/mainboard_front.jpg" width="300" alt="Squawkers McGraw Mainboard" /></td>
<td><img src="images/mainboard_back.jpg" width="300" alt="Squawkers McGraw Mainboard (back)" /></td>
</tr>
</table>
Squawkers features a variety of switches and sensors:
- Light sensor (head), connected via Red, Black, Grey, White wires.
- Tongue switch (head), connected via 2xBlue wires
- Eyelid limit switches, connected via 2xOrange and 2xPurple wires.

Eyelid and mouth movement are both controlled from the same DC hobby motor, connected via slightly thicker Blue and Purple wires.

## Infrared Controller
<img src="Squawkers_Remote.png" width="300" alt="Squawkers McGraw Contoller" />

The controller has a three position switch, three play mode buttons, 6 program buttons, and a custom record button. It appears to broadcast command strings using the common 38kHz carrier frequency. The following command strings were grabbed using an Arduino and the InfraRed signal analyser code described at https://www.patreon.com/posts/infrared-puzzle-28343366

### Response Mode
```
// Repeat Button
unsigned int repeatData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,1000,2000,2000,1000,1000,2000,2000,1000,1000};

// Dance (same in every mode)
unsigned int danceData[17] = {3000,3000,1000,2000,2000,1000,1000,2000,2000,1000,2000,1000,1000,2000,2000,1000,1000};

// Reset (same in every mode)
unsigned int resetData[17] = {3000,3000,1000,2000,2000,1000,2000,1000,1000,2000,1000,2000,2000,1000,1000,2000,1000};

// Buttons
unsigned int aData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,1000,2000,2000,1000,1000,2000,2000,1000,1000};
unsigned int bData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,2000,1000,1000,2000,2000,1000,1000,2000,1000};
unsigned int cData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,2000,1000,2000,1000,2000,1000,2000,1000,1000};
unsigned int dData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,1000,2000,2000,1000,1000,2000,1000,2000,1000};
unsigned int eData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,2000,1000,1000,2000,1000,2000,2000,1000,1000};
unsigned int fData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,2000,1000,2000,1000,2000,1000,1000,2000,1000};

// Custom Record
unsigned int customRecordData[17] = {3000,3000,1000,2000,2000,1000,2000,1000,1000,2000,2000,1000,2000,1000,2000,1000,1000};
```
### Command Mode
```
// Repeat Button (same for Command and Gags)
unsigned int repeatData[17] = {3000,3000,1000,2000,2000,1000,1000,2000,1000,2000,1000,2000,2000,1000,2000,1000,1000};

// Dance (same in every mode)
unsigned int danceData[17] = {3000,3000,1000,2000,2000,1000,1000,2000,2000,1000,2000,1000,1000,2000,2000,1000,1000};

// Reset (same in every mode)
unsigned int resetData[17] = {3000,3000,1000,2000,2000,1000,2000,1000,1000,2000,1000,2000,2000,1000,1000,2000,1000};

// Buttons
unsigned int aData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,1000,2000,2000,1000,2000,1000,1000,2000,1000};
unsigned int bData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,2000,1000,1000,2000,2000,1000,2000,1000,1000};
unsigned int cData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,1000,2000,1000,2000,1000,2000,1000,2000,1000};
unsigned int dData[17] = {3000,3000,1000,2000,1100,2000,2000,1000,1000,2000,2000,1000,1100,2000,2000,1000,1000};
unsigned int eData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,2000,1000,1000,2000,2000,1000,1000,2000,1000};
unsigned int fData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000,1000};

// Custom Record [same as Response]
unsigned int customRecordData[17] = {3000,3000,1000,2000,2000,1000,2000,1000,1000,2000,2000,1000,2000,1000,2000,1000,1000};
```

### Gags Mode
```
// Repeat Button (same for Command and Gags)
unsigned int repeatData[17] = {3000,3000,1000,2000,2000,1000,1000,2000,1000,2000,1000,2000,2000,1000,2000,1000,1000};

// Dance (same in every mode)
unsigned int danceData[17] = {3000,3000,1000,2000,2000,1000,1000,2000,2000,1000,2000,1000,1000,2000,2000,1000,1000};

// Reset (same in every mode)
unsigned int resetData[17] = {3000,3000,1000,2000,2000,1000,2000,1000,1000,2000,1000,2000,2000,1000,1000,2000,1000};

// Buttons
unsigned int aData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,1000,2000,2000,1000,2000,1000,2000,1000,1000};
unsigned int bData[17] = {3000,3000,1000,2000,1000,2000,1000,2000,2000,1000,2000,1000,1000,2000,1000,2000,1000};
unsigned int cData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,1000,2000,1000,2000,1000,2000,2000,1000,1000};
unsigned int dData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,1000,2000,2000,1000,2000,1000,1000,2000,1000};
unsigned int eData[17] = {3000,3000,1000,2000,1000,2000,2000,1000,2000,1000,1000,2000,2000,1000,2000,1000,1000};
unsigned int fData[17] = {3000,3000,1000,2000,2000,1000,1100,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000};

// Custom Record
unsigned int customRecordData[17] = {3000,3000,1000,2000,2000,1000,1000,2000,1000,2000,1000,2000,1000,2000,1000,2000,1000};
```
