import java.util.Random;
import java.util.Scanner;
public class Ex12 {
    public static void main(String[] args) {
        Random random = new Random();
        int numeroSorteado = random.nextInt(100) + 1;
        Scanner scanner = new Scanner(System.in);
        int palpite = 0;

        System.out.println("Advinhe um Numero entre 1 a 100:");

        while (palpite != numeroSorteado) {
            System.out.print("Numero: ");
            palpite = scanner.nextInt();

            if (palpite < numeroSorteado) {
                System.out.println("O numero e maior");
            } else if (palpite > numeroSorteado) {
                System.out.println("O numero e menor");
            } else {
                System.out.println("Parabens bagual! Voce acertou o tal do numero: " + numeroSorteado + " Ta feliz agora?\n(mim robo nao quer mais jogar nao)");
            }
        }
    }
}
