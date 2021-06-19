#include <SoftwareSerial.h>//Bluetooth için.
SoftwareSerial bluetooth(3, 2); // TX, RX
#define eleman 4
char* gelenDizi;
char gelenData;//Bl'den gelen tek veri için.
boolean dataAlindi = false;
int yer;
//dijital pinlerden PWM çıkış olmayanlara bağlanacak.
#define led 8  //8
#define buzzer 13
#define okumaSayisi 50
#define hassasiyet 50

int antPin=A3;
int anHassasiyetPin=A4;
int anDeger=0;
int deger=0;
float gerilim=0.0;
int okunanDegerler[okumaSayisi];
int ortalamalar[100];
int ortalama=0,index=0,toplam=0,maks=0,okumaHassasiyeti=0;
int say=0;// Led yanma sayar, Bluetooth elek. var gondermek için
void setup() {
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  bluetooth.println("Bluetooth Acik...");

  gelenDizi = new char[eleman];//Bl'den gelenleri tutar.
  pinMode(antPin,INPUT);
  pinMode(anHassasiyetPin,INPUT);
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  
  digitalWrite(led,HIGH) ;
  digitalWrite(buzzer,HIGH) ;
  delay(1000);
  for (int i = 0; i < okumaSayisi; i++)
      okunanDegerler[i] = 0;                            
}

void loop() {

    okumaHassasiyeti=analogRead(anHassasiyetPin);
    okumaHassasiyeti= map(okumaHassasiyeti, 1, 1023, 1,1000);//hassasiyet 1-1000 
    for(int i=0;i<okumaSayisi;i++){
         okunanDegerler[i]=analogRead(antPin);
         toplam+=okunanDegerler[i];
    }
     //ortalama = analogRead(analogPin);
     ortalama=toplam/okumaSayisi;
     //gerilim=(5.0/1024.0)*ortalama;
    
     deger = constrain(ortalama, 1, okumaHassasiyeti);                                   
     deger = map(deger, 1, okumaHassasiyeti, 1, 255);    
     if(deger>200)say++;// led yanma durumu sayar
     if(say>25){  //Elektromanyetik alan tam algılandı ise
        say=0;
        bluetooth.println("Hatta Elektrik VARRR...!");
     }
     analogWrite(led, deger);  
     analogWrite(buzzer, deger);
       
    Serial.print("Okuma Hassasiyeti:");Serial.println(okumaHassasiyeti);
    Serial.print("Ortalama:");Serial.println(ortalama);
    Serial.print("PWM = ");Serial.println(deger); 
    //Serial.println(say);
    //********
    
     if (bluetooth.available()){    
        gelenData=bluetooth.read();
         switch(gelenData) {
           case 'b':  
                    bluetooth.println("Bluetooth Bagli...!");
                    dataAlindi=false;
                   break;
           default: if (dataAlindi == false) diziSil();
                    gelenDizi[yer] = gelenData;
                    yer++;
                    dataAlindi = true;  
         }
     }
     ///******
     toplam=0;
     delay(1);// 10mS de bir işlemleri tekrarla
}
 void diziSil(){
   for (int i=0; i<=yer; i++) gelenDizi[i] = 0; 
   yer= 0;
}




