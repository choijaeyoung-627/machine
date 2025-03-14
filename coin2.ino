int btn = A5;  // 기존 버튼 (A5) - "START" 출력
const int coinSensor = 3;  // 센서 (기존 btn1(A3) 대신 사용)
int pushBtn = A7; // 카운트다운 중지 버튼 - "PUSH" 출력
int trig1 = 24, trig2 = 28 ; // 초음파센서 트리거
int echo1 = 22, echo2 = 26 ; // 에코
int elec = 7;

int num1 = 0, num2 = 3;  // 카운트다운 숫자
int counter = 0;         // 카운터 변수
int FND2[7] = {41, 42, 43, 44, 45, 46, 47};  // 10의 자리
int FND1[7] = {34, 35, 36, 37, 38, 39, 40};  // 1의 자리
int FND_data[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

int isCounting = 0;
int isDisplayingSerial = 0;

bool btn1Pressed = false;
bool pushBtnPressed = false;
bool isCounterDisplayed = false;

void setup() {
    Serial.begin(9600);
    pinMode(elec, OUTPUT);
    pinMode(btn, INPUT_PULLUP);
    pinMode(coinSensor, INPUT_PULLUP);
    pinMode(pushBtn, INPUT_PULLUP);
    pinMode(trig1, OUTPUT);
    pinMode(echo1, INPUT);
    pinMode(trig2, OUTPUT);
    pinMode(echo2, INPUT);
  
    for (int i = 0; i < 7; i++) {
        pinMode(FND1[i], OUTPUT);
        pinMode(FND2[i], OUTPUT);
        digitalWrite(FND1[i], HIGH);
        digitalWrite(FND2[i], HIGH);
    }
    digitalWrite(elec,LOW);
    displayNumber(counter / 10, counter % 10);
}

void loop() {
      
    if(Serial.read() == '7'){
      digitalWrite(elec,LOW);
    }
    if (isDisplayingSerial || isCounterDisplayed) {
        return;
    }

    // 센서 감지
    if (digitalRead(coinSensor) == LOW && !pushBtnPressed) {
        if (!btn1Pressed) {
            btn1Pressed = true;
            delay(200);  // 중복 입력 방지
            counter++;
            if (counter > 7) counter = 7;
            displayNumber(counter / 10, counter % 10);

            if (counter >= 1) {
                isDisplayingSerial = 1;
                Serial.print("ONE MORE? \n Counter: ");
                Serial.println(counter);
                delay(2000);
                isDisplayingSerial = 0;
            }
        }
    } else {
        btn1Pressed = false;
    }

    // pushBtn(A7) 버튼 눌림 감지
    if (!digitalRead(pushBtn)) {
        if (!pushBtnPressed) {
            Serial.println("PUSH");
            pushBtnPressed = true;
            delay(200);  // 중복 입력 방지
            stopCountdown();
        }
    } else {
        pushBtnPressed = false;
    }

    // btn(A5) 버튼 눌림 감지
    if (counter > 0 && !digitalRead(btn) && !pushBtnPressed) {
        if (!digitalRead(btn)) {
            Serial.println("START");
            digitalWrite(elec, HIGH);
            delay(200);  // 중복 입력 방지
            resetCountdown();
            isCounting = 1;
        }
    } 


    // 카운트다운 실행
    if (isCounting) {
        displayNumber(num2, num1);
        delay(1000);

        if (num1 == 0) {
            if (num2 > 0) {
                num1 = 9;
                num2--;
            } else {
                displayNumber(0, 0);
                delay(3000);
                counter--;
                if (counter < 0) counter = 0;
                displayNumber(counter / 10, counter % 10);
                isCounting = 0;
            }
        } else {
            num1--;
        }
    }
}

// 카운트다운 정지
void stopCountdown() {
    isCounting = 0;
    displayNumber(num2, num1);
    delay(3000);
    counter--;
    if (counter < 0) counter = 0;
    displayNumber(counter / 10, counter % 10);
}

// 카운트다운 초기화
void resetCountdown() {
    num1 = 0;
    num2 = 3;
}

// 7세그먼트 숫자 표시
void displayNumber(int tens, int ones) {
    for (int i = 0; i < 7; i++) {
        digitalWrite(FND2[i], bitRead(FND_data[tens], i));
        digitalWrite(FND1[i], bitRead(FND_data[ones], i));
    }
}

// 7세그먼트 끄기
void displayOff() {
    for (int i = 0; i < 7; i++) {
        digitalWrite(FND2[i], HIGH);
        digitalWrite(FND1[i], HIGH);
    }
}
