import java.util.Scanner;
public class Ex9 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Quantas vezes o java atrapalha muita gente?: ");
        int n = sc.nextInt();
        
        for (int i = 1; i <= n; i++) {
            System.out.print(i + " ");
            for (int j = 1; j <= i; j++) {
                System.out.print("atrapalha ");
            }
            System.out.println("muita gente");
        }
    }
}