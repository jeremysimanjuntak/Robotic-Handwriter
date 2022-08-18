#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <windows.h>
#include "rs232.h"
#include "serial.h"

#define bdrate 115200               /* 115200 baud */

//Define variables
int row;
char letter;
FILE*ReadText;
int ascii;
int FontDataLinesLeft;
int FirstFontDataLine;
int g;
const float sizedown = 0.278;

char filename[100];

void SendCommands (char *buffer );

//Define Structure for font data
struct Fontdata
    {
        int first, second, third; /* any valid type allowed */
    };

// Define coordinate structure
struct Coordinates
    {
        int xoffset, yoffset;
    };

// Function for reading font data
int ReadFontDataFunction (struct Fontdata *FontArr)
{
    FILE*ReadFont;
    ReadFont = fopen("SingleStrokeFont.txt", "r" );

    // Error checking
    if (ReadFont == NULL)
    {
        printf("\n Fontdata could not be opened");
        return (-1); //Exit as failure
    }

    // Reading into structured array
    for (row=0;row<1027;row++)
    {
        fscanf(ReadFont, "%d %d %d ", &FontArr[row].first, &FontArr[row].second, &FontArr[row].third);
    }

    fclose(ReadFont);

    return (0); //Indicates function has executed without errors
}

// Function for generating G-code
char GCodeGenerator (struct Fontdata *FontArr, int row, struct Coordinates *Coord, const float sizedown, char *buffer)
{
    // Set n as the third value in this structure (the value which tells us how many more lines of font data are needed)
    FontDataLinesLeft = FontArr[row].third;

    // For the case where we need a new line (i.e. 'Enter' ASCII has been registered)
    if ((FontArr[row].first == 999) && (FontArr[row].second == 10))
    {
                        Coord[g].xoffset = -18;
                        Coord[g].yoffset = (Coord[g].yoffset-36);
    }

    int FirstFontDataLine = row;

    for (row=FirstFontDataLine+1;row<=FirstFontDataLine+FontDataLinesLeft;row++)
    {
        // If the pen position for this row of font data is the same as the row beforehand
        if ((FontArr[row].third == FontArr[row-1].third))
        {
            sprintf (buffer, "G%i X%f Y%f\n", FontArr[row].third, (float) (((FontArr[row].first) + (Coord[g].xoffset))*sizedown), (float) (((FontArr[row].second) + (Coord[g].yoffset))*sizedown));
            SendCommands(buffer);
        }
        // If pen position is down
        else if ((FontArr[row].third == 1))
        {
            sprintf (buffer, "S1000\nG%i X%f Y%f\n", FontArr[row].third, (float) (((FontArr[row].first) + (Coord[g].xoffset))*sizedown), (float) (((FontArr[row].second) + (Coord[g].yoffset))*sizedown));
            SendCommands(buffer);
        }
        // If pen position is up
        else if ((FontArr[row].third == 0))
        {
            sprintf (buffer, "S0\nG%i X%f Y%f\n", FontArr[row].third, (float) (((FontArr[row].first) + (Coord[g].xoffset))*sizedown), (float) (((FontArr[row].second) + (Coord[g].yoffset))*sizedown));
            SendCommands(buffer);
        }
    }

    //If  a new line was created, adjust the x-axis offset in the coordinate structured array accordingly
    if ((FontArr[FirstFontDataLine+FontDataLinesLeft].second != 10))
    {
        Coord[g].xoffset =  Coord[g].xoffset + 18;
    }

    return 0;
}

int main()
{
    //char mode[]= {'8','N','1',0};
    char buffer[100];

    //If we cannot open the port then give up immediately
    if ( CanRS232PortBeOpened() == -1 )
    {
        printf ("\nUnable to open the COM port (specified in serial.h) ");
        exit (0);
    }

    //Time to wake up the robot
    printf ("\nAbout to wake up the robot\n");

    // We do this by sending a new-line
    sprintf (buffer, "\n");

    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    Sleep(100);

    // This is a special case - we wait  until we see a dollar ($)
    WaitForDollar();

    printf ("\nThe robot is now ready to draw\n");

    //These commands get the robot into 'ready to draw mode' and need to be sent before any writing commands
    sprintf (buffer, "G1 X0 Y0 F1000\n");
    SendCommands(buffer);
    sprintf (buffer, "M3\n");
    SendCommands(buffer);
    sprintf (buffer, "S0\n");
    SendCommands(buffer);

    //Define structured array
    struct Fontdata FontArr[1027];

    //Define coordinate structure
    struct Coordinates Coord[1];
    Coord[g].xoffset = 0;
    Coord[g].yoffset = 0;

    //Call function for reading data into structured array
    ReadFontDataFunction (FontArr);

    //Open text file for reading, with error checking
    ReadText = fopen("test.txt", "r" );

    if (ReadText == NULL)
    {
        printf("\n Text could not be opened");
        return (-1); //Exit as failure
    }

    // Read contents from text file letter by letter
    while (letter != EOF)
    {
        letter = fgetc(ReadText);
        int ascii = letter;

        // For every line in the Font data array...
        for (row=0;row<1027;row++)
        {
            // If one of the lines begins with 999, and the second term is the ASCII number for the letter to be written...
            if ((FontArr[row].first == 999) && (FontArr[row].second == ascii))
            {
                    GCodeGenerator (FontArr, row, Coord, sizedown, buffer);
            }
        }
    }

    fclose(ReadText);
    return 0;

    // Before we exit the program we need to close the COM port
    CloseRS232Port();
    printf("Com port now closed\n");

    return (0);
}

// Send the data to the robot - note in 'PC' mode you need to hit space twice
// as the dummy 'WaitForReply' has a getch() within the function.
void SendCommands (char *buffer )
{
    // printf ("Buffer to send: %s", buffer); // For diagnostic purposes only, normally comment out
    PrintBuffer (&buffer[0]);
    WaitForReply();
    //WaitForReply();
    Sleep(100); // Can omit this when using the writing robot but has minimal effect
    // getch(); // Omit this once basic testing with emulator has taken place
}
