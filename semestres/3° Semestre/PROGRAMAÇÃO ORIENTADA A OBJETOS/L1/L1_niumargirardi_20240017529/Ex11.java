public class Ex11 {
    public static void main(String[] args) {
        int contador = 0; 
        int numero = 2;  

        System.out.println("Os 15 primeiros números primos são:");
        while (contador < 15) {
            boolean primo = true; 
            for (int i = 2; i < numero; i++) {
                if (numero % i == 0) {
                    primo = false; 
                    break;
                }
            }
            if (primo) {
                System.out.print(numero + " ");
                contador++;
            }

            numero++;
        }
    }
}