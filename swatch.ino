/* Code Created by ShadyFox2004 */

#include "config.h"
#include <time.h>
#define rtc ttgo->rtc

TTGOClass *ttgo;
TFT_eSPI *tft;
AXP20X_Class *power;

char buf[128];
bool rtcIrq = false;
bool irq = false;
int mode = 0;

void setup() {
	/* Usual Watch Booting */
	ttgo = TTGOClass::getWatch();
	ttgo->begin();
	ttgo->openBL();
	
	/* Short-cuts */
	tft = ttgo->tft;
	power = ttgo->power;
	
	/* Interupts */	
    	pinMode(AXP202_INT, INPUT_PULLUP);
    	attachInterrupt(AXP202_INT, [] {
        	irq = true;
    	}, FALLING);
    	    	
	/* Module boot */
	power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ | AXP202_CHARGING_IRQ, true);
    	power->clearIRQ();
	power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
	
	/* Watch display setting */
	tft->setTextFont(2);
	tft->fillRect(0, 0, 210, 130, TFT_BLACK);
	tft->setCursor(0, 0);
	
	/* Finaly Show Somthing PLZ? */
	tft->print("Welcome to swatch. \n \nThe Simple Yet Efficient Opensource SmartWatch Software!\n");
	delay(1000);
}
void loop() {
	/*	Increment the mode if button is press	*/
	if (irq) {
		irq = 0;
   		power->readIRQ();
		if (power->isPEKShortPressIRQ()) {
			tft->println("Powerkey Press... Changing the mode");
			mode++;	
		}
		power->clearIRQ();
	}
	
	/*	Do what de current mode has to do */
	switch(mode) {
	case 0:
		tft->setCursor(0, 0);
		tft->fillScreen(TFT_BLACK);
		tft->println("RTC TIME :");		
 		tft->setTextColor(TFT_GREEN, TFT_BLACK);
  		snprintf(buf, sizeof(buf), "%s", rtc->formatDateTime());
    		tft->drawString(buf, 5, 118, 7);
		break;
	case 1:
		ttgo->closeBL();
		mode++;
		break;
	case 3:
		ttgo->openBL();
		mode++;
		break;
	case 5:
		mode=0;
		break;
	default:
		break;		
	}
	delay(1000);
}

