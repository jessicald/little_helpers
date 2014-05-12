#ifndef LITTLE_HELPERS_H
#define LITTLE_HELPERS_H


#include <Arduino.h>


struct HC595_t
{
    const byte SER, RCLK, SRCLK;

    HC595_t(byte ser_pin, byte rclk_pin, byte srclk_pin)
        :SER(ser_pin), RCLK(rclk_pin), SRCLK(srclk_pin)
    {}
};


struct MBI5026_t
{
    const byte SDI, CLK, LE;

    MBI5026_t(byte sdi_pin, byte clk_pin, byte le_pin)
        :SDI(sdi_pin), CLK(clk_pin), LE(le_pin)
    {}
};


struct HC597_t
{
    const byte _PL, STCP, SHCP, Q;

    HC597_t(byte _pl_pin, byte stcp_pin, byte shcp_pin, byte q_pin)
        :_PL(_pl_pin), STCP(stcp_pin), SHCP(shcp_pin), Q(q_pin)
    {}
};


enum gain_t {x10, x20, x50, x100, x200, x500, x1000};


struct LMP8358_p_t
{
    const byte OUT, G2, G1, G0;

    LMP8358_p_t(byte out_pin, byte g2_pin, byte g1_pin, byte g0_pin, gain_t gain)
        :OUT(out_pin), G2(g2_pin), G1(g1_pin), G0(g0_pin), _gain(gain)
    {}

    void adjust_gain(gain_t gain);
    gain_t auto_gain();

    gain_t get_gain() const;
    

private:
    gain_t _gain;
    const static byte GAIN_CODES[7];
};


float mean_average(const float* values, word n);


float calc_input_voltage(int input);
float calc_output_voltage(int output);

long dither(float f);
long dither(float f, word n);

void print_binary(const word input, const byte length);

void print_bool(boolean input);
void print_bool(boolean input, boolean is_digital);

void print_ordered_pair(boolean one, boolean two, boolean is_digital);
void print_ordered_pair(double one, double two);
void print_ordered_pair(double one, double two, byte precision);

void pulse_pin(byte pin, word milliseconds, boolean active_low);
// void pulse_pin(byte pin, float milliseconds, boolean active_low);

byte hamming_weight(unsigned long i);


#endif
