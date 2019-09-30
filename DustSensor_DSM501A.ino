/*
 * spec file : https://publiclab.org/system/images/photos/000/003/726/original/tmp_DSM501A_Dust_Sensor630081629.pdf
 * 
 * TABLE 1.1. PINOUT I/O DESCRIPTION
 * Pin  |  number Pin  |  name Description
 *  #1  |  Control 1   |  Vout 1 control
 *  #2  |  Vout 2      |  Vout 2 output
 *  #3  |  Vcc         |  Positive power supply
 *  #4  |  Vout 1      |  Vout 1 output
 *  #5  |  GND         |  Ground
 * 2.1.
 * Vout 2 The Vout 2 is normal output port. The sensitivity of Vout 2 pin ispreset up. This port is detectparticle which is over 1 micrometer.
 * 2.2.
 * Vout 1 Use this pinwhen sensitivity of Vout 2 issensitive.The sensitivity of Vout 1 is dull than Vout 2 about 2.5times.
 * (Vout 1’s sensitivityx 2.5times= Vout 2’s sensitivity)It means sizeof particleis 2.5 micrometers.
 * 
 * Reference
 *  1) https://github.com/empierre/arduino/blob/master/DustSensor_SamYoung_DSM501.ino
 *  2) http://learn.linksprite.com/arduino/shields/use-dust-sensor-dsm501-with-arduino/
 *  3) https://m.blog.naver.com/steamedu123/221369188967
 */

#include <string.h>
//byte buff[2];
//int pin = 8;//DSM501A input D8
#define DUST_SENSOR_PM10_PIN 8 /*connect to Vout 2, Pin#2*/
#define DUST_SENSOR_PM25_PIN 7 /*connect to Vout 1, Pin#4*/

unsigned long duration;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 30000; //30sec
unsigned long lowpulseoccupancy = 0;
float ratio = 0.0;
float concentration = 0.0;
float ppmv = 0.0;
float pcsPerCF = 0.0;
float ugm3 = 0.0;//(㎍/㎥)

int temperature = 20; //external temperature, if you can replace this with a DHT11 or better

//int i = 0;
void setup()
{
	Serial.begin(115200);
	pinMode(DUST_SENSOR_PM10_PIN, INPUT);
	//pinMode(DUST_SENSOR_PM25_PIN, INPUT);
	starttime = millis();
  Serial.println("settup");
}

void loop()
{

	/*read PM10*/
	duration = pulseIn(DUST_SENSOR_PM10_PIN, LOW);
	lowpulseoccupancy += duration;
	endtime = millis();
	if ((endtime - starttime) > sampletime_ms)
	{
 #if 0
		ratio = (lowpulseoccupancy - endtime + starttime + sampletime_ms) / (sampletime_ms * 10.0); // Integer percentage 0=>100
		concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;				// using spec sheet curve
		Serial.print("lowpulseoccupancy:");
		Serial.print(lowpulseoccupancy);
		Serial.print("\n");
		Serial.print("ratio:");
		Serial.print("\n");
		Serial.print(ratio);
		Serial.print("DSM501A:");
		Serial.println(concentration);
		Serial.print(";\n\n");


		//ppmv=mg/m3 * (0.08205*Tmp)/Molecular_mass
		//0.08205   = Universal gas constant in atm·m3/(kmol·K)
		ppmv = (float)(((concentration * 0.0283168) / 100) * ((0.08205 * temperature) / 0.01)) / 1000;
		Serial.print("ppm : ");
		Serial.println(ppmv, 3);
#else
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    pcsPerCF = concentration * 100;
    ugm3 = pcsPerCF / 13000;

    Serial.print("PM2.5/ugm3 : ");
    Serial.println(ugm3, 3);
#endif

		lowpulseoccupancy = 0;
		starttime = millis();
	}

	delay(1000);
}
