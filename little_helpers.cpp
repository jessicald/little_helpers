#include <little_helpers.h>


float calc_input_voltage(int input)
{
    return input / 1023.0 * 5.0;
}


float calc_output_voltage(int output)
{
    return output / 255.0 * 5.0;
}



long dither(float f)
{
    /* Uses a simple coin flip to determine how to quantize the input. */

    return (unsigned long) (f + random(2));
}

long dither(float f, word n)
{
    /* Uses a Gaussian dither to quantize the input. */
    
    unsigned long rolls = 0;

    for (word i = 0; i < n; i++)
        rolls += random(101);

    return (unsigned long) (f + ((double) rolls / n - 50));   
}



void print_binary(word input, byte length)
{
    char temp_str1[17], temp_str2[17];
    itoa(input, temp_str1, 2);
    size_t length_diff = length - strlen(temp_str1);
    memset(temp_str2, '0', length_diff);
    temp_str2[length_diff] = '\0';
    sprintf(temp_str2, "%s%s", temp_str2, temp_str1);
    Serial.print(temp_str2);
}

void print_bool(boolean input)
{
    if (input == true)
        Serial.print("TRUE");
    else
        Serial.print("FALSE");
}

void print_bool(boolean input, boolean is_digital)
{
    if (is_digital == true)
    {
        if (input == HIGH)
            Serial.print("HIGH");
        else
            Serial.print("LOW");
    }

    else
    {
        if (input == HIGH)
            Serial.print("TRUE");
        else
            Serial.print("FALSE");
    }
}

void print_ordered_pair(boolean one, boolean two, boolean is_digital)
{
    Serial.print('(');
    print_bool(one, is_digital);
    Serial.print(", ");
    print_bool(two, is_digital);
    Serial.print(')');    
}


void print_ordered_pair(double one, double two)
{
    Serial.print('(');
    Serial.print(one);
    Serial.print(", ");
    Serial.print(two);
    Serial.print(')');    
}


void print_ordered_pair(double one, double two, byte precision)
{
    Serial.print('(');
    Serial.print(one, precision);
    Serial.print(", ");
    Serial.print(two, precision);
    Serial.print(')');    
}

void pulse_pin(byte pin, word milliseconds, boolean active_low)
{
    digitalWrite(pin, !active_low);
    delay(milliseconds);
    digitalWrite(pin, active_low);
}

// void pulse_pin(byte pin, float milliseconds, boolean active_low)
// {
//     digitalWrite(pin, !active_low);
//     delayMicroseconds(milliseconds * 1000);
//     digitalWrite(pin, active_low);
// }

/* https://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer */
byte hamming_weight(unsigned long i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}


/* G2, G1, G0 */
const byte LMP8358_p_t::GAIN_CODES[7] = {B000,  // 10x (power-up default) 930 kHz 000b
                                         B001,  // 20x 385 kHz 000b
                                         B010,  // 50x 460 kHz 001b
                                         B011,  // 100x 640 kHz 010b
                                         B100,  // 200x 195 kHz 010b
                                         B101,  // 500x 130 kHz 011b
                                         B110,  // 1000x 89 kHz 011b
                                         // B111   // User defined 800 kHz 1xxb
};


void LMP8358_p_t::adjust_gain(gain_t gain)
{
    digitalWrite(G2, (GAIN_CODES[gain] >> 2) & 0x1);
    digitalWrite(G1, (GAIN_CODES[gain] >> 1) & 0x1);
    digitalWrite(G0, GAIN_CODES[gain] & 0x1);
}

gain_t LMP8358_p_t::auto_gain()
{
    const word SATURATION = 1015;  // About 4.96 V
    word input;
    char i = 6;
    gain_t gain = x1000;

    adjust_gain(gain);

    while (i > 0)
    {
        input = analogRead(OUT);
        if (input >= SATURATION)
        {
            i--;
            gain = (gain_t) i;
            adjust_gain(gain);
        }
        else
            break;
    }

    _gain = gain;

    return gain;
}

gain_t LMP8358_p_t::get_gain() const
{
    return _gain;
}

float mean_average(const float* values, word n)
{
    float sum = 0;
    for (unsigned long i = 0; i < n; i++)
    {
        sum += values[i];
    }
    return sum / n;
}


void LCC120_switch(byte pin, boolean state, unsigned long time)
{
    digitalWrite(pin, state);
    delay(time);
}
