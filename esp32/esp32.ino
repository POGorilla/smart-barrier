// Biblioteci necesare pentru functionare
#include <WebServer.h>    // Permite crearea unui server web pe ESP32
#include <WiFi.h>         // Pentru conectare la Wi-Fi
#include <esp32cam.h>     // Pentru controlul camerei ESP32-CAM
#include <ESP32Servo.h>   // Pentru controlul servomotorului

// Numele si parola retelei Wi-Fi create de ESP32
const char* AP_SSID = "ESP32-CAM-AP";   // Numele retelei Wi-Fi
const char* AP_PASS = "12345678";   // Parola retelei Wi-Fi

WebServer server(80); // Creeaza un server web pe portul 80 (standard pentru pagini web)

// Setari pentru diferite rezolutii ale camerei
static auto loRes = esp32cam::Resolution::find(320, 240);   // Rezolutie mica 
static auto midRes = esp32cam::Resolution::find(350, 530);  // Rezolutie medie
static auto hiRes = esp32cam::Resolution::find(800, 600);   // Rezolutie mare

Servo barrierServo;   // Obiect C++ care controleaza servomotorul (bariera)
bool barrierOpen = false;   // Retine daca bariera este deschisa sau nu

// Functie care face poza si trimite imaginea prin web
void serveJpg()
{
  auto frame = esp32cam::capture();   // Face poze cu camera
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");   // Daca poza nu a fost realizata
    server.send(503, "", "");   // Trimite raspuns de eroare clientului
    return;
  }

  // Daca poza a fost realizata, afiseaza informatii in consola seriala pe programul Arduino
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  // Trimite imaginea inapoi la client
  server.setContentLength(frame->size());   // Tipul de continut este imagine JPEG
  server.send(200, "image/jpeg");   // Obtinere client care a facut cererea
  WiFiClient client = server.client();  // Trimite poza catre client
  frame->writeTo(client);
}

// Aceste functii schimba rezolutia camerei si trimit poza
void handleJpgLo() {
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}

void handleJpgMid() {
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}

void handleJpgHi() {
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

// Functie care controleaza bariera printr-un parametru web
void handleSetServo() {
  if (server.hasArg("open")) { // Verifica daca s-a trimis parametrul "Open"
    String value = server.arg("open");

    if (value == "1") { // Daca se cere deschiderea barierei
      for (int pos = 0; pos <= 90; pos++) { // Roteste servomotorul pana la 90 de grade
        barrierServo.write(pos);
        delay(15); // Pauza scurta pentru miscare lina
      }
      barrierOpen = true;
      Serial.println("Barrier OPENED");
    } else { // Daca se cere inchiderea barierei
      for (int pos = 90; pos >= 0; pos--) { // Roteste servomotorul inapoi la 0 grade
        barrierServo.write(pos);
        delay(15);
      }
      barrierOpen = false;
      Serial.println("Barrier CLOSED");
    }

    server.send(200, "text/plain", "OK"); // Trimite mesaj de confirmare
  } else {
    server.send(400, "text/plain", "Missing 'open' parameter"); // Daca nu s-a trimis comanda corecta
  }
}

// Functia care se executa o singura data la pornirea dispozitiviului
void setup() {
  Serial.begin(9600); // Porneste comunicarea seriala cu viteza 9600 Bps
  Serial.println();

  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);   // Tipul placii ESP32-CAM
    cfg.setResolution(hiRes);       // Rezolutia initiala a camerei
    cfg.setBufferCount(2);          // Numar de cadre in memorie
    cfg.setJpeg(80);                // Calitatea pozei JPEG (0 - 100)

    bool ok = Camera.begin(cfg);    // Porneste camera cu setarile de mai sus
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  barrierServo.attach(13);    // Leaga servomotorul la pinul D13
  barrierServo.write(0);      // Pozitia intiala a servomotorului (bariera inchisa)

  // Configurare modul Access Point (Placuta creeaza propria retea Wi-Fi)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS); // Creeaza reteaua cu numele si parola definite

  // Afiseaza in consola adresa IP a dispozitivului (pentru accesare)
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: http://");
  Serial.println(IP);
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-mid.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /set-servo?open=1 or 0");

  // Rutele web - adresele la care raspunde serverul
  server.on("/cam-lo.jpg", handleJpgLo);    // Imagine cu rezolutie mica
  server.on("/cam-mid.jpg", handleJpgMid);  // Imagine cu rezolutie medie
  server.on("/cam-hi.jpg", handleJpgHi);    // Imagine cu rezolutie mare
  server.on("/set-servo", handleSetServo);  // Comanda pentru deschiderea / inchiderea barierei

  server.begin(); // Porneste serverul web
}

// Functie care se repeta mereu, atat timp cat placa sta pornita
void loop() {
  server.handleClient(); // Serverul raspunde la cererile primite de la web
}

