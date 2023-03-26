#define echoPin 12
#define trigPin 11
long duration = 0;
int input = 0;

int LMOTORPWR = 255;
int RMOTORPWR = 230;

int distanceMain[] = {25,50,75,100,125,150,175,200,225,250,275,300};
float AMain[] = {0,0.1,0.2,0.25,0.3,0.4,0.5,0.6,0.7,0.8,1,1};
float BMain[] = {0,0.1,0.2,0.4,0.6,1,1,1,0.6,0.4,0.7,1};
float CMain[] = {0,1,0.8,0.6,0.5,0.4,0.7,0.8,0.8,0.9,0.9,1};

int speedMain[] = {50,100,150,200,255};

float ZMain[] = {0,0,0.4,0.7,1};
float XMain[] = {0,0.5,0.5,0.8,1};
float YMain[] = {1,0.7,0.5,0.7,1};

void MotorRun (bool forward, bool Lmotor, bool Rmotor, int LMotorPwr=LMOTORPWR, int RMotorPwr=RMOTORPWR){
  if (forward ){
    if ( Lmotor ){
      digitalWrite(A0, HIGH);
      digitalWrite(A1, LOW);
      analogWrite(5, LMotorPwr);
    }
    if ( Rmotor ){
      digitalWrite(A3, HIGH);
      digitalWrite(A2, LOW);
      analogWrite(6, RMotorPwr);
    }
  }else{
    if ( Lmotor ){
        digitalWrite(A0, LOW);
        digitalWrite(A1, HIGH);
        analogWrite(5, LMotorPwr);
      }
      if ( Rmotor ){
        digitalWrite(A3, LOW);
        digitalWrite(A2, HIGH);
        analogWrite(6, RMotorPwr);
      }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  pinMode(3, INPUT_PULLUP);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
}

void loop() {
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  input = (duration * 0.034 / 2);
  
  Serial.print(input);
  Serial.println(" cm");

  float A[12];
  for (int i=0; i<11;i++){
    A[i] = AMain[i];
   }
  float B[12];
  for (int i=0; i<11;i++){
    B[i] = BMain[i];
   }
  float C[12];
  for (int i=0; i<11;i++){
    C[i] = CMain[i];
   }
  int speed[5];
  for (int i=0; i<4;i++){
    speed[i] = speedMain[i];
   }
  float Z[5];
   for (int i=0; i<5;i++){
    Z[i] = ZMain[i];
   }
  float X[5];
   for (int i=0; i<5;i++){
    X[i] = XMain[i];
   }
  float Y[5];
   for (int i=0; i<5;i++){
    Y[i] = YMain[i];
   }

  int closestVal = getClosest(A, input);
  int index = 0;
  switch (closestVal) {
  case 25:
    index = 0;
    break;
  case 50:
    index = 1;
    break;
  case 75:
    index = 2;
    break;
  case 100:
    index = 3;
    break;
  case 125:
    index = 4;
    break;
  case 150:
    index = 5;
    break;
  case 175:
    index = 6;
    break;
  case 200:
    index = 7;
    break;
  case 225:
    index = 8;
    break;
  case 250:
    index = 9;
    break;
  case 275:
    index = 10;
    break;
  case 300:
    index = 11;
    break;
  }
    
  float setA = A[index];
  for (int x = 0; x < 5; x++) {
    if (Z[x] > setA) {
      Z[x] = setA;
    }
  }

  float setB = B[index];
  for (int x = 0; x < 5; x++) {
    if (X[x] > setB) {
      X[x] = setB;
    }
  }

  float setC = C[index];
  for (int x = 0; x < 5; x++) {
    if (Y[x] > setC) {
      Y[x] = setC;
    }
  }

  float result[] = {0,0,0,0,0};
  float tempMax=0;
  for (int x = 0; x < 5; x++) {
    tempMax = max(max(Z[x],X[x]), Y[x]);
    result[x]=tempMax;
  }
  
  float nominator = 0;
  for(int x= 0;x<5; x++){
    nominator += result[x]*speed[x];
  }
  
  float denominator = 0;
  for(int x= 0;x<5; x++){
    denominator += result[x];
  }
  
  float speedResult = nominator/denominator;
  
  MotorRun(true, true, true,speedResult,speedResult);
  delay(500);
  
}

int getClosest(float arr[], int target){
  if(target > 300){
    return (float)300;
    }
  for (int x = 25; x<=300; x+=25) {
      if (target - x < 13) {
        return (float)x;
      }
  }
}
