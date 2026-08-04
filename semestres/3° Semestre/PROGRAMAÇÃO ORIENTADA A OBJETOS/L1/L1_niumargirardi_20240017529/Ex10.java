import java.util.Scanner;
public class Ex10 { // do meu beecrowd
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Digite o quanto de money vc tem(valor inteiro por favor): ");
        int valor = sc.nextInt();
        System.out.println("So isso?: R$ " + valor + " Reais");

        int[] notas = {100, 50, 20, 10, 5, 2, 1};
        for (int nota : notas) {
            int qtd = valor / nota;
            valor %= nota;
            System.out.println(qtd + " nota(s) de R$ " + nota + ",00");
        }
    }
}