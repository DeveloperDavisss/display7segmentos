#include <Arduino.h>

int mostrarSetpoint;
int setpoint;
int pinos_digito [7];
int pinos_segmento;  // definir conforme a quantidade

void taskDisplay(void *pvParameters) {
  int displayDigits[4];
  unsigned long lastDisplayUpdate = 0;

  for (;;) {
    // Verifica se o setpoint precisa ser exibido
    if (mostrarSetpoint) {
        // Converte o setpoint para exibição
        int setpointToDisplay = (int)(setpoint * 100); // Multiplica para exibir com precisão decimal
        displayDigits[0] = setpointToDisplay / 1000;       // Dígito dos milhares
        displayDigits[1] = (setpointToDisplay / 100) % 10; // Centenas
        displayDigits[2] = (setpointToDisplay / 10) % 10;  // Dezenas
        displayDigits[3] = setpointToDisplay % 10;         // Unidades

        // Exibe o setpoint no display
        for (int digit = 0; digit < 4; digit++) {
            for (int i = 0; i < 4; i++) {
                digitalWrite(pinos_digito[i], (i == digit) ? HIGH : LOW); // Ativa um dígito por vez
            }

            for (int segment = 0; segment < 8; segment++) {
                digitalWrite(pinos_segmento[segment], tabelaDisplay[displayDigits[digit]][segment]);
            }

            delay(5);  // Tempo para exibir o dígito
        }

        // Se já se passaram 3 segundos, muda para exibir a temperatura
        if (millis() - lastDisplayUpdate >= 3000) {
            mostrarSetpoint = false;  // Depois de 3 segundos, exibe a temperatura
            lastDisplayUpdate = millis();  // Atualiza o tempo
        }
    } else {
        // Aqui exibe a temperatura
        if (getSensorTemperature(&temperature, NTC_PIN)) {
            int tempToDisplay = (int)(temperature * 100);  // Multiplica para exibir com precisão decimal
            displayDigits[0] = tempToDisplay / 1000;       // Dígito dos milhares
            displayDigits[1] = (tempToDisplay / 100) % 10; // Centenas
            displayDigits[2] = (tempToDisplay / 10) % 10;  // Dezenas
            displayDigits[3] = tempToDisplay % 10;         // Unidades

            // Exibe a temperatura no display
            for (int digit = 0; digit < 4; digit++) {
                for (int i = 0; i < 4; i++) {
                    digitalWrite(pinos_digito[i], (i == digit) ? HIGH : LOW); // Ativa um dígito por vez
                }

                for (int segment = 0; segment < 8; segment++) {
                    digitalWrite(pinos_segmento[segment], tabelaDisplay[displayDigits[digit]][segment]);
                }

                delay(5);  // Tempo para exibir o dígito
            }
        } else {
            Serial.println("Erro ao ler o sensor de temperatura.");
        }

        delay(100); // Tempo entre as atualizações da temperatura
    }
  }
}
