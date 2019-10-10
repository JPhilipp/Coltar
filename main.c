#pragma config(Sensor, S2, touchSensorScale, sensorTouch)
#pragma config(Sensor, S3, touchSensorPlay, sensorTouch)
#pragma config(Sensor, S4, colorPort, sensorCOLORFULL)

void colorToSound();
float getFrequencyFromNoteAndOctave(string noteAndOctave);
void displayScale();
void setToNextVolume();
void setToNextScale();
task ButtonTask();

int scale = 1;
const int maxOctaves = 5;
const int maxNotes = 12;
const int notesPerScale = 6;
const int maxVolume = 4;
string scaleNames[4] = {"c", "f", "em", "am"};
bool touchSensorScaleWasReleased = true;

task main() {
    nNxtButtonTask  = ButtonTask;
    displayScale();
    nVolume = maxVolume;

    while (true) {
        if ( SensorValue(touchSensorPlay) == 1 ) {
            colorToSound();
        }
        if ( SensorValue(touchSensorScale) == 1 ) {
            if (touchSensorScaleWasReleased) {
                touchSensorScaleWasReleased = false;
                setToNextScale();
            }
        }
        else {
            touchSensorScaleWasReleased = true;
        }
        wait1Msec(1);
    }
}

void colorToSound() {
    int note = -1;
    string color = "";
    switch (SensorValue[colorPort]) {
        case BLACKCOLOR: note = 0; color = "Black"; break;
        case REDCOLOR: note = 1; color = "Red"; break;
        case GREENCOLOR: note = 2; color = "Green"; break;
        case YELLOWCOLOR: note = 3; color = "Yellow"; break;
        case BLUECOLOR: note = 4; color = "Blue"; break;
        case WHITECOLOR: note = 5; color = "White"; break;
    }

    if (note != -1) {

        string noteAndOctaves_c[notesPerScale] = {"g2","c3","d3","g3","c4","d4"};
        string noteAndOctaves_f[notesPerScale] = {"f2","c3","f3","a3","c4","f4"};
        string noteAndOctaves_em[notesPerScale] = {"e2","b2","d3","g3","b3","e4"};
        string noteAndOctaves_am[notesPerScale] = {"fs2","a2","d3","a3","c4","fs4"};

        string noteAndOctave = "";
	    switch (scale) {
	        case 1: noteAndOctave = noteAndOctaves_c[note]; break;
            case 2: noteAndOctave = noteAndOctaves_f[note]; break;
            case 3: noteAndOctave = noteAndOctaves_em[note]; break;
            case 4: noteAndOctave = noteAndOctaves_am[note]; break;
        }
        float frequency = getFrequencyFromNoteAndOctave(noteAndOctave);
        PlayTone(frequency, 1);

        displayScale();
        nxtDisplayTextLine(3, "Colr %s", color);
        nxtDisplayTextLine(4, "Note %s", noteAndOctave);
        nxtDisplayTextLine(5, "Freq %f", frequency);
    }
}

float getFrequencyFromNoteAndOctave(string noteAndOctave) {
    float frequency = 500;

    float frequencies[maxOctaves * maxNotes] = {
            1975.53,1864.66,1760.00,1661.22,1567.98,1479.98,1396.91,1318.51,1244.51,1174.66,1108.73,1046.50,
            987.767,932.328,880.000,830.609,783.991,739.989,698.456,659.255,622.254,587.330,554.365,523.251,
            493.883,466.164,440.000,415.305,391.995,369.994,349.228,329.628,311.127,293.665,277.183,261.626,
            246.942,233.082,220.000,207.652,195.998,184.997,174.614,164.814,155.563,146.832,138.591,130.813,
            123.471,116.541,110.000,103.826,97.9989,92.4986,87.3071,82.4069,77.7817,73.4162,69.2957,65.4064};

    string noteAndOctaves[maxOctaves * maxNotes] = {
            "b4","as4","a4","gs4","g4","fs4","f4","e4","ds4","d4","cs4","c4",
            "b3","as3","a3","gs3","g3","fs3","f3","e3","ds3","d3","cs3","c3",
            "b2","as2","a2","gs2","g2","fs2","f2","e2","ds2","d2","cs2","c2",
            "b1","as1","a1","gs1","g1","fs1","f1","e1","ds1","d1","cs1","c1",
            "b0","as0","a0","gs0","g0","fs0","f0","e0","ds0","d0","cs0","c0"};

    int i;
    for (i = 0; i < maxOctaves * maxNotes; i++) {
        if (noteAndOctaves[i] == noteAndOctave) {
            frequency = frequencies[i];
            break;
        }
    }

    return frequency;
}

task ButtonTask() {
    switch (nNxtButtonPressed) {
        // case kEnterButton:

        case kRightButton:
            setToNextVolume();
            break;
	}
	return;
}

void setToNextVolume() {
    int newVolume = nVolume + 1;
    if (newVolume > maxVolume) { newVolume = 1; }
    nVolume = newVolume;
    eraseDisplay();
    nxtDisplayTextLine(4, "Volume %d", newVolume);
}

void setToNextScale() {
    const int maxScales = 4;
    scale++;
    if (scale > maxScales) { scale = 1; }
    eraseDisplay();
    displayScale();
}

void displayScale() {
    nxtDisplayBigStringAt(5, 60, scaleNames[scale - 1]);
}
