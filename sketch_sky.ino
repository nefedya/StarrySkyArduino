#include <IRremote.h>

#define IRPin 8
#define diodsNum 4
#define maxBright 190
#define stepB 5

IRrecv irrecv(IRPin);
decode_results results;


byte cnstlns[diodsNum] = {0,0,0,0};
byte pins[diodsNum] = {5,6,9,10};
byte cur;

void setup() {

	
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  
  cur = 0; 	
  
  irrecv.enableIRIn(); // запускаем прием
  
  Serial.begin(9600);

}

void loop() {
	
if ( irrecv.decode( &results )) { // если данные пришли
	switch ( results.value ) {
		case 0x707018E7: //+
			brightUp(cur);
			break;
		 case 0x70709867: //-
			brigtDown(cur);
			break;
		case 0x707000FF: //1
			cur = 0;
			blink(cur);
			break;
		case 0x7070807F: //2
			cur = 1;
			blink(cur);
			break;
		case 0x707040BF: //3
			cur = 2;
			blink(cur);
			break;
		case 0x7070C03F: //4
			cur = 3;
			blink(cur);
			break;
		case 0x7070D827: //->
			if (cur==diodsNum-1)
				cur=0;
			else
				cur++;
			blink(cur);
			break;			
		case 0x707058A7: //<-
			if (cur==0)
				cur=diodsNum-1;
			else
				cur--;
			blink(cur);
			break;		
		case 0x7070D02F:
			Serial.println("turnOf");
			turnAll(0);
			break;
		case 0x7070708F:
			Serial.println("turnOn");
			turnAll(maxBright);
			break;	
		case 0x707048B7: //all+
			for (byte i=0; i<diodsNum;i++)
			{
				brightUp(i);
			}
			break;
		case 0x7070C837: //all-	
			for (byte i=0; i<diodsNum;i++)
			{
				brigtDown(i);
			}
			break;		
		default:
			Serial.println(results.value,HEX);
			break;
	}
	
    irrecv.resume(); // принимаем следующую команду
  }

}

void brigtDown(byte i)
{
	if (cnstlns[i]<=stepB)
			{	
				cnstlns[i] = 0;
				blink(i);
			}
			else
			{
				cnstlns[i]-=stepB;
				analogWrite(pins[i], cnstlns[i]);
			}		
}
void brightUp(byte i)
{
	if (cnstlns[i]>=(maxBright-stepB))
			{
				cnstlns[i]=maxBright;
				blink(i);
			}
			else
			{
				cnstlns[i]+=stepB;
				analogWrite(pins[i], cnstlns[i]);
			}	
}

void turnAll(byte power)
{
	
	for (byte i=0; i<diodsNum;i++)
	{
		cnstlns[i] = power;
		analogWrite(pins[i], cnstlns[i]);
	}
	
}
void blink(byte i)
{
	if (cnstlns[i]>=100)
	{
		digitalWrite(pins[i],LOW);
	}
	else
	{
		analogWrite(pins[i], maxBright);	
	}
	
	delay(300);
	analogWrite(pins[i], cnstlns[i]);
}

