
// Gizem EROL - 210101112
// Selvinaz Zeynep Kıyıkcı - 210101101
// Zeynep Sude Kırmacı - 200101011

#include <windows.h>
#include "icb_gui.h"

#define NUM_PHILOSOPHERS 5

int F1, MLE;
ICBYTES Table, Philosopher[NUM_PHILOSOPHERS], Fork[NUM_PHILOSOPHERS], PhilosopherThinking, PhilosopherEating, Plate;
int PhilosopherStates[NUM_PHILOSOPHERS] = { 0 }; // 0: Düşünme, 1: Çatal Bekleme, 2: Yemek Yeme
bool ForkAvailable[NUM_PHILOSOPHERS] = { true, true, true, true, true }; // Çatal durumu
HANDLE ForkSemaphores[NUM_PHILOSOPHERS]; // Semaphore
HANDLE DiningSemaphore; // Aynı anda en fazla 2 filozof yemek yiyebilir
int PhilosopherPositions[NUM_PHILOSOPHERS][2] = {
    {175, 7}, {350, 100}, {250, 290}, {80, 290}, {2, 120}
};
int ForkPositions[NUM_PHILOSOPHERS][2] = {
    {100, 40}, {280, 40}, {350, 250}, {200, 300}, {50, 240}
};

int PlatePosition[2] = { 200,150 };

// Deadlock Fonksiyonları
void UpdateCanvas() {
    ReadImage("masaa.bmp", Table);

    PasteNon0(Plate, PlatePosition[0], PlatePosition[1], Table);

    // Filozofların durumları
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (PhilosopherStates[i] == 2) { // Yemek yeme durumu
            PasteNon0(PhilosopherEating, PhilosopherPositions[i][0], PhilosopherPositions[i][1], Table);
        }
        else { // Düşünme durumu
            PasteNon0(PhilosopherThinking, PhilosopherPositions[i][0], PhilosopherPositions[i][1], Table);
        }
    }

    // Çatallar
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (ForkAvailable[i]) {
            PasteNon0(Fork[i], ForkPositions[i][0], ForkPositions[i][1], Table);
        }
    }

    DisplayImage(F1, Table);
}

DWORD WINAPI PhilosopherTryTakeForks(LPVOID philosopherId) {
    int id = *(int*)philosopherId;
    int rightFork = (id + 1) % NUM_PHILOSOPHERS;
    int leftFork = id;

    // Filozoflar önce sağ çatalı almaya çalışacak
    PhilosopherStates[id] = 1; // Çatal bekleme
    UpdateCanvas();
    Sleep(1000);

    if (ForkAvailable[rightFork]) {
        ForkAvailable[rightFork] = false;
        UpdateCanvas();
        Sleep(5000);
    }

    if (ForkAvailable[leftFork] == false) {
        PhilosopherStates[id] = 0;
        UpdateCanvas();
    }

    return 0;
}

bool CheckDeadlock() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (PhilosopherStates[i] == 2) {
            return false; // Deadlock yok
        }
    }
    return true; // Deadlock oluştu
}

void StartSimulation(void* args) {
    int philosopherIds[NUM_PHILOSOPHERS];
    HANDLE threadHandles[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherIds[i] = i;
        threadHandles[i] = CreateThread(
            NULL, 0, PhilosopherTryTakeForks, &philosopherIds[i], 0, NULL);
        if (threadHandles[i] == NULL) {
            MessageBox(NULL, "Thread creation failed!", "Error", MB_OK | MB_ICONERROR);
            return;
        }
        Sleep(1000); // Küçük bir gecikme
    }

    WaitForMultipleObjects(NUM_PHILOSOPHERS, threadHandles, TRUE, INFINITE);
    Sleep(500);

    if (CheckDeadlock()) {
        MessageBox(NULL, "Deadlock oluştu!", "Uyarı", MB_OK | MB_ICONWARNING);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        CloseHandle(threadHandles[i]);
    }
}

// Semaphore Fonksiyonları
void PhilosopherStartsEatingWithSemaphore(int id) {
    int leftFork = id;
    int rightFork = (id + 1) % NUM_PHILOSOPHERS;

    WaitForSingleObject(ForkSemaphores[leftFork], INFINITE); // Sol çatalı al
    WaitForSingleObject(ForkSemaphores[rightFork], INFINITE); // Sağ çatalı al

    ForkAvailable[leftFork] = false;
    ForkAvailable[rightFork] = false;

    PhilosopherStates[id] = 2; // Yemek yiyor durumuna geç
    UpdateCanvas(); // Görseli güncelle
}

void PhilosopherStopsEatingWithSemaphore(int id) {
    int leftFork = id;
    int rightFork = (id + 1) % NUM_PHILOSOPHERS;

    PhilosopherStates[id] = 0; // Düşünme durumuna geç

    ForkAvailable[leftFork] = true;
    ForkAvailable[rightFork] = true;

    ReleaseSemaphore(ForkSemaphores[leftFork], 1, NULL); // Sol çatalı bırak
    ReleaseSemaphore(ForkSemaphores[rightFork], 1, NULL); // Sağ çatalı bırak

    UpdateCanvas(); // Görseli güncelle
}

void RunPhilosopherWithSemaphore(void*) {
    while (true) {
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            // Filozof düşünüyorsa ve çatallar uygunsa yemek yemeye geç
            if (PhilosopherStates[i] == 0 && ForkAvailable[i] && ForkAvailable[(i + 1) % NUM_PHILOSOPHERS]) {
                PhilosopherStartsEatingWithSemaphore(i); // Yemek yeme durumuna geç
                Sleep(2000); // Yemek yeme süresi

                // Ardışık olmayan bir filozofu kontrol et
                for (int j = 0; j < NUM_PHILOSOPHERS; j++) {
                    if (j != i && // Aynı filozof değil
                        abs(i - j) > 1 && // Ardışık değil
                        ForkAvailable[j] && ForkAvailable[(j + 1) % NUM_PHILOSOPHERS] && // Çatallar müsait
                        PhilosopherStates[j] == 0) { // Düşünen filozof
                        PhilosopherStartsEatingWithSemaphore(j);
                        Sleep(2000); // Yemek yeme süresi
                        PhilosopherStopsEatingWithSemaphore(j);
                        break; // Ardışık olmayan bir filozof bulundu
                    }
                }

                PhilosopherStopsEatingWithSemaphore(i); // İlk filozof yemek yemeyi bitirir
                Sleep(1000); // Düşünme süresi
            }
        }
    }
}

// Görselleri Yükleme
void LoadImages() {
    ReadImage("masaa.bmp", Table);
    ReadImage("plate.bmp", Plate);
    ReadImage("philosopher_thinking.bmp", PhilosopherThinking);
    ReadImage("philosopher_eating.bmp", PhilosopherEating);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ReadImage("fork.bmp", Fork[i]);
        ForkSemaphores[i] = CreateSemaphore(NULL, 1, 1, NULL); // Her çatal için semaphore
    }
    DiningSemaphore = CreateSemaphore(NULL, 2, 2, NULL); // Aynı anda en fazla 2 filozof yemek yiyebilir
    DisplayImage(F1, Table);
}

// ICBYTES Arayüzü
void ICGUI_main() {
    F1 = ICG_FrameThin(10, 10, 600, 600);
    ICG_Button(620, 10, 160, 40, "Yükle", LoadImages);
    ICG_TButton(620, 60, 160, 40, "Deadlock Başlat", StartSimulation, NULL);
    ICG_TButton(620, 110, 160, 40, "Semaphore Başlat", RunPhilosopherWithSemaphore, NULL);
    MLE = ICG_MLEditSunken(620, 160, 160, 150, "Yükle > Deadlock Başlat şeklinde çalıştırdıktan sonra programı kapatıp tekrardan Yükle > Semaphore Başlat yapınız lütfen.", SCROLLBAR_V);
    LoadImages();
}

void ICGUI_Create() {
    ICGUI_main();
}
