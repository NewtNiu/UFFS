public class Main {
    public static void main(String[] args) {
        // Exercicio 1
        System.out.println("Execicio 1: ");
        Livro livro1 = new Livro();

        Livro livro2 = new Livro("Dom Casmurro", "Machado de Assis", 1899);

        livro1.imprimirDados();
        livro2.imprimirDados();

        // Exercicio 2
        System.out.println("\nExecicio 2: ");
        Calculadora calc = new Calculadora();

        int resultadoInt = calc.soma(10, 20);
        System.out.println("Soma de inteiros: " + resultadoInt);

        double resultadoDouble = calc.soma(5.5, 7.3);
        System.out.println("Soma de doubles: " + resultadoDouble);

        // Exercicio 3
        System.out.println("\nExecicio 3: ");
        double f = Conversor.celsiusParaFahrenheit(25);
        System.out.println("25°C em Fahrenheit = " + f);

        double c = Conversor.fahrenheitParaCelsius(100);
        System.out.println("100°F em Celsius = " + c);
    }
}
