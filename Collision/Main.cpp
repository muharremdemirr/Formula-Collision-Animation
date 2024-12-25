/*
Proje 3 öğrenci tarafından ortak yapıldı.
200101010 - Muharrem DEMİR
210101157 - Berkay DURSUN
200101007 - Murat EKER
*/


#include "icb_gui.h"  

int FRM1;
ICBYTES panel;
bool thread_continue = false;
DWORD dw;

// Yağmur damlaları için koordinatlar ve hızlar
int bx = 50;
int by = 50;
int x[12] = { bx, bx + 50, bx + 100, bx + 150, bx + 200, bx + 250, bx + 300, bx + 350, bx + 400, bx + 450, bx + 500, bx + 550 };
int y[12] = { by, by + 50, by + 100, by, by + 50, by + 100, by, by + 50, by + 100, by, by + 50, by + 100 };
int speed[12];

// Araçlar için koordinat ve hız değişkenleri
int left_x = 50;      // Soldan sağa hareket eden araç 
int right_x = 500;    // Sağdan sola hareket eden araç
int left_speed = 10;  // Kırmızı  aracın hızı  (daha hızlı)
int right_speed = 5;  // Mavi aracın hızı  (daha yavaş)

void static setRandomSpeeds() {
    for (int i = 0; i < 11; i++) {
        speed[i] = rand() % 20 + 15;
    }
}

void static DrawRaindrop(int x, int y) {
    HalfRect(panel, x, y, -4, -15, 0x00BFFF);
    HalfRect(panel, x, y, 4, -15, 0x00BFFF);
    FillCircle(panel, x, y, 3, 0x00BFFF);
}

void static DrawFormulaCar(int posX, int posY, int color, int kabin) {
    // Gövde
    FillRect(panel, posX + 5, posY - 5, 40, 10, color);  // Gövde rengi (kırmızı veya mavi)

    // Kabin (cam)
    FillRect(panel, kabin, posY - 8, 10, 5, 0xFFFFFF);  // Kabin kısmı (açık mavi)

    // Tekerlekler
    FillCircle(panel, posX + 10, posY + 5, 6, 0x333333);  // Ön tekerlek
    FillCircle(panel, posX + 35, posY + 5, 6, 0x333333);  // Arka tekerlek
}

void static DrawHorizontalObjects() {
    // Soldan sağa giden arac çizimi
    DrawFormulaCar(left_x, 300, 0xFF0000, left_x + 13);  // Kırmızı araç 

    // Sağdan sola giden arac çizimi
    DrawFormulaCar(right_x, 300, 0x0000FF, right_x + 25);  // Mavi araç 
}

// Ağaç  çizme fonksiyonu
void static DrawTree(int treeX, int treeY) {
    // Gövde (kahverengi dikdörtgen)
    FillRect(panel, treeX, treeY, 10, 30, 0x8B4513);  // Gövde rengi (kahverengi)

    // Yapraklar (yeşil daireler)
    FillCircle(panel, treeX + 5, treeY - 10, 15, 0x228B22);  // Üst yaprak (yeşil)
    FillCircle(panel, treeX + 5, treeY - 25, 12, 0x228B22);  // Orta yaprak (yeşil)
    FillCircle(panel, treeX + 5, treeY - 35, 8, 0x228B22);   // Alt yaprak (yeşil)
}


void static Trees() {
    // Ağaç  koordinatları 
    int treeCoords[25][2] = {
        {200, 500},   // 1. ağaç
        {50, 450},    // 2. ağaç 
        {100, 520},   // 3. ağaç 
        {150, 470},   // 4. ağaç
        {200, 510},   // 5. ağaç
        {250, 480},   // 6. ağaç
        {300, 530},   // 7. ağaç
        {350, 460},   // 8. ağaç
        {400, 500},   // 9. ağaç
        {450, 490},   // 10. ağaç 
        {500, 470},   // 11. ağaç 
        {550, 520},   // 12. ağaç 
        {550, 320},   // 13. ağaç 
        {200, 400},   // 14. ağaç 
        {50, 350},    // 15. ağaç 
        {100, 420},   // 16. ağaç 
        {150, 370},   // 17. ağaç 
        {200, 410},   // 18. ağaç 
        {250, 380},   // 19. ağaç 
        {300, 430},   // 20. ağaç 
        {350, 360},   // 21. ağaç 
        {400, 400},   // 22. ağaç 
        {450, 390},   // 23. ağaç 
        {500, 370},   // 24. ağaç 
        {550, 420},   // 25. ağaç   
    };

    // Ağaçları  sabit konumlarda çizme
    for (int i = 0; i < 25; i++) {
        DrawTree(treeCoords[i][0], treeCoords[i][1]);
    }
}

// Bulut çizme fonksiyonu
void static DrawCloud(int x, int y) {
    FillCircle(panel, x, y, 20, 0xD3D3D3); // Büyük gri daire
    FillCircle(panel, x - 15, y, 20, 0xD3D3D3); // Sol küçük daire
    FillCircle(panel, x + 15, y, 20, 0xD3D3D3); // Sağ küçük daire
    FillCircle(panel, x - 7, y - 10, 20, 0xD3D3D3); // Üst sol
    FillCircle(panel, x + 7, y - 10, 20, 0xD3D3D3); //  Üst sağ 
}

void static DrawClouds() {
    // Bulutların pozisyonları, yukarıdan başlaması için y koordinatları artırıldı 
    DrawCloud(100, 50);
    DrawCloud(200, 40);
    DrawCloud(360, 60);
    DrawCloud(450, 30);
    DrawCloud(30, 20);
    DrawCloud(550, 70);
    DrawCloud(250, 80);
}

VOID* Crashed(PVOID lpParam) {
    int smokeY1 = 290; // İlk duman pozisyonları 
    int smokeY2 = 285;
    int smokeY3 = 280;
    thread_continue = true;

    //bool a = true;
    while (thread_continue) {
        FillRect(panel, 0, 0, 600, 600, 0x55FFFF);  // Ekranı beyaza boyar
        FillRect(panel, 0, 310, 600, 295, 0x55FF55); // Çimen
        FillRect(panel, 0, 305, 600, 15, 0x808080); //  Yol

        Trees();

        // Yağmur damlalarının çizimi
        for (int i = 0; i < 11; i++) {
            DrawRaindrop(x[i], y[i]);
            y[i] += speed[i];
            if (y[i] > 600) y[i] = 0;
        }
        DrawClouds();
        // Kırmızı  aracın  dağılmış  hali
        FillRect(panel, left_x, 300, 30, 8, 0xFF0000);     // Gövde parçası 
        FillRect(panel, left_x + 10, 290, 10, 5, 0x00FFFF); // Cam parçası 
        FillCircle(panel, left_x + 5, 310, 6, 0x333333);    // Sol tekerlek
        FillCircle(panel, left_x + 20, 315, 6, 0x333333);   // Sağ tekerlek

        // Mavi aracın  dağılmış  hali
        FillRect(panel, right_x, 300, 30, 8, 0x0000FF);    // Gövde parçası 
        FillRect(panel, right_x + 10, 290, 10, 5, 0x00FFFF); // Cam parçası 
        FillCircle(panel, right_x + 5, 310, 6, 0x333333);   // Sol tekerlek
        FillCircle(panel, right_x + 20, 315, 6, 0x333333);  // Sağ tekerlek

        // Duman efektleri (gri daireler), yukarı doğru hareket eder
        FillCircle(panel, (left_x + right_x) / 2 - 10, smokeY1, 8, 0x808080);
        FillCircle(panel, (left_x + right_x) / 2 + 10, smokeY2, 10, 0xA9A9A9);
        FillCircle(panel, (left_x + right_x) / 2, smokeY3, 12, 0xB0B0B0);

        // Dumanın yukarı doğru hareketini sağlar
        smokeY1 -= 1; // Duman yavaşça yukarı çıkar
        smokeY2 -= 1;
        smokeY3 -= 1;

        // Eğer duman ekran dışına  çıkarsa yeniden pozisyonla
        if (smokeY1 < 0) smokeY1 = 290;
        if (smokeY2 < 0) smokeY2 = 285;
        if (smokeY3 < 0) smokeY3 = 280;

        // Yangın efektleri (sarı ve kırmızı alevler)
        for (int i = 0; i < 10; i++) {
            int fireX = (left_x + right_x) / 2 + (rand() % 20 - 10); // Rastgele x pozisyonu
            int fireY = 300 - (rand() % 20);                         // Rastgele y pozisyonu

            // Sarı ve kırmızı  dairelerle alevleri oluştur
            FillCircle(panel, fireX, fireY, rand() % 5 + 3, 0xFFA500);  // Sarı alev
            FillCircle(panel, fireX + 5, fireY - 5, rand() % 4 + 2, 0xFF4500);  // Kırmızı alev
        }


        // Görüntüyü ekrana yansıt
        DisplayImage(FRM1, panel);
        Sleep(30);
    }
    return NULL;
}

VOID* Animate(PVOID lpParam) {


    while (thread_continue) {
        FillRect(panel, 0, 0, 600, 600, 0x55FFFF);
        FillRect(panel, 0, 310, 600, 295, 0x55FF55);
        FillRect(panel, 0, 305, 600, 15, 0x808080);

        // Yağmur damlalarını çiz
        for (int i = 0; i < 11; i++) {
            DrawRaindrop(x[i], y[i]);
            y[i] += speed[i];
            if (y[i] > 600) y[i] = 0;
        }
        DrawClouds();


        DrawHorizontalObjects();


        //Ağaçlar
        Trees();
        // Araçların hareketi
        left_x += left_speed;
        right_x -= right_speed;

        // Çarpışma kontrolü
        if (left_x + 40 >= right_x) {  // Araç genişliğine göre çarpışma kontrolü 
            left_speed = -left_speed;  // Yön değiştirme
            right_speed = -right_speed;
            thread_continue = false;
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Crashed, NULL, 0, &dw); // Crash Sicimi
            return NULL; 
        }

        // Ekran sınırlarını  kontrol etme
        if (left_x <= 0 || left_x >= 540) left_speed = -left_speed;  // Kırmızı aracın sınırları 
        if (right_x <= 0 || right_x >= 540) right_speed = -right_speed;  // Mavi aracın sınırları  

        // Görüntüyü ekrana yansıt
        DisplayImage(FRM1, panel);
        Sleep(30);
    }

}
void butonfonk() {
    panel = 0;
    if (!thread_continue) {
        thread_continue = true;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Animate, NULL, 0, &dw); // Bar hareketi

        SetFocus(ICG_GetMainWindow());
    }
    else {
        thread_continue = false;
    }

}

void ICGUI_Create() {
    ICG_MWTitle("Formula Cars Collision");
    ICG_MWSize(800, 720);
    ICG_SetFont(30, 12, "");
}

void ICGUI_main() {
    setRandomSpeeds();

    CreateImage(panel, 600, 600, ICB_UINT);

    FRM1 = ICG_FrameMedium(0, 50, 600, 600);
    ICG_Button(650, 50, 125, 50, "START / STOP", butonfonk);
}