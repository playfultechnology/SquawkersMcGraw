# SquawkersMcGraw
Reverse engineering and remixing the Hasbro FurReal Friend "Squawkers McGraw" animatronic parrot

## Infrared Commands
The following command strings were grabbed using an Arduino and the InfraRed signal analyser code described at https://www.patreon.com/posts/infrared-puzzle-28343366

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
