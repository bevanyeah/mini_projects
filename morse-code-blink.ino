
/*-------------
SIT730 - Task 2.1P First Name Blinky
Bevan Fairleigh
219296864


Morse Code blink

Morse code rules
Duration of a dot is 1 unit
Dash is 3 units
space between components of a letter is 1 unit
space between words is 3 dots


-------------*/



int led2 = D7; // Instead of writing D7 over and over again, we'll write led2

// Delare an array of all codes that we can reference when we submit a string of chars
// For each letter, we have 4 codes.  1 = dot 3=dash 0 =nothing, for those letters that don't have 4 components
const int codes[26][4] = {
        {1,3,0,0}, //A
        {3,1,1,1}, //B
        {3,1,3,1}, //C
        {3,1,1,0}, //D
        {1,0,0,0}, //E
        {1,1,3,1}, //F
        {3,3,1,0}, //G
        {1,1,1,1}, //H
        {1,1,0,0}, //I
        {1,3,3,3}, //J
        
        {3,1,3,0}, //K
        {1,3,1,1}, //L
        {3,3,0,0}, //M
        {3,1,0,0}, //n
        {3,3,3,0}, //O
        {1,3,3,1}, //p
        {3,3,1,3}, //q
        {1,3,1,0}, //r
        {1,1,1,0}, //s
        {3,0,0,0}, //t
        
        {1,1,3,0}, //u
        {1,1,1,3}, //v
        {1,3,3,0}, //w
        {3,1,1,3}, //x
        {3,1,3,3}, //y
        {3,3,1,1}, //z
};

void setup() {

    // Set our led2 as an output
    pinMode(led2, OUTPUT);
    
    // Set our D2 pin as HIGH, waiting for a signal 
    pinMode(D2, INPUT_PULLUP);
    // Attach interrupt to the D2 pin on the falling edge
    attachInterrupt(D2, isr_reset_morse, FALLING);
  
    // Build the pattern fpr the light.  Once built, we don't need to build it again
    build_display_morse();

}


// VARIABLES
// THe word to be converted to morse
const char word[] = "BEVAN";

// How long each 'tick' will take.   ~250 is pretty good
const int multiplier = 250;

// Some global loop positions and reset toggles for reseting and stopping our morse signal
int global_loop = -1;
int push_reset = 0;


// We initialise an array to contain the word in the encoded form, which comprises of at least 4 components per letter.  we also need an end of array signal (+1)
const int pattern_length = 99;
int pattern[pattern_length];



void loop() {
    
    // Loop while we're not at the end of the pattern, and while global_loop is >0

    
    while (pattern[global_loop] != 9 && global_loop != -1) {
        // reset the reset
        push_reset = 0;
    
        // If current pattern is not 0, set led2 to high and look at the pattern to determine delay
        if (pattern[global_loop] > 0 ) {
            digitalWrite(led2, HIGH);
            delay(pattern[global_loop]*multiplier);
        }
        // If current pattern is not 0, set led2 to low and look at the pattern to determine delay
        if (pattern[global_loop+1] > 0 ) {
            digitalWrite(led2, LOW);
            delay(pattern[global_loop+1]*multiplier);
        }
        
        // If reset has been pushed during those delays, simply reset the loop
        if (push_reset==1) {
            global_loop=0;
            push_reset = 0;
        }
        // otherwise, go to the next high/low pair in the pattern
        else {
            global_loop+=2;
        }
    }
    
    // set the global loop to stop the LED flashing for now, and small delay
    global_loop = -1;
    delay(5*multiplier);
    
    
    
}


// ISR to reset the morse code.  Setting global_loop to 0 will start the process, while reset will restart if already in progress
void isr_reset_morse() {
    global_loop = 0;
    push_reset = 1;
}


/*
Function to encode our pattern according to the morse code rules.  

*/
void build_display_morse() {

    int letter_val;
    
    // initialise the array
    for (int i = 0; i < pattern_length ; i++){
        pattern[i] = 0;
    }
    
    pattern[pattern_length-1] = 9;
    // iterate through the word
    for (int i = 0; i < strlen(word) ; i++){
    
        // A = 65 in ascii, so for each letter, we need to subtract 65 from the int value to find the 'code' we should print
        letter_val = word[i] - 65;
        
        pattern[8*i] = codes[letter_val][0];
        
        // Check if the letter has more components
        if (codes[letter_val][1] > 0) {
    
        // if yes, then set a low and high delay in our pattern
            pattern[8*i+1] = 1;
            pattern[8*i+2] = codes[letter_val][1];

            // check if letter has a 3rd component
            if (codes[letter_val][2] > 0) {

                pattern[8*i+3] = 1;
                pattern[8*i+4] = codes[letter_val][2];

                //Finally, check if there is a 4th component
                if (codes[letter_val][3] > 0) {
                    pattern[8*i+5] = 1;
                    pattern[8*i+6] = codes[letter_val][3];
                }
            }
        }

    // Always delay for 3 ticks after the character has finished
    pattern[8*i+7] = 3; // always

    }
}

