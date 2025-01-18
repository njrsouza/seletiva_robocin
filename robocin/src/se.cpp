#include <iostream>
#include "mbed.h"
#include "MPU6050.h"

using namespace std;

#define PI 3.14159

// Função para converter graus/s para rad/s
void convertToRadians(double gyroRead[3]) {
    for (int i = 0; i < 3; i++) {
        gyroRead[i] *= (PI / 180.0);
    }
}

int main() {
    MPU6050 m(I2C_SDA, I2C_SCL);
    double gyroRead[3] = {0.0, 0.0, 0.0}; // Array estático para leitura do giroscópio

    if (!m.initialize()) {
        cout << "Erro ao inicializar sensor" << endl; 
        exit(1);
    }

    m.setGyroConfig(GYRO_ST_OFF, GFS_500dps); //Configurando giroscópio com sensibilidade de 500 dps

    // Leitura do giroscópio
    if (m.readGyro(gyroRead)) {
        cout << "Velocidade angular do eixo X: " << gyroRead[0] << " graus/s" << endl;
        cout << "Velocidade angular do eixo Y: " << gyroRead[1] << " graus/s" << endl;
        cout << "Velocidade angular do eixo Z: " << gyroRead[2] << " graus/s" << endl;

        // Converter para radianos/s
        convertToRadians(gyroRead);

        cout << "Velocidade angular em rad/s:" << endl;
        cout << "Eixo X: " << gyroRead[0] << " rad/s" << endl;
        cout << "Eixo Y: " << gyroRead[1] << " rad/s" << endl;
        cout << "Eixo Z: " << gyroRead[2] << " rad/s" << endl;

        // Aqui você pode implementar a lógica para calcular a integral dos valores.
        // Isso dependerá de um loop para ler continuamente os valores e integrar ao longo do tempo.
    } else {
        cout << "Erro ao ler dados do giroscópio" << endl; 
        exit(2);
    }

    return 0;
}
