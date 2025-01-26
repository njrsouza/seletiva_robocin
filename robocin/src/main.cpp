#include <iostream>
#include "mbed.h"
#include "MPU6050.h"

using namespace std;

#define PI 3.14159
#define ALPHA 0.2 //Parametro para o filtro passa-baixa

//Filtro para eliminar uma parte dos ruídos e possui maior precisão na coleta dos dados 
void PassaBaixa(double novovalor[3], double valorfiltrado[3]) {
    for (int i = 0; i < 3; i++) {   //Utilizandoa a fórmula do passa-baixa - Parâmetro alpha faz parte desse filtro, definido previamente
        valorfiltrado[i] = ALPHA*novovalor[i] + (1 - ALPHA)*valorfiltrado[i];
    }
}

//Convertendo para rad/s 
void convertToRadians(double gyroRead[3]) {
    for (int i = 0; i < 3; i++) {           
        gyroRead[i] *= (PI / 180.0);
    }
}

//Calculo da movimentação angular - Integração da velocidade angular
void MovimAngular(Timer t, double gyro[3], double angulos[3]) {
    float tempo = t.read();
    for (int i = 0; i < 3; i++) {
        angulos[i] += (gyro[i] * tempo);

        //Mantendo o ângulo sempre entre -PI e PI
        if (angulos[i] > PI) {
            angulos[i] -= 2 * PI;
        } else if (angulos[i] < -PI) {
            angulos[i] += 2 * PI;
        }
    }
    t.reset(); //Para pegar mais um intervalo de tempo e integrar novamente
}

int main() {
    MPU6050 conexao(I2C_SDA, I2C_SCL);  
    double gyroRead[3] = {0.0, 0.0, 0.0};         //Leitura do giroscópio
    double filteredGyro[3] = {0.0, 0.0, 0.0};     //Velocidade angular filtrada pelo passa baixa 
    double variacao_angular[3] = {0.0, 0.0, 0.0}; //Variação angular
    Timer t, d;

    if (!conexao.initialize()) {
        cout << "Erro ao inicializar sensor" << endl;
        exit(1);
    }
    t.start(); //Timer para integração 

    conexao.setGyroConfig(GYRO_ST_OFF, GFS_250dps); //Configurando giroscópio com sensibilidade de 250 dps

    d.start();
    while (1) {
        conexao.readGyro(gyroRead); //Leitura do giroscópio 

        //Aplicando filtro passa-baixa
        PassaBaixa(gyroRead, filteredGyro);

        //Exibindo valores filtrados
        cout << "Velocidade angular filtrada (graus/s):" << endl;
        cout << "Eixo X: " << filteredGyro[0] << " graus/s" << endl;
        cout << "Eixo Y: " << filteredGyro[1] << " graus/s" << endl;
        cout << "Eixo Z: " << filteredGyro[2] << " graus/s" << endl;

        convertToRadians(filteredGyro); //Convertendo valores filtrados para rad/s

        //Exibindo valores em rad/s
        cout << "Velocidade angular em rad/s:" << endl;
        cout << "Eixo X: " << filteredGyro[0] << " rad/s" << endl;
        cout << "Eixo Y: " << filteredGyro[1] << " rad/s" << endl;
        cout << "Eixo Z: " << filteredGyro[2] << " rad/s" << endl;

        //Calculando movimentação angular
        MovimAngular(t, filteredGyro, variacao_angular);

        //Garantindo leituras a cada 5 ms
        while (d.read() < 0.05) {
        }
        d.reset();
    }

    return 0;
}
