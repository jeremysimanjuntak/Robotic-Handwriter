# Robotic-Handwriter

Software description:
1. Begins by checking the RS232 port, which is responsible for sending the G-code to the Arduino and 2D plotter, to ensure that it can be opened and is ready for use
2. Wakes up the writing robot from sleep by sending a new line
3. Sends a few G-code commands to the robot to set it in position, ready to begin writing
4. Receives text file, regardless of length or arrangement of characters
5. Receives Font data, which are font stroke instructions for the robot writer, corresponding to every ASCII character.
6. Reads font data from SingleFontStroke.txt into a structured array, ready for accessing
7. Reads each character from the text file, converts it to ASCII form, then draws out the associated font data and prints it to the buffer in G-code form.
8. Buffer fully loads, with Arduino having sent all the G-code to the 2D plotter, resulting in writing out of text from text file

Please read System Manual PDF for detailed project description.
