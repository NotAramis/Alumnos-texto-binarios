#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> // Para remove() y rename() 

using namespace std;

// 1. Estructura para Texto (usa string para el nombre, lo que facilita la lectura y escritura en texto, pero no es tan directo para binario)
struct Alumno {
    int id;
    string nombre;
    float nota;
};

// 2. Estructura para Binario (la diferencia principal es que el nombre es un arreglo de caracteres en lugar de un string, para facilitar la escritura y lectura en binario)
struct AlumnoBinario {
    int id;
    char nombre[50];
    float nota;
};

//modulo 1: texto
void crearTexto() {
    ofstream archivo("alumnos.txt", ios::app);
    Alumno a;

    cout << "ID: "; cin >> a.id;
	cin.ignore(); // Limpia el salto de línea para que funcione el getline correctamente
    cout << "Nombre: "; getline(cin, a.nombre);
    cout << "Nota: "; cin >> a.nota;

    archivo << a.id << " " << a.nombre << " " << a.nota << endl;
    archivo.close();
    cout << "Guardado.\n";
}

void listarTexto() {
    ifstream archivo("alumnos.txt");
    Alumno a;

    cout << "\n--- LISTA TEXTO ---\n";
    while (archivo >> a.id >> a.nombre >> a.nota) {
        cout << a.id << " | " << a.nombre << " | " << a.nota << endl;
    }
    archivo.close();
}

void buscarTexto() {
    ifstream archivo("alumnos.txt");
    int idBuscar;
    Alumno a;

    cout << "ID a buscar: "; cin >> idBuscar;

    while (archivo >> a.id >> a.nombre >> a.nota) {
        if (a.id == idBuscar) {
            cout << "Encontrado: " << a.nombre << " con nota " << a.nota << endl;
            archivo.close();
            return; // Termina la función aquí mismo porque ya lo encontró  
        }
    }
    cout << "No existe.\n";
    archivo.close();
}

void actualizarTexto() {
    ifstream archivo("alumnos.txt");
    ofstream temporal("temp.txt");
    int idBuscar;
    Alumno a;

    cout << "ID a actualizar: "; cin >> idBuscar;

	// Pasamos los datos del viejo al nuevo archivo, pero si encontramos el ID que queremos actualizar, cambiamos la nota antes de escribirlo en el nuevo archivo
    while (archivo >> a.id >> a.nombre >> a.nota) {
        if (a.id == idBuscar) {
            cout << "Nueva nota para " << a.nombre << ": ";
			cin >> a.nota; // Cambiamos la nota antes de guardarla en el nuevo archivo
        }
        temporal << a.id << " " << a.nombre << " " << a.nota << endl;
    }

    archivo.close();
    temporal.close();

	// Borramos el viejo y renombramos el nuevo para que tome su lugar 
    remove("alumnos.txt");
    rename("temp.txt", "alumnos.txt");
    cout << "Actualizado.\n";
}
// modulo 2: binario

void crearBinario() {
    ofstream archivo("alumnos.dat", ios::binary | ios::app);
    AlumnoBinario ab;

    cout << "ID: "; cin >> ab.id;
    cin.ignore();
    cout << "Nombre: "; cin.getline(ab.nombre, 50);
    cout << "Nota: "; cin >> ab.nota;

	// Forma corta de guardar la estructura completa en binario, sin necesidad de escribir cada campo por separado
    archivo.write((char*)&ab, sizeof(AlumnoBinario));
    archivo.close();
    cout << "Guardado en binario.\n";
}

void leerAleatorioBinario() {
    ifstream archivo("alumnos.dat", ios::binary);
    int n;

    cout << "Numero de registro (1, 2, 3...): "; cin >> n;

	// Saltamos matemáticamente a la posición deseada en el archivo, multiplicando el número de registro por el tamaño de la estructura
    archivo.seekg((n - 1) * sizeof(AlumnoBinario), ios::beg);

    AlumnoBinario ab;
	// Si logra leer algo ahí, lo muestra en pantalla, sino muestra un mensaje de error
    if (archivo.read((char*)&ab, sizeof(AlumnoBinario))) {
        cout << "ID: " << ab.id << " | Nombre: " << ab.nombre << " | Nota: " << ab.nota << endl;
    }
    else {
        cout << "Registro en blanco.\n";
    }
    archivo.close();
}

void listarBinario() {
    ifstream archivo("alumnos.dat", ios::binary);
    AlumnoBinario ab;

    cout << "\n--- LISTA BINARIA ---\n";
    while (archivo.read((char*)&ab, sizeof(AlumnoBinario))) {
        cout << ab.id << " | " << ab.nombre << " | " << ab.nota << endl;
    }
    archivo.close();
}

// al final solo nos queda el menú para llamar a cada función según la opción elegida por el usuario, y un bucle para que se repita hasta que elija salir (opción 0)
int main() {
    int opcion;
    do {
        cout << "\n1.Agregar(txt) 2.Listar(txt) 3.Buscar(txt) 4.Actualizar(txt)\n";
        cout << "5.Agregar(bin) 6.Buscar(bin) 7.Listar(bin) 0.Salir\n";
        cout << "Opcion: "; cin >> opcion;

        switch (opcion) {
        case 1: crearTexto(); break;
        case 2: listarTexto(); break;
        case 3: buscarTexto(); break;
        case 4: actualizarTexto(); break;
        case 5: crearBinario(); break;
        case 6: leerAleatorioBinario(); break;
        case 7: listarBinario(); break;
        }
    } while (opcion != 0);

    return 0;
}