#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// -------------------------- Basic Circuit Analysis Module --------------------------
// Calculate series resistors total resistance
float seriesResistors(int count, float resistors[]) {
    float total = 0.0;
    for (int i = 0; i < count; i++) total += resistors[i];
    return total;
}

// Calculate parallel resistors total resistance
float parallelResistors(int count, float resistors[]) {
    float total = 0.0;
    for (int i = 0; i < count; i++) {
        if (resistors[i] <= 0) {
            printf("❌ Error: Resistor value cannot be 0 or negative!\n");
            return -1.0;
        }
        total += 1.0 / resistors[i];
    }
    return 1.0 / total;
}

// Kirchhoff's Current Law (KCL) node current analysis
void kclAnalysis(int count, float currents[]) {
    float sum = 0.0;
    printf("\n📊 KCL Node Current Analysis (Inflow = Positive, Outflow = Negative):\n");
    for (int i = 0; i < count; i++) {
        sum += currents[i];
        printf("Current %d: %.2f A\n", i+1, currents[i]);
    }
    printf("Total Node Current: %.2f A\n", sum);
    if (fabs(sum) < 0.001) printf("✅ Complies with KCL (Node current balanced)\n");
    else printf("❌ Does not comply with KCL (Node current unbalanced)\n");
}

// Series circuit complete analysis (Voltage/Current/Power)
void seriesCircuitCalc(float voltage, int resCount, float resistors[]) {
    float totalRes = seriesResistors(resCount, resistors);
    float totalCurr = voltage / totalRes;
    printf("\n📈 Series Circuit Analysis Results:\n");
    printf("Supply Voltage: %.2f V | Total Resistance: %.2f Ω | Total Current: %.2f A\n", voltage, totalRes, totalCurr);
    printf("Resistor Details:\n");
    for (int i = 0; i < resCount; i++) {
        float v = totalCurr * resistors[i];
        float p = v * totalCurr;
        printf("Resistor%d (%.2fΩ): Voltage Drop %.2fV | Power %.2fW\n", i+1, resistors[i], v, p);
    }
}

// -------------------------- Resistor Colour Code Decoder Module --------------------------
// Structure for colour code mapping (digit, multiplier, tolerance)
typedef struct {
    char color[20];
    int digit;
    float multiplier;
    float tolerance;
} ResistorColor;

// Standard resistor colour code table
ResistorColor colorCodes[] = {
    {"black", 0, 1e0, -1.0},    {"brown", 1, 1e1, 1.0},    {"red", 2, 1e2, 2.0},
    {"orange", 3, 1e3, -1.0},  {"yellow", 4, 1e4, -1.0},  {"green", 5, 1e5, 0.5},
    {"blue", 6, 1e6, 0.25},    {"violet", 7, 1e7, 0.1},   {"grey", 8, 1e8, 0.05},
    {"white", 9, 1e9, -1.0},   {"gold", -1, 1e-1, 5.0},   {"silver", -1, 1e-2, 10.0}
};

// Find colour code details by colour name
ResistorColor* findColor(const char* color) {
    for (int i = 0; i < sizeof(colorCodes)/sizeof(colorCodes[0]); i++) {
        if (strcmp(color, colorCodes[i].color) == 0) return &colorCodes[i];
    }
    return NULL;
}

// Decode 4-band resistor
void decode4Band(char* c1, char* c2, char* c3, char* c4) {
    ResistorColor *d1 = findColor(c1), *d2 = findColor(c2), *m = findColor(c3), *t = findColor(c4);
    if (!d1 || !d2 || !m || !t || d1->digit == -1 || d2->digit == -1) {
        printf("❌ Invalid colour band combination!\n");
        return;
    }
    float resistance = (d1->digit * 10 + d2->digit) * m->multiplier;
    printf("\n🎨 4-Band Resistor Decoding Result:\n");
    printf("Resistance: %.2f Ω (%.2f kΩ) | Tolerance: ±%.1f%%\n", resistance, resistance/1000, t->tolerance);
}

// Decode 5-band resistor (high precision)
void decode5Band(char* c1, char* c2, char* c3, char* c4, char* c5) {
    ResistorColor *d1 = findColor(c1), *d2 = findColor(c2), *d3 = findColor(c3), *m = findColor(c4), *t = findColor(c5);
    if (!d1 || !d2 || !d3 || !m || !t || d1->digit == -1 || d2->digit == -1 || d3->digit == -1) {
        printf("❌ Invalid colour band combination!\n");
        return;
    }
    float resistance = (d1->digit * 100 + d2->digit * 10 + d3->digit) * m->multiplier;
    printf("\n🎨 5-Band Resistor Decoding Result:\n");
    printf("Resistance: %.2f Ω (%.2f kΩ) | Tolerance: ±%.2f%%\n", resistance, resistance/1000, t->tolerance);
}

// -------------------------- Scientific Calculator Module --------------------------
void scientificCalc() {
    int choice;
    float a, b, res;
    printf("\n🔢 Scientific Calculator Submenu:\n");
    printf("1. Trigonometric Functions (sin/cos/tan, radians)\n2. Exponential/Logarithmic (e^x / ln(x) / log10(x))\n");
    printf("3. Power Operation (a^b)\n4. Square Root/Cube Root\n5. Absolute Value/Rounding\n");
    printf("Select calculation type (1-5): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            printf("Enter angle (radians): "); scanf("%f", &a);
            printf("sin(%.2f)=%.4f | cos(%.2f)=%.4f | tan(%.2f)=%.4f\n",
                   a, sin(a), a, cos(a), a, tan(a));
            break;
        case 2:
            printf("1. e^x  2. ln(x)  3. log10(x): "); scanf("%d", &choice);
            printf("Enter x: "); scanf("%f", &a);
            if (choice==1) res=exp(a);
            else if (choice==2 && a>0) res=log(a);
            else if (choice==3 && a>0) res=log10(a);
            else { printf("❌ Invalid input!\n"); return; }
            printf("Result: %.4f\n", res);
            break;
        case 3:
            printf("Enter base a: "); scanf("%f", &a);
            printf("Enter exponent b: "); scanf("%f", &b);
            printf("%.2f^%.2f=%.4f\n", a, b, pow(a, b));
            break;
        case 4:
            printf("1. Square Root  2. Cube Root: "); scanf("%d", &choice);
            printf("Enter x: "); scanf("%f", &a);
            if (choice==1 && a>=0) res=sqrt(a);
            else if (choice==2) res=cbrt(a);
            else { printf("❌ Invalid input!\n"); return; }
            printf("Result: %.4f\n", res);
            break;
        case 5:
            printf("1. Absolute Value  2. Ceiling  3. Floor  4. Round: "); scanf("%d", &choice);
            printf("Enter x: "); scanf("%f", &a);
            switch(choice) {
                case 1: res=fabs(a); break;
                case 2: res=ceil(a); break;
                case 3: res=floor(a); break;
                case 4: res=round(a); break;
                default: printf("❌ Invalid selection!\n"); return;
            }
            printf("Result: %.4f\n", res);
            break;
        default: printf("❌ Invalid selection!\n");
    }
}

// -------------------------- Main Menu & Interaction Module --------------------------
int main() {
    int mainChoice;
    printf("=========================================\n");
    printf("    Multifunctional Electrical Engineering Tool\n");
    printf("=========================================\n");
    printf("1. Basic Circuit Analysis\n");
    printf("2. Resistor Colour Code Decoder\n");
    printf("3. Scientific Calculator\n");
    printf("4. Exit Program\n");
    printf("=========================================\n");
    printf("Select function (1-4): ");
    scanf("%d", &mainChoice);

    switch(mainChoice) {
        // Basic Circuit Analysis
        case 1: {
            int circuitChoice, count;
            float *components, voltage;
            printf("\n⚡ Circuit Analysis Submenu:\n");
            printf("1. Series Resistors Calculation\n2. Parallel Resistors Calculation\n3. KCL Node Analysis\n4. Series Circuit Complete Analysis\n");
            printf("Select (1-4): "); scanf("%d", &circuitChoice);

            switch(circuitChoice) {
                case 1:
                    printf("Enter number of series resistors: "); scanf("%d", &count);
                    components = (float*)malloc(count*sizeof(float));
                    for (int i=0; i<count; i++) {
                        printf("Resistor%d (Ω): ", i+1); scanf("%f", &components[i]);
                        if (components[i]<=0) { printf("❌ Resistor must be a positive value!\n"); return 1; }
                    }
                    printf("Total Series Resistance: %.2f Ω\n", seriesResistors(count, components));
                    free(components); break;
                case 2:
                    printf("Enter number of parallel resistors: "); scanf("%d", &count);
                    components = (float*)malloc(count*sizeof(float));
                    for (int i=0; i<count; i++) {
                        printf("Resistor%d (Ω): ", i+1); scanf("%f", &components[i]);
                    }
                    float parallelTotal = parallelResistors(count, components);
                    if (parallelTotal>0) printf("Total Parallel Resistance: %.2f Ω\n", parallelTotal);
                    free(components); break;
                case 3:
                    printf("Enter number of node currents (Inflow=+, Outflow=-): "); scanf("%d", &count);
                    components = (float*)malloc(count*sizeof(float));
                    for (int i=0; i<count; i++) {
                        printf("Current%d (A): ", i+1); scanf("%f", &components[i]);
                    }
                    kclAnalysis(count, components);
                    free(components); break;
                case 4:
                    printf("Enter supply voltage (V): "); scanf("%f", &voltage);
                    if (voltage<=0) { printf("❌ Voltage must be a positive value!\n"); return 1; }
                    printf("Enter number of series resistors: "); scanf("%d", &count);
                    components = (float*)malloc(count*sizeof(float));
                    for (int i=0; i<count; i++) {
                        printf("Resistor%d (Ω): ", i+1); scanf("%f", &components[i]);
                        if (components[i]<=0) { printf("❌ Resistor must be a positive value!\n"); return 1; }
                    }
                    seriesCircuitCalc(voltage, count, components);
                    free(components); break;
                default: printf("❌ Invalid selection!\n");
            }
            break;
        }

        // Resistor Colour Code Decoder
        case 2: {
            int bandCount;
            char c1[20], c2[20], c3[20], c4[20], c5[20];
            printf("\n🎨 Resistor Colour Code Decoder (Supports 4/5 bands)\n");
            printf("Available Colours: black, brown, red, orange, yellow, green, blue, violet, grey, white, gold, silver\n");
            printf("Enter number of bands (4 or 5): "); scanf("%d", &bandCount);

            if (bandCount == 4) {
                printf("Enter 1st band colour: "); scanf("%s", c1);
                printf("Enter 2nd band colour: "); scanf("%s", c2);
                printf("Enter 3rd band colour (multiplier): "); scanf("%s", c3);
                printf("Enter 4th band colour (tolerance): "); scanf("%s", c4);
                decode4Band(c1, c2, c3, c4);
            } else if (bandCount == 5) {
                printf("Enter 1st band colour: "); scanf("%s", c1);
                printf("Enter 2nd band colour: "); scanf("%s", c2);
                printf("Enter 3rd band colour: "); scanf("%s", c3);
                printf("Enter 4th band colour (multiplier): "); scanf("%s", c4);
                printf("Enter 5th band colour (tolerance): "); scanf("%s", c5);
                decode5Band(c1, c2, c3, c4, c5);
            } else {
                printf("❌ Only 4 or 5 bands are supported!\n");
            }
            break;
        }

        // Scientific Calculator
        case 3:
            scientificCalc();
            break;

        case 4:
            printf("👋 Program exited successfully!\n");
            break;

        default:
            printf("❌ Invalid function selection!\n");
            return 1;
    }

    return 0;
}