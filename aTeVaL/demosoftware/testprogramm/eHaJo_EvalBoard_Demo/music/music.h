/*
 * music.h
 *
 * Created: 22.09.2013 19:09:18
 *  Author: calm
 */ 


#ifndef MUSIC_H_
#define MUSIC_H_
#include <avr/io.h>


//Einmal die Töne anlegen				Ton in Hz
#define TON_C		(F_CPU/2/264)		//264
#define TON_CIS		(F_CPU/2/275)		//275
#define TON_D		(F_CPU/2/297)		//297
#define TON_ES		(F_CPU/2/317)		//317
#define TON_E		(F_CPU/2/330)		//330
#define TON_F		(F_CPU/2/351)		//351
#define TON_FIS		(F_CPU/2/367)		//367
#define TON_G		(F_CPU/2/396)		//396
#define TON_GIS		(F_CPU/2/422)		//422
#define TON_A		(F_CPU/2/440)		//440
#define TON_B		(F_CPU/2/475)		//475
#define TON_H		(F_CPU/2/495)		//495
#define TON_CC		(F_CPU/2/528)		//528
#define TON_CCIS	(F_CPU/2/550)		//550
#define TON_DD		(F_CPU/2/594)		//594
#define TON_EES		(F_CPU/2/634)		//634
#define TON_EE		(F_CPU/2/660)		//660
#define TON_FF		(F_CPU/2/704)		//704
#define TON_FFIS	(F_CPU/2/734)		//734
#define TON_GG		(F_CPU/2/792)		//792
#define TON_GGIS	(F_CPU/2/844)		//844
#define TON_AA		(F_CPU/2/880)		//880
#define TON_BB		(F_CPU/2/950)		//950
#define TON_HH		(F_CPU/2/990)		//990

/*
// Diese Werte berechnen sich aus "1MHz / Frequenz des Tones / 2"
// Sollte ein anderer CPU-Takt verwendet werden muessen
// die Werte neu berechnet werden.
#define TON_C		1894l
#define TON_CIS		1818l
#define TON_D		1684l
#define TON_ES		1557l
#define TON_E		1515l
#define TON_F		1420l
#define TON_FIS		1362l
#define TON_G		1263l
#define TON_GIS		1185l
#define TON_A		1136l
#define TON_B		1053l
#define TON_H		1010l
#define TON_CC		947l
#define TON_CCIS	909l
#define TON_DD		842l
#define TON_EES		789l
#define TON_EE		758l
#define TON_FF		710l
#define TON_FFIS	682l
#define TON_GG		631l
#define TON_GGIS	592l
#define TON_AA		568l
#define TON_BB		526l
#define TON_HH		505l
*/

void play_tone(void);




#endif /* MUSIC_H_ */