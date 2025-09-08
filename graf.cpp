#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace sf;
using namespace std;

Color obtenerColorDesdeTexto(const string& colorStr) {
        if (colorStr == "rojo") return Color::Red;
        if (colorStr == "verde") return Color::Green;
        if (colorStr == "azul") return Color::Blue;
        if (colorStr == "amarillo") return Color::Yellow;
        if (colorStr == "negro") return Color::Black;
        if (colorStr == "blanco") return Color::White;
        if (colorStr == "gris") return Color(128, 128, 128);
        if (colorStr == "marron") return Color(108, 59, 42);
        if (colorStr == "celeste") return Color(135, 206, 235);
        if (colorStr == "violeta") return Color(138, 43, 226);
        if (colorStr == "naranja") return Color(255, 165, 0);
        return Color::Magenta; 
}

int main() {
    // Abrir archivo de figuras
    ifstream figuras("figuras.txt");
    if (!figuras.is_open()) {
        cerr << "No se pudo abrir figuras.txt" << endl;
        return 1;
    }

    // Crear lienzo de 800x600
    int lienzo_ancho = 800, lienzo_alto = 600; // valor por defecto

    // Buscar y leer la línea del lienzo
    vector<string> lineas_figuras;
    string linea;
    while (getline(figuras, linea)) {
        if (linea.find("LIENZO") == 0) {
            istringstream iss(linea); 
            string tag;
            iss >> tag >> lienzo_ancho >> lienzo_alto;
        } else {
            lineas_figuras.push_back(linea);
        }
    }
    RenderTexture renderTexture(Vector2u(static_cast<unsigned int>(lienzo_ancho), static_cast<unsigned int>(lienzo_alto)));
    renderTexture.clear(Color(135, 206, 235));

    for (const string& linea : lineas_figuras){
        istringstream iss(linea);
        string tipo, nombre, color;
        iss >> tipo >> nombre;

        if (tipo == "CUADRADO") {
            int x, y, lado;
            iss >> x >> y >> lado >> color;
            RectangleShape cuadrado(Vector2f(lado, lado));
            cuadrado.setPosition(Vector2f(x, y));
            cuadrado.setFillColor(obtenerColorDesdeTexto(color));
            renderTexture.draw(cuadrado);
        }
        else if (tipo == "CIRCULO") {
            int x, y, radio;
            iss >> x >> y >> radio >> color;
            CircleShape circulo(radio);
            circulo.setPosition(Vector2f(x, y));
            circulo.setFillColor(obtenerColorDesdeTexto(color));
            renderTexture.draw(circulo);
        }
        else if (tipo == "RECTANGULO") {
            int x, y, ancho, alto;
            iss >> x >> y >> ancho >> alto >> color;
            RectangleShape rect(Vector2f(ancho, alto));
            rect.setPosition(Vector2f(x, y));
            rect.setFillColor(obtenerColorDesdeTexto(color));
            renderTexture.draw(rect);
        }
        else if (tipo == "TRIANGULO") {
            int x1, y1, x2, y2, x3, y3;
            iss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> color;
            ConvexShape triangulo;
            triangulo.setPointCount(3);
            triangulo.setPoint(0, Vector2f(x1, y1));
            triangulo.setPoint(1, Vector2f(x2, y2));
            triangulo.setPoint(2, Vector2f(x3, y3));
            triangulo.setFillColor(obtenerColorDesdeTexto(color));
            renderTexture.draw(triangulo);
        }
    }

    // Finalizar dibujo y guardar imagen
    renderTexture.display();
    Image finalImage = renderTexture.getTexture().copyToImage();
    if (!finalImage.saveToFile("salida.jpg")) {
        cerr << "Error al guardar salida.jpg" << endl;
        return 1;
    }

    cout << "Imagen generada exitosamente como salida.jpg" << endl;
    return 0;
}
