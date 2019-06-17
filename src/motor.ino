void moveMotor(int L, int R) {
				if (L >= 0 ) {
								analogWrite(LMF, L * 1.05);
								analogWrite(LMR, 0);
				} else {
								analogWrite(LMF, 0);
								analogWrite(LMR, -L);
				}
				if (R >= 0) {
								analogWrite(RMF, R);
								analogWrite(RMR, 0);
				} else {
								analogWrite(RMF, 0);
								analogWrite(RMR, -R);
				}
}

void moveStraight(int pos) {
				offsetStraight(80);
				P = 0; I = 0; D = 0;
				lastError = 0;
				encoderposL = 0;
				encoderposR = 0;
				desiredposL = map(pos, 0, 300, 0, 350);
				desiredposR = map(pos, 0, 300, 0, 350);
				int encoderpos = (encoderposL + encoderposR) / 2;
				int desiredpos = (desiredposL + desiredposR) / 2;
				int encoderbackupL = 0;
				int encoderbackupR = 0;
				yaw = 0;
				delay(50);
				if ((desiredpos - encoderpos) > 0) {
								while ((desiredpos - encoderpos) > 10) {
												encoderpos = (encoderposL + encoderposR) / 2;
												int pwm = constrain((desiredpos - encoderpos), 0, MAXSPEED);
												int err = 0;
												int offset = 0;
												int wallL = getDistance(LEFT);
												int wallR = getDistance(RIGHT);
												if (wallL < WALLDISTANCE && wallR < WALLDISTANCE) {
																//err = (wallR - wallL);
																err = (wallDistance - wallL);
																P = 0;
												} else if (wallL > WALLDISTANCE && wallR < WALLDISTANCE) {
																err = (wallR - wallDistance);
																P = 0;
												} else if (wallR > WALLDISTANCE && wallL < WALLDISTANCE) {
																err = (wallDistance - wallL);
																P = 0;
												} else if (wallR > WALLDISTANCE && wallL > WALLDISTANCE) {
																err  = yaw;//readGyroZ();
																P = 0; I = 0; D = 0;
																lastError = 0;
												}
												P = 0.35 * err; //1.818
												I += 0 * err; //0.4
												D = 0 * (err - lastError);
												lastError = err;
												offset = P + I + D;
												moveMotor(pwm + offset, pwm - offset);
												if (leftBumpFlag && getDistance(FRONT) > 80) {
																bumpcheck=false;
																encoderbackupL =encoderposL;
																encoderbackupR =encoderposR;
																moveMotor(0,0);
																delay(50);
																moveMotor(-40,-40);
																delay(500);
																moveMotor(40,-20);
																delay(300);
																moveMotor(0,0);
																delay(50);
																moveMotor(40,40);
																delay(500);
																moveMotor(-20,40);
																delay(300);
																moveMotor(40,40);
																delay(100);
																encoderposL = encoderbackupL;
																encoderposR = encoderbackupR;
																leftBumpFlag = false;
																rightBumpFlag = false;
																bumpcheck=true;
												}else if (rightBumpFlag && getDistance(FRONT) > 80) {
																bumpcheck=false;
																encoderbackupL =encoderposL;
																encoderbackupR =encoderposR;
																moveMotor(0,0);
																delay(50);
																moveMotor(-40,-40);
																delay(500);
																moveMotor(-20,40);
																delay(300);
																moveMotor(0,0);
																delay(50);
																moveMotor(40,40);
																delay(500);
																moveMotor(40,-20);
																delay(300);
																moveMotor(40,40);
																delay(100);
																encoderposL = encoderbackupL;
																encoderposR = encoderbackupR;
																leftBumpFlag = false;
																rightBumpFlag = false;
																bumpcheck=true;
												}
												delayMicroseconds(500);
												yield();
								}
				}
				else {
								while ((encoderpos - desiredpos) > 0) {
												encoderpos = (encoderposL + encoderposR) / 2;
												int pwm = constrain((desiredpos - encoderpos), 0, MAXSPEED);
												int err = yaw;
												P = 0.3 * err; //1.818
												I += 0 * err; //0.4
												D = 0 * (err - lastError);
												lastError = err;
												int offset = P + I + D;
												moveMotor(-(pwm - offset), -(pwm + offset));
												delayMicroseconds(500);
												yield();
								}
				}
				beep();
				P = 0; I = 0; D = 0;
				lastError = 0;
				delay(50);
				moveMotor(0, 0); //stop motors
				lastError = 0;
}

void turn90(int angle, int dir) {
				bumpcheck=false;
				bool flag = false;
				int ANGLE = angle;
				//int ANGLE = angle - 6;
				if (getDistance(0) < WALLDISTANCE && dir == 1 || getDistance(2) < WALLDISTANCE && dir == -1) {
								flag = true;
				}
				unsigned long lastTime=0;
				if (getDistance(1) < WALLDISTANCE) {
								offsetStraight(35);
				}else{
								offsetStraight(80);
				}
				delay(10);
				moveMotor(0, 0);
				delay(10);
				int err = 0;
				Pr = 0, Ir = 0, Dr = 0;
				pitch = 0; roll = 0; yaw = 0;
				delay(10);
				do {
								unsigned long now = millis();
								double timeChange = (double)(now - lastTime);
								err = (ANGLE - abs(yaw));
								if (err < 5) err *= 2;
								if (abs(pitch) > 2.7) {
												beep();
												delay(80);
								}
								Pr = err;
								Ir += err * timeChange;
								Dr = (err - lastErrorTurn) / timeChange;
								lastErrorTurn = err;
								lastTime = now;
								int diff = 1.1 * Pr;
								moveMotor(dir * constrain(diff, 30, 100), -dir * constrain(diff, 30, 100));
								delay(1);
								yield();
				} while (abs(err) > 20);
        //delay(10);
				beep();
				moveMotor(0, 0);
				delay(50);
				if (flag) {
								moveMotor(-50, -50);
								delay(800);
								moveMotor(0, 0);
								yaw = 0;
				}
				leftBumpFlag = false;
				rightBumpFlag = false;
				bumpcheck = true;
}


void offsetStraight(int value) {
				yaw = 0;
				delay(100);
				int dist = (getDistance(1) % 300);
				if (dist > value) {
								while (dist > value) {
												dist = (getDistance(1) % 300);
												int pwm = constrain((dist - value),40,100);
												int err  = 1.5 * yaw;//readGyroZ() / 14;
												moveMotor(pwm + err, pwm - err);
												delay(2);
//yield();
								}
				} else {
								while (dist < value) {
												dist = (getDistance(1) % 300);
												int pwm = constrain((value - dist),40,100);
												int err = -1.5 * yaw;//readGyroZ() / 14;
												moveMotor(-(pwm + err), -(pwm - err));
												delay(2);
//yield();
								}
				}
}


//int rpmOffset() {
//  int L = 60000 / (((currentpulsetime1) / 1000.0) * 562.215);
//  L = constrain(L, 0, 255);
//  float KL = kalman.updateEstimate(L);
//  int R = 60000 / (((currentpulsetime2) / 1000.0) * 562.215);
//  R = constrain(R, 0, 255);
//  float KR = kalman.updateEstimate(R);
//  int offset = (KL - KR) * 2;
//  return offset;
//}
