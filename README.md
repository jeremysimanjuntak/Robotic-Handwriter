# Robotic-Handwriter
 
SOFTWARE DESCRIPTION

User inputs:
Text File ‘test.txt’ – the text to be written out by the 2D plotter

Other inputs:
Font data for all ASCII characters ‘SingleFontStroke.txt’ – text file containing the font data for each ASCII characters. Each row of the data represents a single movement, containing information as to whether the pen position is up or down, the x-axis position and the y-axis position

Software description:
1. Begins by checking the RS232 port, which is responsible for sending the G-code to the Arduino and 2D plotter, to ensure that it can be opened and is ready for use
2. Wakes up the writing robot from sleep by sending a new line
3. Sends a few G-code commands to the robot to set it in position, ready to begin writing
4. Receives text file, regardless of length or arrangement of characters
5. Receives Font data, which are font stroke instructions for the robot writer, corresponding to every ASCII character.
6. Reads font data from SingleFontStroke.txt into a structured array, ready for accessing
7. Reads each character from the text file, converts it to ASCII form, then draws out the associated font data and prints it to the buffer in G-code form.
8. Buffer fully loads, with Arduino having sent all the G-code to the 2D plotter, resulting in writing out of text from text file

PROJECT FILES

Source code – file written in C programming language.
Main.c – contains main source code
Serial.c – for utilising serial port
Rs232.c – for RS232 library
Header files - allow commonly used functions to be written only once and referenced by other source files when needed
Serial.h – this program may need to be changed depending on the COM port used. As stated in the file, the number entered into the code as the port number is given as (port number – 1)
RS232.h – for RS232 library

Functions
Function for reading font data into a structured array
Int ReadFontDataFunction (struct Fontdata FontArr)

Parameters
FontArr – structured array which font data will be stored in
Return value – returns 1 if SingleStrokeFont.txt fails to read, 0 if successful

Function for generating G-code
Char GCodeGenerator (struct Fontdata FontArr, int row, struct Coordinates Coord, const float sizedown, char buffer)

Parameters:
FontArr – structured array which font data will be stored in
Row – the row of data within SingleStrokeFont.txt which is being accessed for the letter
being inspected
FirstFontDataLine – the row number where the font data for the specific letter begins
FontDataLinesLeft – number of rows of data remaining after the first row, for the relevant
letter being inspected
Coord – Array which coordinate offsets for x-axis and y-axis positions are being stored. This
ensures letters are written next to each other and that a new line is started from the correct
position
Sizedown – a value of 0.278, which scales down the dimensions of the writing for the 2D
plotter
Buffer – where G-code is stored before the writing instructions are executed by the robot
Return value – returns 0 if successful

