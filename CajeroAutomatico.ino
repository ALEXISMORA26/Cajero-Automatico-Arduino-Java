#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Crear el objeto lcd  dirección  0x27 y 20 columnas x 4 filas

//sacar dinero
int cant;
int cant10;
int res10;
int cant5;
int res5;
int cant2;
int res2;
int cant1;
int res1;

//cantidad de saldo que tiene la tarjeta
int saldo;


// cantidad de cada moneda adentro del cajero
int unPeso = 10;
int dosPesos = 10;
int cincoPesos = 10;
int diezPesos = 10;

int beforeReadUnPeso = 1;
int beforeReadDosPesos = 1;
int beforeReadCincoPesos = 1;
int beforeReadDiezPesos = 1;




void setup() {

  myservo1.attach(6); // moneda de 1 peso
  myservo2.attach(7); // moneda de 2 pesos
  myservo3.attach(8); // moneda de 5 pesos
  myservo4.attach(9); // moneda de 10 pesos
  lcd.init();       // Inicializar el LCD
  lcd.backlight();  //Encender la luz de fondo del lcd

  pinMode(2, INPUT);  // 1 Peso
  pinMode(3, INPUT);  // 2 Pesos
  pinMode(4, INPUT);  // 5 Pesos
  pinMode(5, INPUT);  // 10 Pesos

  Serial.begin(9600);
}


void loop() {
  //aqui se ponel los grados de los servomotores para que no se cainagn las monedas
  myservo1.write(160);
  myservo2.write(180);
  myservo3.write(155);
  myservo4.write(145);

  //1peso
  if (digitalRead(2) == 0 && beforeReadUnPeso == 1) {
    //Serial.println("1 Peso");
    unPeso += 1;
    beforeReadUnPeso = 0;
  } else if (digitalRead(2) == 1) {
    beforeReadUnPeso = 1;
  }
  //2pesos
  if (digitalRead(3) == 0 && beforeReadDosPesos == 1) {
    //Serial.println("2 Pesos");
    dosPesos += 1;
    beforeReadDosPesos = 0;
  } else if (digitalRead(3) == 1) {
    beforeReadDosPesos = 1;
  }
  //5pesos
  if (digitalRead(4) == 0 && beforeReadCincoPesos == 1) {
    //Serial.println("5 Pesos");
    cincoPesos += 1;
    beforeReadCincoPesos = 0;
  } else if (digitalRead(4) == 1) {
    beforeReadCincoPesos = 1;
  }
  //10pesos
  if (digitalRead(5) == 0 && beforeReadDiezPesos == 1) {
    //Serial.println("10 Pesos");
    diezPesos += 1;
    beforeReadDiezPesos = 0;
  } else if (digitalRead(5) == 1) {
    beforeReadDiezPesos = 1;
  }
  //aqui se imprime en la pantalla lcd para tener control de las monedas
  lcd.setCursor(3, 0);
  lcd.print(unPeso);
  lcd.setCursor(6, 0);
  lcd.print(dosPesos);
  lcd.setCursor(10, 0);
  lcd.print(cincoPesos);
  lcd.setCursor(13, 0);
  lcd.print(diezPesos);
  lcd.setCursor(0, 1);
  int sum = unPeso * 1 + dosPesos * 2 + cincoPesos * 5 + diezPesos * 10;
  lcd.print("TOTAL MXN: ");
  lcd.print(sum);
  saldo = sum-100;
  Serial.println(saldo);
  if (Serial.available() > 0) {
    cant = Serial.parseInt();
    if (cant == saldo | cant <= saldo) {
      darDinero(cant);
    }
  }
}
void darDinero(int cant) {
  if (cant > 0) {
    //aqui se hacen las operaciones para dar cada moneda
    saldo -= cant;
    cant10 = cant / 10;
    res10 = cant % 10;
    cant5 = res10 / 5;
    res5 = res10 % 5;
    cant2 = res5 / 2;
    res2 = res5 % 2;
    cant1 = res2 / 1;
    res1 = res2 % 1;
    /*
    Serial.print("cantitad de 10: ");
    Serial.print(cant10);
    Serial.print(", residuo: ");
    Serial.println(res10);
    Serial.print("cantitad de 5: ");
    Serial.print(cant5);
    Serial.print(", residuo: ");
    Serial.println(res5);
    Serial.print("cantitad de 2: ");
    Serial.print(cant2);
    Serial.print(", residuo: ");
    Serial.println(res2);
    Serial.print("cantitad de 1: ");
    Serial.print(cant1);
    Serial.print(", residuo: ");
    Serial.println(res1);
*/
    while (cant10 > 0) {
      diezPesos--;
      delay(500);
      myservo4.write(0);
      delay(500);
      myservo4.write(145);
      //Serial.println(cant10);
      cant10--;

    }
    while (cant5 > 0) {
      cincoPesos--;
      delay(500);
      myservo3.write(0);
      delay(500);
      myservo3.write(155);
      //Serial.println(cant10);
      cant5--;

    }
    while (cant2 > 0) {
      dosPesos--;
      delay(500);
      myservo2.write(0);
      delay(500);
      myservo2.write(180);
      //Serial.println(cant10);
      cant2--;

    }
    while (cant1 > 0) {
      unPeso--;
      delay(500);
      myservo1.write(0);
      delay(500);
      myservo1.write(160);
      //Serial.println(cant10);
      cant1--;
    }
  }
}
