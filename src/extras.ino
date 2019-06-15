void setupIO() {
				pinMode(RMF, OUTPUT);
				pinMode(RMR, OUTPUT);
				pinMode(LMF, OUTPUT);
				pinMode(LMR, OUTPUT);
				pinMode(encoder0PinA, INPUT_PULLUP);
				pinMode(encoder0PinB, INPUT_PULLUP);
				pinMode(encoder1PinA, INPUT_PULLUP);
				pinMode(encoder1PinB, INPUT_PULLUP);
				pinMode(buzzPin, OUTPUT);
				pinMode(LEFTBUMP,INPUT);
				pinMode(RIGHTBUMP,INPUT);
}

void beginSerialUSB() {
				int count = 0;
				SerialUSB.begin(9600);
				while (!SerialUSB && count < 4) {
								beep();
								delay(32);
								count++;
				}
}

void beep() {
				digitalWrite(buzzPin, HIGH);
				delay(50);
				digitalWrite(buzzPin, LOW);
}

void waitForSignal(){
				while (getDistance(1) > 80) {
								for (int i = 0; i < 8; i++) {
												pixels.setPixelColor(i, pixels.Color(20, 20, 20));
												pixels.show();
								}
								delay(20);
				}
				//Move one tile sequence
				clearPixels();
				beep();
				while (getDistance(1) < 100) {
								delay(20);
				}
				delay(500);
}

void bumpLoop(){
				if (bumpcheck) {
								if (!digitalRead(LEFTBUMP)) {
												leftBumpFlag=true;
												delay(100);
								}
								if (!digitalRead(RIGHTBUMP)) {
												rightBumpFlag=true;
												delay(100);
								}
				}else{
								delay(100);
				}
				yield();
}
