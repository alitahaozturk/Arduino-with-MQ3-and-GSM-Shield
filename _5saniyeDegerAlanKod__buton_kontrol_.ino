

#include <LiquidCrystal.h> // include komutu ile kütüphane projede kullanılmak üzere tanımlanır..
#include <GSM.h>
#define PINNUMBER "" // eğer gsm modülünde kullanılan sim kartta pin numarası varsa burada belirtilir.
 
GSM gsmAccess;
GSM_SMS sms;

LiquidCrystal lcd(12, 11, 10, 5, 4, 9, 8);

int backLight = 13;
int degerler[6];
bool degerokunmadi = true;
int butonpin = 6;
int rolepin = 7;
int alkolpin = A0;
int butondurum = 0;

void setup() {
  lcd.begin(16,2);
  lcd.clear();
  Serial.begin(9600);
  pinMode(butonpin, INPUT);
  pinMode(rolepin,OUTPUT);
  digitalWrite(rolepin,HIGH);
  
  while(!Serial){
    ; // Seri portun açılması bekleniyor.
  }
  lcd.setCursor(0,0);
  lcd.print("SMS Gonderme Uyg");
  
  boolean notConnected = true;
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      lcd.setCursor(0,1);
      lcd.print("GSM Baglanamadi");
      delay(1000);
    }
  }
  lcd.setCursor(0,1);
  lcd.print("GSM HAZIR.");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Butona basiniz..");
  lcd.setCursor(0,1);
  lcd.print("5sn Ufleyiniz..");
}

void loop() {
butondurum = digitalRead(butonpin); // butonun durumunu okuyoruz..

if(butondurum == 1){// butona basıldıysa değer okunacak
  while(degerokunmadi){// Değer sadece bir kere okunacak
    for(int i=0;i<6;i++)// 6 kere değer okuyacağız (6saniye)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Uflemeye devam edin.");
      degerler[i] = analogRead(alkolpin);
      lcd.setCursor(0,1);
      lcd.print(degerler[i]);
      delay(1000);
    }
    degerokunmadi = false; // Değer okunduğu için bir daha okunmaması gerekiyor.
  }
  if(degerokunmadi == false)
    if(degerler[5] < 250){
      lcd.clear();
      // Araç çalışabilir.
      lcd.setCursor(0,0);
      lcd.print("Deger Uygun");
      lcd.setCursor(0,1);
      lcd.print("Araci Calistir.");
      //Roleye 5volt çıkış verilir.
      digitalWrite(rolepin,HIGH);
      delay(1000);
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Deger Yuksek");
      lcd.setCursor(0,1);
      lcd.print("Mesaj atiliyor..");
      SMSGONDER();
    }
}
delay(1000);
}

void SMSGONDER(){
  // mesaj atılacak numara
  char remoteNum[20] = "+905389655522";

  // mesaj içeriği
  char txtMsg[200] = "Alkollu araca bindim. Lutfen beni al.";

  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Mesaj gonderildi.");
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Aracta Bekleyin....");
}
 
