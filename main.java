public class Main {
    public static void main(String[] args) {
        System.out.println("Hola, SonarQube!");
        
        int resultado = sumar(5, 7);
        System.out.println("5 + 7 = " + resultado);
    }

    public static int sumar(int a, int b) {
        return a + b;
    }
}
